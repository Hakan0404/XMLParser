#include "StringNode.h"


StringNode::StringNode(TreeNode* _parent, std::string _text): TreeNode(_parent), text(_text) {

}

std::string StringNode::getText() const {
    return text;
}

void StringNode::testPrint(int indentationLevel) const {
    for (int i = 0; i < indentationLevel; i++) std::cout << '\t';
    std::cout << text << std::endl;
}

void StringNode::toString(std::string& outputString, int indentationLevel) const {
    for (int i = 0; i < indentationLevel; i++) outputString.push_back('\t');
    outputString += text;
}

std::string StringNode::getNodeType() const {
    return "StringNode";
}