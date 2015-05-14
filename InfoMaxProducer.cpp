#include "InfoMaxProducer.hpp"
#include "Prioritizers.hpp"
#include <string.h>

using namespace std;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax { 

/**
 * InfoMax Producer implementation.
 */
InfoMaxProducer :: InfoMaxProducer (Name &prefix)
{
	if (prefix.empty()) {
		throw prefixException;
	}
	this->prefix = prefix;

	// NULL casted as a TreeNode ptr
	ndn::infomax::TreeNode *nullPtr = NULL;

	this->root = new TreeNode (*new Name(prefix), nullPtr);	
	this->prioritizedVector = new vector<TreeNode*>;
	this->listSize = 10;
	this->whichAlgorithm = 0;

}

InfoMaxProducer :: ~InfoMaxProducer ()
{
	if (prefix.empty()) {
		throw prefixException;
	}

	this->prefix = "";
	this->root = NULL;	
	this->prioritizedVector = NULL;
	this->listSize = 0;
	this->whichAlgorithm = -1;
}

bool InfoMaxProducer :: add (Name &postfix, Data& data)
{		
	unsigned int lastElement = postfix.size();

	TreeNode *prev = root;	

	for (unsigned int i = 0; i <= postfix.size() ; ++i) {			
		vector<TreeNode*> prevChildren = prev->getChildren();					
		TreeNode *curr = NULL;
		for(unsigned int j=0; j<prevChildren.size(); j++) {
			if(prevChildren[j]->getName().equals(postfix.getSubName(0, i)))
			{
				curr = prevChildren[j];
				break;
			}
		}
		
		if(curr == NULL) {
			if(i==lastElement) {					
				curr = new TreeNode(postfix, data, prev);
				prev->addChild(curr);			
				this->prioritizedVector->push_back(curr);				
			} else {								
				Name *insertName = new Name(postfix.getSubName(0, i).toUri());
				curr = new TreeNode(*insertName, prev);								
				prev->addChild(curr);									
			}
		}
		prev = curr;				
	}	
	return false;
}

bool InfoMaxProducer :: deletes (Name &postfix)
{	
	 TreeNode *prev = root;	

	for (unsigned int i = 0; i <= postfix.size() ; ++i) {			
		vector<TreeNode*> prevChildren = prev->getChildren();					
		TreeNode *curr = NULL;
		for(unsigned int j=0; j<prevChildren.size(); j++) {
			if(prevChildren[j]->getName().equals(postfix.getSubName(0, i)))
			{
				curr = prevChildren[j];
				break;
			}
		}
		if (curr == NULL)
		{
			cout << "can't find postfix" << endl;
			return false;
		} 
		prev = curr;
	}

	TreeNode *parent = prev->getParent();
	if(parent->removeChild(prev))
	{
		cout << prev->getName() << " remove succeeded" << endl;
	}else
	{
		cout << postfix << " remove failed" << endl;
	}
	
	return true;
}

bool InfoMaxProducer :: destroy()
{
	m_face.unsetInterestFilter(prefixID);
	delete this;

	return true;
}

bool InfoMaxProducer :: prioritize() {	

	string content;	
	prioritizedVector = prioritizers::prioritizeWithAlgorithm(this->whichAlgorithm, root, prioritizedVector);		

	return true;
}

/*bool InfoMaxProducer :: findSimilarObject(Name objectName, int numObjects)
{
	vector<TreeNode *> *similarObjectVector;

	cout << getNumSharedPrefix(Name("/uiuc/edu/A"), Name("/uiuc/edu"));

	return false;
}*/

void InfoMaxProducer :: onInterestHandler (InfoMaxProducer &producer, const Name& name, const Interest& interest)
{	
	unsigned int lastElement = interest.getName().size()-1;
	if(interest.getName().get(lastElement-1).toUri().compare(INFOMAX_INTEREST_TAG) == 0)
	{
		onInfoMaxInterest (producer, name, interest);
	} else {		
		onInterest (producer, name, interest);
	}
}

void InfoMaxProducer :: onRegisterFailed(InfoMaxProducer &producer, const string& reason)
{
	cerr << "ERROR: Failed to register prefix in local hub's daemon (" << reason << ")"
			<< endl;
	(producer.m_face).shutdown();
}

void InfoMaxProducer :: onInfoMaxInterest (InfoMaxProducer &producer, const Name& name, const Interest& interest)
{	

	(void)name;
	int lastElement = interest.getName().size() - 1;
	string auxiliaryInformation = interest.getName().get(lastElement).toUri();
	pair <unsigned long long int, unsigned long long int> information = getRevisionAndListNumber(auxiliaryInformation);
	unsigned long long int requiredListNumber = information.first;
	unsigned long long int lastKnownRevision = information.second;	

	(void)lastKnownRevision;

	unsigned long long int listSize = producer.listSize;
	unsigned long long int vectorStartIdx = (requiredListNumber-1)*listSize;

	listSize = ((vectorStartIdx + listSize) >= producer.prioritizedVector->size())?producer.prioritizedVector->size()-vectorStartIdx:listSize;
	unsigned long long int vectorEndIdx = vectorStartIdx + listSize -1;

	cout<<"preparing InfoMaxList: "<<endl;
	string *listString = prepareInfoMaxListString(producer.prioritizedVector, vectorStartIdx, vectorEndIdx);
    
    const string content = string(*listString);
	cout<<"Sending InfoMaxList: "<<content<<endl;
	
	Name infoMaxName(interest.getName());
	infoMaxName.appendVersion();

	shared_ptr<Data> data = make_shared<Data>();	
	data->setName(infoMaxName);
	data->setFreshnessPeriod(time::seconds(10));
	data->setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.size());

	(producer.m_keyChain).sign(*data);
	(producer.m_face).put(*data);	
}

string* InfoMaxProducer :: prepareInfoMaxListString(vector<TreeNode *> *nodeVector, unsigned long long int startIdx, unsigned long long int endIdx)
{
	string *listString = new string();
	unsigned int i = 0;

	if (startIdx >= nodeVector->size()) {
		return listString;
	}

	while(startIdx < nodeVector->size() && startIdx <= endIdx) {
		listString->append(nodeVector->at(startIdx)->getName().toUri());
		listString->push_back(' ');
		startIdx ++;
		i++;
	}	
	return listString;
}

Name InfoMaxProducer :: getPrefix()
{
	return this->prefix;
}

pair <unsigned long long int, unsigned long long int> InfoMaxProducer :: getRevisionAndListNumber(string auxiliaryInformation)
{
	string tag = "%23"; //ndn::Name() converts '#' to the HTML encoded version, i.e, %23.
	
	size_t found = auxiliaryInformation.find(tag);	
	size_t rfound = auxiliaryInformation.rfind(tag);

	string listNumberStr = auxiliaryInformation.substr(found+tag.size(), rfound-found-tag.size());
	string revisionNumberStr = auxiliaryInformation.substr(rfound+tag.size());

	pair <unsigned long long int, unsigned long long int> values (strtoull(listNumberStr.c_str(), 0, 10), strtoull(revisionNumberStr.c_str(), 0, 10));

	return values;
}

void InfoMaxProducer :: onInterest (InfoMaxProducer &producer, const Name& name, const Interest& interest)
{
	(void)name;
	Name interestName = interest.getName();
	cout << "Recieved Interest: " << interestName.toUri() << endl;
	
	// int lastElement = interestName.size() - 1;
	unsigned int i = 0;	
	if(interestName.get(i).toUri() != producer.prefix.get(i).toUri()) {
		cout<<"Invalid Interest."<<endl;
		return;
	}

	Name interestPostfix = Name(interestName.getSubName(producer.getPrefix().size()));

	TreeNode *prev;
	for (i = 0; i < producer.prioritizedVector->size() ; i++)
	{
		if (producer.prioritizedVector->at(i)->getName().equals(interestPostfix))
		{
			prev = producer.prioritizedVector->at(i);
			break;
		}
	}

	cout<<"found: "<<prev->getName().toUri()<<endl;	

	Name infoMaxName(interest.getName());
	infoMaxName.appendVersion();

	Data data = *(prev->getData());
	data.setName(infoMaxName);
 	data.setFreshnessPeriod(time::seconds(10));

	(producer.m_keyChain).sign(data);
	(producer.m_face).put(data);
	cout<<"sending: "<<data.getName().toUri()<<endl;
	
}

void InfoMaxProducer :: run ()
{
	
	prioritize();
	// root->printTreeNodeName();	

	prefixID = m_face.setInterestFilter(prefix, 
							 bind(onInterestHandler, ref(*this), _1, _2),
							 RegisterPrefixSuccessCallback(),
							 bind(onRegisterFailed, ref(*this), _2));

	try {
   		m_face.processEvents();
	}
	catch (const std::exception& e) {
   		std::cerr << "ERROR: " << e.what() << endl;
	}
}

vector<TreeNode *> InfoMaxProducer :: getPrioritizedVector()
{
	return *prioritizedVector;
}

}
}
