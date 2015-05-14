#ifndef INVALID_PREFIX_EXCEPTION_HPP
#define INVALID_PREFIX_EXCEPTION_HPP 

#include <iostream>
#include <exception>

using namespace std;
// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax {

/**
 * Exception defination for invalid prefix.
 */
class InvalidPrefixException: public exception
{
	virtual const char* what() const throw()
  	{
    	return "Invalid prefix.";
  	}
};
static InvalidPrefixException prefixException;
}
}

#endif