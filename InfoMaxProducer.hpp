#ifndef INFO_MAX_PRODUCER_HPP
#define INFO_MAX_PRODUCER_HPP

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>

#include "TreeNode.hpp"
#include "InvalidPrefixException.hpp"

#define INFOMAX_INTEREST_TAG "InfoMax"
#define INFOMAX_INTEREST_AUX_CHAR "#"


/**
 * Make the producer modular enough such that the consumer can asked for a prioritized subtree. 
 * Right now the the whole tree would be prioritized and would be offered to the consumer.
 */


using namespace std;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax {

/**
 * Class defination for the InfoMaxProducer.
 */
class InfoMaxProducer 
{

public:
	/**
	 * Constructor for the InfoMaxProducer.	 
	 */
	InfoMaxProducer (Name &prefix);

	/**
	 * Deconstructor for the InfoMaxProducer.
	 */
	~InfoMaxProducer ();

	// InfoMaxProducer (Name &prefix, int listSize);  // Need to be deleted
	// InfoMaxProducer (Name &prefix, int listSize, int whichAlgorithm);  // Need to be deleted
	
	/**
	 * Function for offer element as InfoMax interest. 	 
	 */	
	bool add (Name &postfix, Data& data);

	/**
	 * Function to delete the specified tree branch. 	 
	 */
	bool deletes (Name &postfix);

	/**
	 * Function to call destructor for the InfoMaxProducer.
	 */	
	bool destroy ();	
	

	/**
	 * Function to run the infomax socket.
	 */
	void run();

private:	

 	/**
	 * Function to prioritize the available elements and begin offering.
	 */	
	bool prioritize();

	/**
	 * Function to find similiar data objects to the given name.
	 */
	//bool findSimilarObject(Name objectName, int numObjects);

	/**
	 * Helper function to get priorized vector.
	 */
	vector<TreeNode *> getPrioritizedVector();

	/**
	 * Helper function to get prefix of InfoMaxProducer.
	 */
	 Name getPrefix();	

	/**
	 * This functions checks for the last known revision number and 
	 * the required list number. Based on this information, it sends a vector
	 * with LIST_SIZE Name elements to the user.
	 */
	static  void onInfoMaxInterest (InfoMaxProducer &producer, const Name& name, const Interest& interest);
	/**
	 * This functions calls the appropriate interest handler depending on the presence of the infomax tag.
	 */
	static void onInterestHandler (InfoMaxProducer &producer, const Name& name, const Interest& interest);
	/**
	 * This function sends the data packet corresponding to the interest name to the consumer.
	 */
	static void onInterest (InfoMaxProducer &producer, const Name& name, const Interest& interest);
	/**
	 * This function is called in case of registration failure.
	 */
	static void onRegisterFailed(InfoMaxProducer &producer, const string& reason);
	/**
	 * Helper function to extract requiredListNumber and lastKnownRevision Number from the InfoMaxInterest.
	 */
	static pair <unsigned long long int, unsigned long long int> getRevisionAndListNumber(string auxiliaryInformation);
	/**
	 * Helper function to prepare InfoMax List String.
	 */
	static string* prepareInfoMaxListString(vector<TreeNode *> *nodeVector, unsigned long long int startIdx, unsigned long long int endIdx);	

private:
	Name prefix;
	TreeNode *root;
	Face m_face;
	KeyChain m_keyChain;
	vector<TreeNode *> *prioritizedVector;
	int listSize;
	int whichAlgorithm;
	const RegisteredPrefixId* prefixID;
};

}
}

#endif