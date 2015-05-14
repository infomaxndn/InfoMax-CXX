#include <iostream>
#include <fstream>

#include "InfoMaxConsumer.hpp"

using namespace std;
using namespace ndn;
using namespace ndn::infomax;

int mode=0;

int
main(int argc, char** argv) {
	(void)argc;
	(void)argv;

    string root = argv[1];

	Name nRoot = Name(root);
    InfoMaxConsumer *consumer = new InfoMaxConsumer (nRoot);
    
    while(consumer->get());    
	
	return 0;
}