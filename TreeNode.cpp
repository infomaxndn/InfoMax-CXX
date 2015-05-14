#include "TreeNode.hpp"

using namespace std;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax { 


TreeNode :: TreeNode(Name &name, Data &data, TreeNode *parent) 
{
	this->data = new Data(data);
	init(name, parent);	
}
TreeNode :: TreeNode(Name &name, TreeNode *parent)
{
	init(name, parent);
	this->data = NULL;
}

void TreeNode :: init(Name &name, TreeNode *parent) 
{
	this->name = name;
  	(this->children).clear();
  	// (this->childrenMap).clear();
  	this->isMarked = false;
  	this->parent = parent;

	this->revisionCount = 0;
  	this->treeSize = 0;
}

Name TreeNode :: getName () 
{
	return this->name;
}

vector<TreeNode *> TreeNode :: getChildren () 
{
	return (this->children);
}

// unordered_map<string, TreeNode*> TreeNode :: getChildrenMap()
// {
// 	return (this->childrenMap);
// }

bool TreeNode :: updateRevisionCount (unsigned long long int revisionCount) 
{
	this->revisionCount = revisionCount;
	return true;
}
	
TreeNode* TreeNode :: getParent()
{
	return this->parent;
}

unsigned long long int TreeNode :: getRevisionCount()
{
	return this->revisionCount;
}

bool TreeNode :: isDataNode()
{
	return !(data == NULL);
}

bool TreeNode :: isLeafNode()
{
	if (this->getChildren().empty())
		return true;

	return false;
}

bool TreeNode :: markNode(bool status)
{
	this->isMarked = status;
	return true;
}

bool TreeNode :: isNodeMarked()
{
	return this->isMarked;	
}

bool TreeNode :: freeData()
{
	return data = NULL;	
}

bool TreeNode :: removeChild (TreeNode * child) 
{
	if (child == NULL) {
		return false;
	}

	for (unsigned int i=0; i <= this->children.size(); i++)
	{
		if (this->children.at(i)->getName().equals(child->getName()))
		{
			// cout << this->children.at(i)->getName() << " is deleted" << endl;
			this->children.erase(this->children.begin()+i);						
			break;
		} else
		{
			if (i == this->children.size())
			{
				cout << "Can't find node for deletion" << endl;
				return false;
			}
		}
	}

	unsigned long long int newSize = this->parent->getTreeSize() - this->getTreeSize();
	this->parent->setTreeSize(newSize);	

	return true;	
}

bool TreeNode :: addChild (TreeNode * child) 
{
	if (child == NULL) {
		return false;
	}

	(this->children).push_back(child);
	// unsigned int lastElement = child->getName().size() - 1;
	// (this->childrenMap).insert({child->getName().get(lastElement).toUri(), child});
	
	if (child->isDataNode())
	{
		unsigned long long int newSize = getTreeSize()+1;
		this->setTreeSize(newSize);	
	}
	
	// child->pseudoName = (this->children).size();
	// reverseConcat(child);
	// child->reverseConcatName.pop_back();
	
	// if(this->isDataNode())
	// {
	//  Do we still need this?
	// 	this->data = NULL; // Only leaf nodes can have data!
	// }

	return true;
}

bool TreeNode :: setTreeSize (unsigned long long int treeSize)
{
	if(treeSize < (this->treeSize)) {
		return false;
	}
	unsigned long long int difference = treeSize - getTreeSize();
	this->treeSize = treeSize;
	if(parent != NULL){
		(this->parent)->setTreeSize((this->parent)->getTreeSize() + difference);
	}
	return true;
}

unsigned long long int TreeNode :: getTreeSize ()
{
	return treeSize;
}

Data* TreeNode :: getData ()
{
	return data;
}

int TreeNode :: getNumSharedPrefix(TreeNode *node)
{	
	int cnt = 0;
	unsigned int nameSize = min(this->getName().size(), node->getName().size());

	for (unsigned int i=0 ; i<nameSize; i++)
	{	
		string name1 = this->getName().get(i).toUri();
		string name2 = node->getName().get(i).toUri();			
		// cout << name1 << " ==? " << name2 << endl;
		if(name1.compare(name2) == 0)
		{
			// cout << "true" << endl;
			cnt++;
		}
		else
			break;
	}
	return cnt;
}

// void TreeNode :: reverseConcat(TreeNode *node)
// {
// 	reverseConcat(node, node);
// }

// void TreeNode :: reverseConcat(TreeNode *node, TreeNode *super)
// {
// 	(node->reverseConcatName).push_back(super->pseudoName);
// 	if(super->parent != NULL)
// 	{
// 		super = super->parent;
// 		reverseConcat(node, super);
// 	}
// 	return;
// }

void TreeNode :: printTreeNode()
{
	string parentStr = "null";
	if (parent != NULL) {
		parentStr = parent->getName().toUri();
	}
	cout << "Name: " << name << " isMarked: " << isMarked << " Parent: " << parentStr << " revisionCount: " << revisionCount << " treeSize: " << treeSize << endl;
  	
 //  	cout << "ReverseConcatName: ";
	// for(unsigned int i=0; i<reverseConcatName.size(); i++)
	// {
	// 	if(i == reverseConcatName.size()-1)
	// 		cout << reverseConcatName[i] ;
	// 	else
	// 		cout << reverseConcatName[i] << " : ";
	// }
	// cout << endl;

  	cout << "Children: "<<endl;

  	for(unsigned int i = 0 ; i < this->children.size() ; ++i ) {
  		TreeNode *n = this->children[i];
		std::cout << " " << n->getName() << ":";		
		n->printTreeNode();
	}

 	// 	for(auto it = childrenMap.begin(); it != childrenMap.end(); ++it ) {
	// 	std::cout << " " << it->first << ":";
	// 	TreeNode *n = it->second;
	// 	n->printTreeNode();
	// }
}

void TreeNode :: printTreeNodeName()
{
	string parentStr = "null";
	if (parent != NULL) {
		parentStr = parent->getName().toUri();
	}
	cout << "Name: " << name << endl;

  	for(unsigned int i = 0 ; i < this->children.size() ; ++i ) {
  		TreeNode *n = this->children[i];
		std::cout << " " << n->getName() << ":";		
		n->printTreeNodeName();
	}

 	//for(auto it = childrenMap.begin(); it != childrenMap.end(); ++it ) {		
	// 	TreeNode *n = it->second;
	// 	n->printTreeNodeName();
	//}
}

// vector<unsigned long long int> TreeNode :: getReverseConcatName()
// {
// 	return this->reverseConcatName;
// }


// int TreeNode :: compareTreeNodes(TreeNode* A, TreeNode* B) {
// 	if(A->getReverseConcatName().size() > B->getReverseConcatName().size()) {
// 		return 1;
// 	} 
// 	else if(A->getReverseConcatName().size() < B->getReverseConcatName().size()) {
// 		return -1;
// 	} 
// 	else {
// 		for(unsigned int i=0; i<A->getReverseConcatName().size(); i++) {
// 			if(A->getReverseConcatName().at(i) > B->getReverseConcatName().at(i)) {	
// 				return 1;
// 			}
// 			if(A->getReverseConcatName().at(i) < B->getReverseConcatName().at(i)) {	
// 				return -1;
// 			}
// 		}
// 		return 0;
// 	}
// }

} // namespace infomax
} // namespace ndn