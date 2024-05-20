#include "XMLNode.h"
#include "StringNode.h"

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

// UNSAFE; simply adds the pointer, not a copy of the object at the pointer
void XMLNode::addChild(TreeNode* newChild) {
    this->children.push_back(newChild);
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

XMLNode* XMLNode::constructNode(XMLNode* parent, std::string& fileString, int* fileStringIndex) {
    // move the file string index past the initial '<'
    (*fileStringIndex)++;
    
    if (!((fileString[*fileStringIndex] >= 'A' && fileString[*fileStringIndex] <= 'Z') || 
          (fileString[*fileStringIndex] >= 'a' && fileString[*fileStringIndex] <= 'z'))) {
            
        throw std::invalid_argument("Improper formatting. Node name must begin with a letter.");
    }

    // reads the name and attributes (present in the opening tag) of the current Node and constructs the Node from the read string
    std::string nodeConstructorString;
    while (fileString[*fileStringIndex] != '>') {
        nodeConstructorString = nodeConstructorString + fileString[*fileStringIndex];
        (*fileStringIndex)++;
    }
    XMLNode* currentNode = new XMLNode(parent, nodeConstructorString);
    //if (parent != nullptr) parent->addChild(currentNode); // TO REMOVE

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

            // checks whether the hit tag is a closing tag
            if (fileString[*fileStringIndex + 1] == '/') {
                (*fileStringIndex) += 2; // move the file string index into the closing tag
                std::string closingTagName;
                
                while (fileString[*fileStringIndex] != '>') {
                    closingTagName = closingTagName + fileString[*fileStringIndex];
                    (*fileStringIndex)++;
                }
                
                (*fileStringIndex)++; // move the string index past the '>' of the closing tag
                if (currentNode->getName() != closingTagName) throw std::invalid_argument("Improper formatting. Closing tag must match opening tag.");
                break;
            }
            else { // if the hit tag is not a closing one, a new nested node has been hit
                XMLNode* newChild = constructNode(currentNode, fileString, fileStringIndex);
                currentNode->addChild(newChild);
            }
        }
    }

    return currentNode;
    
    /*
    while (true) {
        char currentChar = fileString[*fileStringIndex];

        if (currentChar != '<') {
            std::string textOfStringNode;
        
            while (fileString[*fileStringIndex] != '<') {
                textOfStringNode = textOfStringNode + fileString[*fileStringIndex];
                *fileStringIndex += 1;
            }

        
        }
        else {

        } 
    }
    */
}

XMLNode* XMLNode::constructTree(std::string& fileString) {
    int* fileStringIndex = new int(0);
    XMLNode* root = constructNode(nullptr, fileString, fileStringIndex);
    return root;
}