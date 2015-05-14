#include <iostream>
#include <fstream>
#include <string>
#include "InfoMaxProducer.hpp"

using namespace std;
using namespace ndn;
using namespace ndn::infomax;

bool
readFileintoString(string fileName, string *content)
{
	string line;
	ifstream readFile (fileName);
	
	if (readFile.is_open())
	{
		while (getline (readFile, line))
		{
			*content += line;
		}
		readFile.close();
	}

	return true;
}

bool
setContentFromFile(Data *data, string fileName, unsigned long long int freshnessPeriod)
{
	data->setFreshnessPeriod(time::seconds(freshnessPeriod));
	string content;	

	readFileintoString(fileName, &content);
	data->setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.size());
	return true;
}

bool
readFileintoNameAndData(string fileName, vector<Name> *nameList, vector<Data> *dataList, unsigned long long int freshnessPeriod)
{
	string line;
	string marker = "\t";
	string name, content;
	int markerAt=0;
	ifstream readFile (fileName);
	
	if (readFile.is_open())
	{
		while (getline (readFile, line))
		{
			if (line.find(marker) != string::npos)
            {            	            	
            	markerAt = line.find(marker);
                name = line.substr(0, markerAt);

                Name temp = Name(name);
                nameList->push_back(temp);
           	    content = line.substr(markerAt+1, line.length() - markerAt);    

           	    Data data = Data(Name("/Data"));
           	    data.setFreshnessPeriod(time::seconds(freshnessPeriod));
           	    data.setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.size());
				dataList->push_back(data);					
            } else
            {
                cout << "cannot find markers" << endl;
            }
		}		
	}else
    {
        cout << "Unable to open the file.\n";
    }
    readFile.close();

	return true;
}

int
main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	Name nRoot = Name(argv[1]);
	
	InfoMaxProducer *producer = new InfoMaxProducer(nRoot);	

	vector<Name> nameList;
	vector<Data> dataList;

	readFileintoNameAndData(argv[2], &nameList, &dataList, 10);
	for(unsigned int i=0; i<nameList.size(); i++) {	
		producer->add(nameList[i], dataList[i]);
	}

	// Name del = Name("a");
	// producer->deletes(del);
	// producer->prioritize();    	
	producer->run();    	
    
	return 0;
}