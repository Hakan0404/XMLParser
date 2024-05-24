#include "XMLNode.h"
#include "StringNode.h"
#include "SingleTagNode.h"

#include <fstream>

XMLNode::XMLAttribute::XMLAttribute(std::string _key, std::string _value): key(_key), value(_value) {

}


XMLNode::XMLNode(XMLNode* _parent, std::string nameAttributeString): TreeNode(_parent) {
    std::vector<std::string> substrings;

    std::string currentString;
    for (int i = 0; i < nameAttributeString.size(); i++) {
        if (nameAttributeString[i] != ' ') {
            currentString = currentString + nameAttributeString[i];
        }
        else {
            substrings.push_back(currentString);
            currentString.clear();
        }
    }
    substrings.push_back(currentString);

    this->name = substrings[0];
    std::string currentAttributeName;
    std::string currentAttributeValue;
    for (int i = 1; i < substrings.size(); i++) {
        int equalityIndex = substrings[i].find('=');
        currentAttributeName = substrings[i].substr(0, equalityIndex);
        currentAttributeValue = substrings[i].substr(equalityIndex + 2, substrings[i].length() - equalityIndex - 3);
        this->attributes.push_back(XMLAttribute(currentAttributeName, currentAttributeValue));
    }
}


// creates a list containing the XMLNode it is called on and all its descendant XMLNodes 
void XMLNode::getXMLNodes(std::vector<XMLNode*>& xmlNodeList) {
    xmlNodeList.push_back(this);
    for (int i = 0; i < this->children.size(); i++) {
        if (this->children[i]->getNodeType() == "XMLNode") {
            dynamic_cast<XMLNode*>(children[i])->getXMLNodes(xmlNodeList);
        }
    }
}


// Iterates through "this" node and all its descendant XML nodes and gives each one a unique "id" attribute
void XMLNode::setUniqueIds() {
    std::vector<XMLNode*> xmlNodeList;
    this->getXMLNodes(xmlNodeList);

    // generates new ids for nodes without the "id" attribute
    int idCounter = 0;
    for (int i = 0; i < xmlNodeList.size(); i++) {
        if (!(xmlNodeList[i]->hasAttribute("id"))) {
            xmlNodeList[i]->addAttribute("id", std::to_string(idCounter));
            idCounter++;
        }
    }

    // finds and fixes cases of duplicate ids
    for (int i = 0; i < xmlNodeList.size(); i++) {
        if (xmlNodeList[i]->hasAttribute("id")) {
            std::string currentIdValue = (xmlNodeList[i]->getAttribute("id")).value;
            int duplicatesCounter = 1; // counts the id of the first node too

            for (int j = 0; j < xmlNodeList.size(); j++) {
                if (j != i) { // so that we don't count the original node as a duplicate of itself
                    if (xmlNodeList[j]->hasAttribute("id", currentIdValue)) { // a node with the same value for the "id" attribute has been found
                        duplicatesCounter++;
                        std::string newIdValue = (xmlNodeList[j]->getAttribute("id")).value + "_" + std::to_string(duplicatesCounter);
                        xmlNodeList[j]->setAttribute("id", newIdValue);
                    }
                }
            }

            if (duplicatesCounter > 1) { // if nodes with duplicate "id" attributes have been hit, then change the value of the original node
                xmlNodeList[i]->setAttribute("id", currentIdValue + "_1");
            }
        }
    }
}


// UNSAFE; simply adds the pointer, not a copy of the object at the pointer
void XMLNode::addChild(TreeNode* newChild) {
    this->children.push_back(newChild);
} 


void XMLNode::addAttribute(std::string attributeName, std::string attributeValue) {
    attributes.push_back(XMLAttribute(attributeName, attributeValue));
}


void XMLNode::setAttribute(std::string attributeName, std::string newAttributeValue) {
    if (!(this->hasAttribute(attributeName))) {
        std::string errorMessage = "No attribute with name " + attributeName + " found within node with name " + this->name + ".";
        throw std::invalid_argument(errorMessage);
    }

    for (int i = 0; i < attributes.size(); i++) {
        if (attributes[i].key == attributeName) {
            attributes[i].value = newAttributeValue;
            break;
        }
    }
}


XMLNode::XMLAttribute XMLNode::getAttribute(std::string attributeName) const {
    for (int i = 0; i < attributes.size(); i++) {
        if (attributes[i].key == attributeName) return attributes[i];
    }

    std::string errorMessage = "No attribute with name " + attributeName + " found within node with name " + this->name + ".";
    throw std::invalid_argument(errorMessage);
}


XMLNode::XMLAttribute XMLNode::getAttribute(int attributeIndex) const {
    if (attributeIndex >= attributes.size()) {
        std::string errorMessage = "Attribute index " + std::to_string(attributeIndex) + " for node with name " + this->name + " is out of bounds.";
        throw std::invalid_argument(errorMessage);
    }
    return attributes[attributeIndex];
}


bool XMLNode::hasAttribute(std::string attributeName) const {
    for (int i = 0; i < attributes.size(); i++) {
        if (attributes[i].key == attributeName) return true;
    }

    return false;
}


bool XMLNode::hasAttribute(std::string attributeName, std::string attributeValue) const {
    for (int i = 0; i < attributes.size(); i++) {
        if (attributes[i].key == attributeName && attributes[i].value == attributeValue) return true;
    }

    return false;
}


std::string XMLNode::getName() const {
    return name;
}


void XMLNode::testPrint(int indentationLevel) const {
    for (int i = 0; i < indentationLevel; i++) std::cout << '\t'; //prints indentiation before opening tag
    
    // prints name
    std::cout << '<' << name;
    //prints attributes
    for (int i = 0; i < attributes.size(); i++) {
        std::cout << ' ' << attributes[i].key << "=\"" << attributes[i].value << '\"';
    }
    std::cout << ">\n";

    // recursive call to print the children
    for (int i = 0; i < children.size(); i++) {
        children[i]->testPrint(indentationLevel + 1);
    }

    for (int i = 0; i < indentationLevel; i++) std::cout << '\t'; //prints indentiation before closing tag
    // prints closing tag
    std::cout << "</" << name << ">\n";
}


std::vector<TreeNode*> XMLNode::getChildren() const {
    return children;
}


std::string XMLNode::convertFileToString(const std::string filePath) {
    std::ifstream inputStream(filePath);

    std::string fileText;
    std::string current;

    // reads all of the file's contents into the fileText string, while removing the new lines
    while (!inputStream.eof()) {
        getline(inputStream, current);
        fileText = fileText + current;
    }

    // removes all indentation from the file string
    while (fileText.contains("\t")) {
        int tabIndex = fileText.find("\t");
        fileText.erase(tabIndex, 1);
    }

    // add the encompassing root node
    fileText = "<root>" + fileText + "</root>";

    return fileText;
}


XMLNode* XMLNode::constructNode(XMLNode* parent, std::string& fileString, int* fileStringIndex) {
    // move the file string index past the initial '<'
    (*fileStringIndex)++;
    
    if (!((fileString[*fileStringIndex] >= 'A' && fileString[*fileStringIndex] <= 'Z') || 
          (fileString[*fileStringIndex] >= 'a' && fileString[*fileStringIndex] <= 'z'))) {
            
        std::cout << fileString[*fileStringIndex] << std::endl;
        throw std::invalid_argument("Improper formatting. Node name must begin with a letter.");
    }

    // reads the name and attributes (present in the opening tag) of the current Node and constructs the Node from the read string
    std::string nodeConstructorString;
    while (fileString[*fileStringIndex] != '>') {
        nodeConstructorString = nodeConstructorString + fileString[*fileStringIndex];
        (*fileStringIndex)++;
    }
    XMLNode* currentNode = new XMLNode(parent, nodeConstructorString);

    // Debugging
    //std::cout << "Running for: " << currentNode->getName() << std::endl;

    // moves the string index to the first character after the opening tag
    (*fileStringIndex)++;

    std::string currentString; // the internal text of the current child string node
    while (true) {
        if (fileString[*fileStringIndex] != '<') {
            currentString = currentString + fileString[*fileStringIndex];
            (*fileStringIndex)++;
        }
        else { // the start of a tag has been hit
            // checks if the text string read so far is empty, and therefore worth adding as a child
            if (!currentString.empty()) {
                currentNode->addChild(new StringNode(currentNode, currentString));
                currentString.clear();
            }


            int startOfTagIndex = *fileStringIndex; // remembers the start of the current tag
            (*fileStringIndex)++; // moves the file string index onto the first character of the tag
            std::string tagString;
            // reads the tag's contents into the string
            while (fileString[*fileStringIndex] != '>') {
                tagString = tagString + fileString[*fileStringIndex];
                (*fileStringIndex)++;
            }
            (*fileStringIndex)++; // moves the file string index onto the character after the '>' of the tag

            // Debugging
            //std::cout << tagString << std::endl;

            if (tagString[0] == '/') { // if the first character of the closing tag is a '/', then the closing tag has been hit
                if (currentNode->getName() != tagString.substr(1)) throw std::invalid_argument("Improper formatting. Closing tag must match opening tag.");
                break;
            }
            else if (tagString[tagString.length() - 1] == '/') { // if the last character of the closing tag is a '/', then a SingleTagNode has been hit
                std::string singleTagName = tagString;
                singleTagName.pop_back();
                currentNode->addChild(new SingleTagNode(currentNode, singleTagName));
            }
            else { // a new nested node has been hit
                (*fileStringIndex) = startOfTagIndex;
                XMLNode* newChild = constructNode(currentNode, fileString, fileStringIndex);
                currentNode->addChild(newChild);
            }
        }
    }

    return currentNode;
}

XMLNode* XMLNode::constructTree(const std::string& filePath) {
    std::string fileString = convertFileToString(filePath);
    int* fileStringIndex = new int(0);
    
    XMLNode* root = constructNode(nullptr, fileString, fileStringIndex);
    delete fileStringIndex;

    root->setUniqueIds();
    
    return root;
}

std::string XMLNode::getNodeType() const {
    return "XMLNode";
}