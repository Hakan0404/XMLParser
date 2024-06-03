#include "ConsoleInterface.h"
#include "XPathParser.h"
#include "TreeNode.h"
#include "AttributeNode.h"

#include <iostream>
#include <fstream>


void ConsoleInterface::parseCommand(XMLNode*& tree, bool& treeLoaded, std::string commandString, bool& wantsToExit, std::string& currentFilePath) {
    int commandStringIndex = 0;
    
    std::string commandName;
    while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
        commandName.push_back(commandString[commandStringIndex]);
        commandStringIndex++;
    }
    
    if (commandName == "open") {
        std::string filePath;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            filePath.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        open(tree, treeLoaded, filePath, currentFilePath);

        //std::cout << "Parse command tree is null: " << (tree == nullptr) << std::endl;
    }
    else if (commandName == "close") {
        close(tree, treeLoaded);
    }
    else if (commandName == "save") {
        saveas(tree, currentFilePath, treeLoaded);
    }
    else if (commandName == "saveas") {
        std::string filePath;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            filePath.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        saveas(tree, filePath, treeLoaded);
    }
    else if (commandName == "help") {
        help();
    }
    else if (commandName == "exit") {
        exit(tree, treeLoaded, wantsToExit);
    }
    else if (commandName == "print") {
        print(tree, treeLoaded);
    }
    else if (commandName == "select") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        std::string attributeKey;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            attributeKey.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        select(tree, nodeId, attributeKey, treeLoaded);
    }
    else if (commandName == "set") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        std::string attributeKey;
        commandStringIndex++; // move the index past the ' '
        while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
            attributeKey.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        std::string newAttributeValue;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            newAttributeValue.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        set(tree, nodeId, attributeKey, newAttributeValue, treeLoaded);
    }
    else if (commandName == "child") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        std::string positionString;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            positionString.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        child(tree, nodeId, std::stoi(positionString), treeLoaded);
    }
    else if (commandName == "children") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        children(tree, nodeId, treeLoaded);
    }
    else if (commandName == "text") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        text(tree, nodeId, treeLoaded);
    }
    else if (commandName == "delete") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        std::string attributeKey;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            attributeKey.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        deleteAttribute(tree, nodeId, attributeKey, treeLoaded);
    }
    else if (commandName == "newchild") {
        std::string nodeId;
        commandStringIndex++; // move the index past the ' '
        while (commandString[commandStringIndex] != ' ' && commandStringIndex < commandString.length()) {
            nodeId.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        std::string newChildName;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            newChildName.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        newChild(tree, nodeId, newChildName, treeLoaded);
    }
    else if (commandName == "xpath") {
        std::string xPathString;
        commandStringIndex++; // move the index past the ' '
        while (commandStringIndex < commandString.length()) {
            xPathString.push_back(commandString[commandStringIndex]);
            commandStringIndex++;
        }

        xpath(tree, xPathString, treeLoaded);
    }
    else std::cout << "Unrecognized command. Type \"help\" for a list of commands.\n";
}


void ConsoleInterface::open(XMLNode*& tree, bool& treeLoaded, std::string filePath, std::string& currentFilePath) {
    if (treeLoaded) {
        std::cout << "File already open. Close the current file to open a new one.\n";
        return;
    }

    // Debugging
    //std::cout << filePath << std::endl;
    //std::cout << "File path length: " << filePath.length() << std::endl;

    currentFilePath = filePath;
    tree = XMLNode::constructTree(filePath);
    treeLoaded = true;

    std::cout << "Tree loaded\n";
}

void ConsoleInterface::close(XMLNode* tree, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }

    delete tree;
    treeLoaded = false;

    std::cout << "Tree unloaded\n";
}

void ConsoleInterface::saveas(XMLNode* tree, std::string filePath, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }
    
    std::string fileString;
    std::vector<TreeNode*> rootChildren = tree->getChildren();

    for (int i = 0; i < rootChildren.size(); i++) {
        std::string currentString;
        rootChildren[i]->toString(currentString);
        fileString += currentString;
        fileString += "\n";
    }

    std::ofstream out(filePath);
    out << fileString;
    out.close();

    std::cout << "Tree saved\n";
}

void ConsoleInterface::help() {
    std::cout << "List of commands:\n";
    std::cout << "\"open filePath\" | opens the file at the given path\n";
    std::cout << "\"close\" | closes the current file WITHOUT saving it\n";
    std::cout << "\"save\" | saves the changes made to the current file\n";
    std::cout << "\"saveas filePath\" | saves the changes made to the current file in a new file\n";
    std::cout << "\"help\" | displays this list\n";
    std::cout << "\"exit\" | terminates the program\n";
    std::cout << "\"print\" | prints the currently opened XML tree\n";
    std::cout << "\"select nodeId attributeKey\" | prints the attribute with key attributeKey of the node with id nodeId\n";
    std::cout << "\"set nodeId attributeKey newAttributeValue\" | sets the value of the attribute with key attributeKey of the node with id nodeId to newAttributeValue\n";
    std::cout << "\"child nodeId n\" | prints the n-th child of the node with id nodeId\n";
    std::cout << "\"children nodeId\" | prints all attributes of all children of the node with id nodeId\n";
    std::cout << "\"text nodeId\" | prints all text content of the node with id nodeId\n";
    std::cout << "\"delete nodeId attributeKey\" | deletes the attribute with key attributeKey of the node with id nodeId\n";
    std::cout << "\"newchild nodeId newChildName\" | adds a new child with name newChildName to the node with id nodeId\n";
    std::cout << "\"xpath xPathExpression\" | executes the given XPath 2.0 expression\n";
}

void ConsoleInterface::exit(XMLNode* tree, bool& treeLoaded, bool& wantsToExit) {
    if (treeLoaded) delete tree;
    wantsToExit = true;
}

void ConsoleInterface::print(XMLNode* tree, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }

    //std::cout << "Print entered\n";
    //std::cout << "Nullptr: " << (tree == nullptr) << std::endl;

    std::string printString;
    tree->toString(printString);
    std::cout << printString << std::endl;
}

void ConsoleInterface::select(XMLNode* tree, std::string nodeId, std::string attributeKey, bool& treeLoaded) {
    /*std::vector<XMLNode*> xmlDescendants;
    tree->getXMLDescendants(xmlDescendants);

    for (int i = 0; i < xmlDescendants.size(); i++) {
        if (xmlDescendants[i]->hasAttribute("id", nodeId)) {
            if (xmlDescendants[i]->hasAttribute(attributeKey)) {
                std::cout << xmlDescendants[i]->getAttribute(attributeKey)->getValue() << std::endl;
                return;
            } 
        }
    }

    std::cout << "Couldn't find node with id " << nodeId << ", or couldn't find attribute with key " << attributeKey << std::endl;*/

    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }

    std::cout << "Select id and key: " << nodeId << "|" << attributeKey << std::endl;

    std::string xPathString = "//*[@id=\"" + nodeId + "\"]/@" + attributeKey;
    XPathParser parser = XPathParser(tree, xPathString);
    parser.execute();
}

void ConsoleInterface::set(XMLNode* tree, std::string nodeId, std::string attributeKey, std::string newAttributeValue, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }
    
    std::vector<XMLNode*> xmlDescendants;
    tree->getXMLDescendants(xmlDescendants);

    for (int i = 0; i < xmlDescendants.size(); i++) {
        if (xmlDescendants[i]->hasAttribute("id", nodeId)) {
            if (xmlDescendants[i]->hasAttribute(attributeKey)) {
                xmlDescendants[i]->setAttribute(attributeKey, newAttributeValue);
                std::cout << "Attribute set\n";
                return;
            } 
        }
    }

    std::cout << "Couldn't find node with id " << nodeId << ", or couldn't find attribute with key " << attributeKey << std::endl;
}

void ConsoleInterface::child(XMLNode* tree, std::string nodeId, int position, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }
    
    std::vector<XMLNode*> xmlDescendants;
    tree->getXMLDescendants(xmlDescendants);

    for (int i = 0; i < xmlDescendants.size(); i++) {
        if (xmlDescendants[i]->hasAttribute("id", nodeId)) {
            std::vector<TreeNode*> children = xmlDescendants[i]->getChildren();
            if (position >= children.size()) {
                std::cout << "Node with id " << nodeId << " only has " << children.size() << " children\n";
                return;
            }

            std::string printString;
            children[position]->toString(printString);
            std::cout << printString << std::endl;
        }
    }
}

void ConsoleInterface::children(XMLNode* tree, std::string nodeId, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }
    
    std::string xPathString = "//*[@id=\"" + nodeId + "\"]/*/@*";
    XPathParser parser = XPathParser(tree, xPathString);
    parser.execute();
}

void ConsoleInterface::text(XMLNode* tree, std::string nodeId, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }

    std::string xPathString = "//*[@id=\"" + nodeId + "\"]";
    XPathParser parser = XPathParser(tree, xPathString);
    parser.execute();
}

void ConsoleInterface::deleteAttribute(XMLNode* tree, std::string nodeId, std::string attributeKey, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }

    if (attributeKey == "id") throw std::invalid_argument("Cannot delete id attribute");
    
    std::vector<XMLNode*> xmlDescendants;
    tree->getXMLDescendants(xmlDescendants);

    for (int i = 0; i < xmlDescendants.size(); i++) {
        if (xmlDescendants[i]->hasAttribute("id", nodeId)) {
            std::vector<AttributeNode*> attributes = xmlDescendants[i]->getAllAttributes();

            for (int j = 0; j < attributes.size(); j++) {
                if (attributes[j]->getKey() == attributeKey) {
                    xmlDescendants[i]->removeAttribute(attributeKey);
                    std::cout << "Attribute deleted\n";
                    return;
                }
            }
        }
    }

    std::cout << "Couldn't find node with id " << nodeId << ", or couldn't find attribute with key " << attributeKey << std::endl;
}

void ConsoleInterface::newChild(XMLNode* tree, std::string nodeId, std::string newChildName, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }
    
    std::vector<XMLNode*> xmlDescendants;
    tree->getXMLDescendants(xmlDescendants);

    bool suitableIdFound = false;
    int idForNewChild = 0;
    while (!suitableIdFound) {
        bool currentIdUnique = true;
        for (int i = 0; i < xmlDescendants.size(); i++) {
            if (xmlDescendants[i]->hasAttribute("id")) {
                if (xmlDescendants[i]->getAttribute("id")->getValue() == std::to_string(idForNewChild)) {
                    currentIdUnique = false;
                    break;
                }
            }
        }

        if (currentIdUnique) suitableIdFound = true;
        else idForNewChild++;
    }

    std::string xmlNodeConstructorString = newChildName + " id=\"" + std::to_string(idForNewChild) + "\"";

    for (int i = 0; i < xmlDescendants.size(); i++) {
        if (xmlDescendants[i]->hasAttribute("id", nodeId)) {
            xmlDescendants[i]->addChild(new XMLNode(xmlDescendants[i], xmlNodeConstructorString));
            std::cout << "Child added\n";
            return;
        }
    }

    std::cout << "Couldn't find node with id " << nodeId << std::endl;
}

void ConsoleInterface::xpath(XMLNode* tree, std::string xPathString, bool& treeLoaded) {
    if (!treeLoaded) {
        std::cout << "No file opened, call \"open\" command first.\n";
        return;
    }

    XPathParser parser = XPathParser(tree, xPathString);
    parser.execute();
}