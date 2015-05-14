#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <unordered_map>
#include <ndn-cxx/face.hpp>
using namespace std;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax { 

class TreeNode
{
private:
	Name name;
  	vector<TreeNode *> children;
  	// unordered_map<string, TreeNode*> childrenMap;
  	// unordered_map<string, TreeNode*&> children;
  	bool isMarked;
  	Data *data = NULL;
  	TreeNode *parent;

	unsigned long long int revisionCount;
  	unsigned long long int treeSize;

  	// unsigned long long int pseudoName;
  	// vector<unsigned long long int> reverseConcatName;

private:
  	/**
	 * Helper functions to calculate reverse concat.
	 */
	// void reverseConcat(TreeNode *node);
	// void reverseConcat(TreeNode *node, TreeNode *super);
	/**
	 * Helper function for the constructor to initialize the node.
	 */
	void init(Name &name, TreeNode *parent);

public:
	/**
	 * Constructor for nodes without data.
	 */
	TreeNode(Name &name, TreeNode *parent);
	/**
	 * Constructor for nodes with data.
	 */
	TreeNode(Name &name, Data &data, TreeNode *parent);
	/**
	 * Function to get the name associated with the TreeNode.
	 */
	Name getName();
	/**
	 * Function to get the children of the TreeNode.
	 */
	vector<TreeNode *> getChildren();
	/**
	 * Function to get the map of children.
	 */
	// unordered_map<string, TreeNode*> getChildrenMap();
	/**
	 * Function to mark the TreeNode.
	 */
	bool markNode(bool status);
	/**
	 * Function to check if the TreeNode is marked.
	 */
	bool isNodeMarked();
	/**
	 * Function to change the revision count of the TreeNode.
	 */
	bool updateRevisionCount(unsigned long long int revisionCount);
	/**
	 * Function to get the revision count of the TreeNode.
	 */
	unsigned long long int getRevisionCount();
	/**
	 * Function to check if the current node is a data node.
	 */
	bool isDataNode();	
	/**
	 * Function to check if the current node is a leaf node.
	 */
	bool isLeafNode();	
	/**
	 * Function to free data of the node.
	 */
	bool freeData();	
	/**
	 * Function to get data related to the TreeNode.
	 */
	Data* getData();
	/**
	 * Function to get number of shared prefix with input name.
	 */
	int getNumSharedPrefix(TreeNode *node);
	/**
	 * Function to remove a child from the current TreeNode. Removing a child 
	 * changes the treesize of the current node.
	 */
	bool removeChild (TreeNode * child);
	/**
	 * Function to add a child to the current TreeNode. Adding a child 
	 * changes the treesize of the current node and has an upward spiral
	 * affect, i.e., it changes the size of the upper level TreeNodes as well.
	 * Complexity O(N).
	 */
	bool addChild (TreeNode * child);
	/**
	 * Function to change the tree size rooted at the current TreeNode.
	 * Changing the treesize of the current node and has an upward spiral
	 * affect, i.e., it changes the size of the upper level TreeNodes as well.
	 * Complexity O(N).
	 */
	bool setTreeSize (unsigned long long int treeSize);
	/**
	 * Function to get the tree size rooted at the current TreeNode.
	 */
	unsigned long long int getTreeSize ();
	/**
	 * Function to print tree nodes data for debugging purposes.
	 */
	void printTreeNode();
	/**
	 * Function to print tree nodes data for debugging purposes.
	 */
	void printTreeNodeName();
	/**
	 * Function to get the PseudoName for a node.
	 */
	unsigned long long int getPseudoName();
	/**
	 * Function to set the PseudoName for a node.
	 */
	unsigned long long int setPseudoName();
	/**
	 * Function to get the ReverseConcatName vector for a node.
	 */
	vector<unsigned long long int> getReverseConcatName();	
	/**
	 * Function to get the parent node.
	 */
	TreeNode* getParent();
	/**
	 * Function to compare two tree nodes on the basis of reverseConcatName.
	 * Returns 	1: 	A > B
	 * 			0: 	in case of tie.
	 *			-1: B > A
	 */
	static int compareTreeNodes(TreeNode* A, TreeNode* B);
};

} // namespace infomax
} // namespace ndn

#endif // TREE_NODE_HPP