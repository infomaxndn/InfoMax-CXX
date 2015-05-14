#ifndef INFO_MAX_CONSUMER_HPP
#define INFO_MAX_CONSUMER_HPP

#include <ndn-cxx/face.hpp>

#include <iostream>
#include <vector>
#include <stddef.h>
#include <unordered_set>

#define INFOMAX_INTEREST_TAG "/InfoMax/"
#define INFOMAX_INTEREST_AUX_CHAR "#"

using namespace std;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax {

/**
 * Enumeration for different operation modes for the consumer.
 */
// enum ConsumerModes { STATIC, REALTIME };

/**
 * Type defination for the onData function.
 */
// typedef void (*onDataFunction) (const Interest& interest, Data& data);

/**
 * Type defination for the onTimeout function.
 */
// typedef void (*onTimeoutFunction) (const Interest& interest);

/**
 * Class defination for the InfoMaxConsumer.
 */
class InfoMaxConsumer 
{

public:
	/**
	 * Constructor for the InfoMaxConsumer.
	 */
	 InfoMaxConsumer (Name &prefix);	
	 /**
	 * Destructor for the InfoMaxConsumer.
	 */
	 ~InfoMaxConsumer ();
	/**
	 * Function for expressing InfoMax interest. Adds the list number,
	 * revision number and the InfoMax tag to the interest name.
	 */
	bool get ();

	bool destroy ();

private:
	/**
	 * Function for handling InfoMax prioritized list.
	 */
	static void onList(const Interest& interest, Data& data, vector<string> *names);
	/**
	 * Function for handling timeouts for interests.
	 */
	static void onInfoMaxTimeout(const Interest& interest);
	/**
	 * Function for handling data.
	 */
	static void onData(const Interest& interest, Data& data);
	/**
	 * Function for handling timeouts for data.
	 */
	static void onTimeout(const Interest& interest);
	/**
	 * Helper function to get vector of names from received list.
	 */
	static vector<string> convertInfoMaxList(string list);	
	/**
	 * Function for expressing NDN interest for different elements in the prioritized list.
	 */	 
	bool expressInterest (const Interest &interest);
	/**
	 * Function for expressing InfoMax interest for different elements in the prioritized list.
	 */	 
	bool expressInfoMaxInterest (Interest &interest, unsigned long long int listNumber, unsigned long long int lastKnownRevisionNumber);
	
private:	
	Name prefix;	
	Face face;
	unordered_set<string> receivedElements;
	int windowSize;
	unsigned long long indexNum;
};

}
}

#endif