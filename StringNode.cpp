#include "StringNode.h"

StringNode::StringNode(TreeNode* _parent, std::string _text): TreeNode(_parent), text(_text) {

}

void StringNode::testPrint(int indentationLevel) const {
    for (int i = 0; i < indentationLevel; i++) std::cout << '\t';
    std::cout << text << std::endl;
}