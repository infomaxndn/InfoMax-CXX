#include <iostream>
#include <unordered_map>
#include "TreeNode.hpp"
#include <string>

using namespace std;
using namespace std;
using namespace ndn;
using namespace ndn::infomax;

int
main(int argc, char** argv) {
	// Ignore parameters
	(void)argc;
	(void)argv;

	// Names to be used for testing.
	ndn::Name nuiuc = ndn::Name("/uiuc");
	ndn::Name nsc = ndn::Name("/uiuc/sc");
	ndn::Name nece = ndn::Name("/uiuc/ece");
	ndn::Name nsc1 = ndn::Name("/uiuc/sc/1");
	ndn::Name nsc2 = ndn::Name("/uiuc/sc/2");
	ndn::Name ndata = ndn::Name("/data");

	// NULL casted as a TreeNode ptr
	ndn::infomax::TreeNode *nullPtr = NULL;

	// Tests the constructor without 
	ndn::infomax::TreeNode root(nuiuc, nullPtr);

	cout << "Printing root:" << endl;
	root.printTreeNode();
	cout << "END root:" << endl << endl;

	ndn::Data daa;
	daa.setName(ndata);
	ndn::infomax::TreeNode sc(nsc, &root);
	ndn::infomax::TreeNode ece(nece, daa, &root);
	ndn::infomax::TreeNode sc1(nsc1, &sc);
	ndn::infomax::TreeNode sc2(nsc2, &sc);

	root.addChild(&sc);
	root.addChild(&ece);
	sc.addChild(&sc1);
	sc.addChild(&sc2);


	unordered_map<string, TreeNode*> map = root.getChildrenMap();
	for(auto it = map.begin(); it != map.end(); ++it ){
		std::cout << " " << it->first << ":";
		TreeNode *n = it->second;
		cout <<n->getName().toUri() <<endl;
		if(n->getData()){
    		cout<<endl<<"Data: "<<n->getData()->getName().toUri()<<endl;
    	}
	}
	cout<<endl;


	root.markNode(true);
	root.updateRevisionCount(200ull);
	if(root.isNodeMarked() ==false || root.getRevisionCount() != 200ull || root.isDataNode()==true || ece.isDataNode()==false){
		cout<<"EEEEERRRRRR";
	}	

	cout << "Printing root:" << endl;
	root.printTreeNode();
	cout << "END root:" << endl << endl;



	return 0;
}