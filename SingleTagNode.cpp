#include "SingleTagNode.h"

SingleTagNode::SingleTagNode(TreeNode* _parent, std::string _name): TreeNode(_parent), name(_name) {

}

void SingleTagNode::testPrint(int indentationLevel) const {
    for (int i = 0; i < indentationLevel; i++) std::cout << '\t';
    std::cout << "<" << name << "/>" << std::endl;
}

void SingleTagNode::toString(std::string& outputString, int indentationLevel) const {
    for (int i = 0; i < indentationLevel; i++) outputString += '\t';
    outputString += '<';
    outputString += name;
    outputString += "/>";
}

std::string SingleTagNode::getNodeType() const {
    return "SingleTagNode";
}