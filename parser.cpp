#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    int begin=0;
    int end=0;
    
    string marker1;
    ifstream rFile (argv[1]);
    ofstream wFile (argv[2]);    
    string rFileName = string(argv[1]);
    string wFileName = string(argv[2]);
    string line1, line2;
    begin = rFileName.find("-") + 1;
    end = rFileName.size() - begin - 4;
    marker1 = "Address:";
    marker1.append(rFileName.substr(begin, end));
    marker1.append("-");

    string marker2 = "Length:";    

    if (rFile.is_open())
    {
        while( getline (rFile, line1) )
        {            
            if (line1.find(marker1) != string::npos && line1.find(marker2) != string::npos)
            {
                begin = line1.find(marker1)+marker1.length();
                end = line1.find(marker2) - 1;
                for(int i=begin; i<end; i++)
                {
                    wFile << "/" << line1.at(i);
                }

                getline (rFile, line2);
                wFile << '\t' << line2 << endl;
            } else
            {
                cout << "cannot find markers" << endl;
            }

        }            
        rFile.close();
        wFile.close();
    }
    else
    {
        cout << "Unable to open the file.\n";
    }
}
