/**
 * TODO: No way to handle realtime case.
 */

#include <ndn-cxx/face.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "InfoMaxConsumer.hpp"
#include "InvalidPrefixException.hpp"

using namespace std;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax { 

/**
 * InfoMax consumer implementation.
 */

InfoMaxConsumer :: InfoMaxConsumer (Name &prefix)
{
	this->prefix = prefix;
	this->windowSize = 10;
	this->indexNum = 1;
}

InfoMaxConsumer :: ~InfoMaxConsumer ()
{
	this->prefix = "";
	this->windowSize = -1;
	this->indexNum = -1;
}

/**
 * Function for expressing InfoMax interest.
 */
bool InfoMaxConsumer :: expressInfoMaxInterest (Interest &interest, unsigned long long int listNumber, unsigned long long int lastKnownRevisionNumber)
{
	
	std::ostringstream stringStream;
	stringStream << interest.getName().toUri();
	stringStream << INFOMAX_INTEREST_TAG;
	stringStream << INFOMAX_INTEREST_AUX_CHAR;
	stringStream << listNumber;
	stringStream << INFOMAX_INTEREST_AUX_CHAR;
	stringStream << lastKnownRevisionNumber;
	std::string infoMaxInterestStr = stringStream.str();		
	vector<string> *names = new vector<string>();

	Name *newName =new Name(interest.getName());
	newName->set(infoMaxInterestStr);	
	interest.setName(*newName);
	// cout<< "sending InfoMax interest: " << interest.getName().toUri()<<endl;

	face.expressInterest(interest, 
						bind(onList, _1, _2, names),
						bind(onInfoMaxTimeout, _1));	

	// processEvents will block until the requested data received or timeout occurs	
	try {
 		face.processEvents();
	}
	catch (const std::exception& e) {
   		std::cerr << "ERROR: " << e.what() << endl;
	}

	if(names->empty()) {
		// cout << "You got all!" << endl;
		return false;
	}
   
	for (unsigned int i=0; i<names->size();i++){
		if (this->receivedElements.count(names->at(i)) <= 0) {
			// cout << names->at(i) << endl;
			Name elementName = Name(prefix);
			elementName.append(names->at(i));
			Interest elementInterest(elementName);    		
			elementInterest.setScope(1);
			elementInterest.setInterestLifetime(time::seconds(1));
			elementInterest.setMustBeFresh(true);
			this->expressInterest(elementInterest);			
		}				

		if (i % this->windowSize == 0)
		{
			try {
 				face.processEvents();
			} catch (const std::exception& e) {
   				std::cerr << "ERROR: " << e.what() << endl;
			}
		}		
	}

	return true;
}

vector<string> InfoMaxConsumer :: convertInfoMaxList(string list)
{
	string buf; // Have a buffer string
	stringstream ss(list); // Insert the string into a stream
	vector<string> tokens; // Create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

bool InfoMaxConsumer :: expressInterest (const Interest &interest)
{
	face.expressInterest(interest,
						bind(onData, _1, _2),
						bind(onTimeout, _1));
	
	receivedElements.insert(interest.getName().toUri());
	return true;
}

void InfoMaxConsumer :: onList(const Interest& interest, Data& data, vector<string> *names)
{
	(void)interest;
	string receivedContent = reinterpret_cast<const char*>(data.getContent().value());
	receivedContent = receivedContent.substr(0, data.getContent().value_size());
	vector<string> temp = convertInfoMaxList(receivedContent);
	(*names) = temp;
}

void InfoMaxConsumer :: onInfoMaxTimeout(const Interest& interest)
{
	cout << "Timeout for: " << interest.getName() << endl;
}

void InfoMaxConsumer :: onData(const Interest& interest, Data& data)
{
    string receivedContent = reinterpret_cast<const char*>(data.getContent().value());
    receivedContent = receivedContent.substr(0, data.getContent().value_size());

    cout << "I: " << interest.toUri() << endl;
    cout << "D: " << data.getName().toUri() << endl;    
}

void InfoMaxConsumer :: onTimeout(const Interest& interest)
{
  (void)interest;
  cout << "Timeout" << endl;
}

bool InfoMaxConsumer :: get ()
{	
 	Interest i(this->prefix);
    i.setScope(1);
    i.setInterestLifetime(time::seconds(10));
    i.setMustBeFresh(true);
    if(!this->expressInfoMaxInterest (i, indexNum++, 0ull))
    {
    	// cout << "no more data" << endl;
    	return false;
    }
    return true;
}

bool InfoMaxConsumer :: destroy()
{	
	delete this;

	return true;
}
}
}
