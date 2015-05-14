#ifndef PRIORITIZERS_HPP
#define PRIORITIZERS_HPP

#include "TreeNode.hpp"
#include <ndn-cxx/face.hpp>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace infomax {

namespace prioritizers {

vector<TreeNode *>* prioritizeWithAlgorithm(int algoNum, TreeNode *root, vector<TreeNode*>* prioritizedVector);

}
}
}
#endif