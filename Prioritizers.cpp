#include "Prioritizers.hpp"
#include <climits>
// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax {

namespace prioritizers {

vector<TreeNode *>* simplePrioritizer(TreeNode *root, vector<TreeNode*>* prioritizedVector);
vector<TreeNode *>* complexPrioritizer(TreeNode *root, vector<TreeNode*>* prioritizedVector);
vector<TreeNode *>* dummy(TreeNode *root);
TreeNode* getNextPriorityNode(TreeNode *root);
TreeNode* getNextPriorityNodeWithPrefixMatch(TreeNode *root, vector<TreeNode*>* prioritizedVector);
int getMaxNumSharedPrefix(TreeNode *root, vector<TreeNode*>*prioritizedVector);
int getMinNumSharedPrefix(TreeNode *root, vector<TreeNode*>*prioritizedVector);
// bool tweakSortBinaryInsert(TreeNode *node, vector<TreeNode *>* prioritizedVector);
// vector<TreeNode *>* tweakSortPrioritization(TreeNode *root, vector<TreeNode*>* prioritizedVector);

vector<TreeNode *>* prioritizeWithAlgorithm(int algoNum, TreeNode *root, vector<TreeNode*>* prioritizedVector) {
	
	switch (algoNum) {
		case 0:
			// cout << "simple algorithm" << endl;
			return simplePrioritizer(root, prioritizedVector);
			break;
		case 1:
			// cout << "complex algorithm" << endl;
			return complexPrioritizer(root, prioritizedVector);
			break;
		default:
			return dummy(root);
	}
}

vector<TreeNode *>* simplePrioritizer(TreeNode *root, vector<TreeNode*>* prioritizedVector) {
	(void)root;
	
	unsigned int numOfLeafNodes = prioritizedVector->size();	
	prioritizedVector->clear();

	while(root->getRevisionCount() < numOfLeafNodes) {		
		prioritizedVector->push_back(getNextPriorityNode(root));
	}
	return prioritizedVector;
}

vector<TreeNode *>* complexPrioritizer(TreeNode *root, vector<TreeNode*>* prioritizedVector) {
	(void)root;
	
	vector<TreeNode*> *sentList = new vector<TreeNode*>;
	TreeNode* sentNode;
	// prioritizedVector->clear();
	sentNode = prioritizedVector->at(0);
	sentNode->markNode(true);
	sentList->push_back(sentNode);
	
	while(sentList->size() < prioritizedVector->size())
	{	
		int leastSharedPrefixCountNow = INT_MAX;
		for (unsigned int i=1; i<prioritizedVector->size(); i++)
		{
			if (!prioritizedVector->at(i)->isNodeMarked())
			{
				if (leastSharedPrefixCountNow > getMaxNumSharedPrefix(prioritizedVector->at(i), sentList))
				{
					leastSharedPrefixCountNow = getMaxNumSharedPrefix(prioritizedVector->at(i), sentList);
					sentNode = prioritizedVector->at(i);
				}
			}			
		}

		// cout << sentNode->getName() << " is selected " << leastSharedPrefixCountNow << endl;

		sentNode->markNode(true);
		sentList->push_back(sentNode);
	}

	// prioritizedVector->clear();
	// prioritizedVector = sentList;
	// return prioritizedVector;
	return sentList;
}

vector<TreeNode *>* dummy(TreeNode *root) {
	vector<TreeNode *> *prioritizedVector = new vector<TreeNode *> ();
	prioritizedVector->push_back(root);

	// unordered_map<string, TreeNode*> child = root->getChildrenMap();
	// for(auto it = child.begin(); it != child.end(); ++it ) {
	// 	TreeNode *n = it->second;
	// 	prioritizedVector->push_back(n);
	// }

	vector<TreeNode*> children = root->getChildren();
	for(unsigned int i=0; i<children.size(); i++ ) {
		TreeNode *n = children[i];
		prioritizedVector->push_back(n);
	}

	return prioritizedVector;
}

TreeNode* getNextPriorityNode(TreeNode *root) {
	if(root == NULL) {
		return NULL;
	}		

	root->updateRevisionCount(root->getRevisionCount()+1);

	if(root->isDataNode() && !(root->isNodeMarked())) {				
		// cout << "upload:" << root->getName() << endl;
		root->markNode(true);		
		return root;
	}
	
	// unordered_map<string, TreeNode*> children = root->getChildrenMap();		
	vector<TreeNode*> children = root->getChildren();

	if(children.size() > 0) {		
		unsigned long long int leastRevisionCountNow = ULLONG_MAX;
		TreeNode *nodeWithLeastCount = NULL;
		
		for(unsigned int i=0; i<children.size(); i++ )
		{			
			TreeNode* child = children[i];
			if(child->getRevisionCount() < child->getTreeSize() || child->getRevisionCount() == 0) {
				if(nodeWithLeastCount == NULL || (nodeWithLeastCount != NULL && leastRevisionCountNow > child->getRevisionCount())) {
					nodeWithLeastCount = child;
					leastRevisionCountNow = child->getRevisionCount();
				}
			}
		}

		return getNextPriorityNode(nodeWithLeastCount);			
	}
	return NULL;
}

// TreeNode* getNextPriorityNodeWithPrefixMatch(TreeNode *root, vector<TreeNode*>* prioritizedVector) {

// 	vector<TreeNode*> candidateNode;

// 	if(root == NULL) {
// 		return NULL;
// 	}	

// 	root->updateRevisionCount(root->getRevisionCount()+1);

// 	if(root->isDataNode() && !(root->isNodeMarked())) {				
// 		cout << "upload:" << root->getName() << endl;
// 		root->markNode(true);		
// 		return root;
// 	}

// 	vector<TreeNode*> children = root->getChildren();

// 	if(children.size() > 0) {		
// 		unsigned long long int leastRevisionCountNow = ULLONG_MAX;
// 		int leastSharedPrefixCountNow = INT_MAX;
// 		TreeNode *nodeWithLeastCountLeastPrefixMatch = NULL;
		
// 		for(unsigned int i=0; i<children.size(); i++ )
// 		{			
// 			TreeNode* child = children[i];
// 			int numSharePrefix = getMaxNumSharedPrefix(child, prioritizedVector);

// 			cout << child->getName() << "'s numSharePrefix = ' " << numSharePrefix << " and leastSharedPrefixCountNow = " << leastSharedPrefixCountNow << endl;

// 			if (leastSharedPrefixCountNow > numSharePrefix)
// 			{
// 				cout << " leastSharedPrefixCountNow > numSharePrefix " << endl;
// 				leastSharedPrefixCountNow = numSharePrefix;
// 				candidateNode.clear();
// 				candidateNode.push_back(child);
// 			} else if (leastSharedPrefixCountNow == numSharePrefix)
// 			{
// 				cout << " leastSharedPrefixCountNow == numSharePrefix " << endl;
// 				candidateNode.push_back(child);	
// 			}
// 		}

// 		if (candidateNode.empty())
// 		{
// 			cout << "something wrong" << endl;
// 		} else if (candidateNode.size() == 1)
// 		{						
// 			nodeWithLeastCountLeastPrefixMatch = candidateNode[0];
// 		} else
// 		{			
// 			for(unsigned int i=0; i<candidateNode.size(); i++)
// 			{
// 				TreeNode* child = candidateNode[i];
// 				if(child->getRevisionCount() < child->getTreeSize() || child->getRevisionCount() == 0) {
// 					if(nodeWithLeastCountLeastPrefixMatch == NULL || (nodeWithLeastCountLeastPrefixMatch != NULL && leastRevisionCountNow > child->getRevisionCount())) {
// 						nodeWithLeastCountLeastPrefixMatch = child;
// 						leastRevisionCountNow = child->getRevisionCount();
// 					}
// 				}	
// 			}
			
// 		}
// 		cout << nodeWithLeastCountLeastPrefixMatch->getName() << " is selected to upload" << endl;	
// 		return getNextPriorityNodeWithPrefixMatch(nodeWithLeastCountLeastPrefixMatch, prioritizedVector);			
// 	}
// 	return NULL;
// }

int getMaxNumSharedPrefix(TreeNode *root, vector<TreeNode*>*prioritizedVector) {

	int max=0;
	// cout << "vector size: " << prioritizedVector->size() << endl;
	for (unsigned int i=0; i<prioritizedVector->size(); i++)
	{
		// cout << "max: " << max << "root->getNumSharedPrefix(prioritizedVector->at(i)) " << root->getNumSharedPrefix(prioritizedVector->at(i)) << endl;
		if(max < root->getNumSharedPrefix(prioritizedVector->at(i)))
		{
			max = root->getNumSharedPrefix(prioritizedVector->at(i));
		}
	}
	return max;
}

int getMinNumSharedPrefix(TreeNode *root, vector<TreeNode*>*prioritizedVector) {

	int min=INT_MAX;
	// cout << "vector size: " << prioritizedVector->size() << endl;
	for (unsigned int i=0; i<prioritizedVector->size(); i++)
	{
		// cout << "max: " << max << "root->getNumSharedPrefix(prioritizedVector->at(i)) " << root->getNumSharedPrefix(prioritizedVector->at(i)) << endl;
		if(min > root->getNumSharedPrefix(prioritizedVector->at(i)))
		{
			min = root->getNumSharedPrefix(prioritizedVector->at(i));
		}
	}
	return min;
}

}
}
}