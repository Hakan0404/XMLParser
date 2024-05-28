#include "AttributeNode.h"

AttributeNode::AttributeNode(TreeNode* _parent, std::string _key, std::string _value): TreeNode(_parent), key(_key), value(_value) {

}

std::string AttributeNode::getKey() const {
    return key;
}

std::string AttributeNode::getValue() const {
    return value;
}

void AttributeNode::setValue(std::string newValue) {
    value = newValue;
}

void AttributeNode::testPrint(int redundant) const {
    std::cout << key << "=\"" << value << "\""; 
}

std::string AttributeNode::getNodeType() const {
    return "AttributeNode";
}