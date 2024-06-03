#include "XPathParser.h"
#include "StringNode.h"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

XPathParser::XPathParser(XMLNode* _xmlTree, std::string expressionString) {
    xmlTree = _xmlTree;
    
    if ((expressionString[0] >= 'a' && expressionString[0] <= 'z') ||
        (expressionString[0] >= 'A' && expressionString[0] <= 'Z') ||
        (expressionString[0] == '@' || expressionString[0] == '*')) {
        expressionString = "//" + expressionString;
    }


    int expressionStringIndex = 0;
    std::vector<CommandTriplet> stringCommands;
    std::string childCommand;
    std::string nameCommand;
    std::string predicateCommand;

    while (expressionStringIndex < expressionString.length()) {

        while (expressionString[expressionStringIndex] == '/' && 
               expressionStringIndex < expressionString.length()) { // reads all backslashes
            
            childCommand = childCommand + '/';
            expressionStringIndex++;
        }

        while ((expressionString[expressionStringIndex] != '/' && expressionString[expressionStringIndex] != '[') && 
                expressionStringIndex < expressionString.length()) { // reads the name command
            
            nameCommand.push_back(expressionString[expressionStringIndex]);
            expressionStringIndex++;
        }

        if (expressionString[expressionStringIndex] == '[') {
            expressionStringIndex++; // moves the string index to the character after the opening '[' of the predicate
            while (expressionString[expressionStringIndex] != ']' &&
                   expressionStringIndex < expressionString.size()) {

                predicateCommand.push_back(expressionString[expressionStringIndex]);
                expressionStringIndex++;
            }
            expressionStringIndex++; // moves the string index past the closing ']' of the predicate
        }


        // Debugging
        //std::cout << "Commands:\n";
        //std::cout << childCommand << nameCommand << "[" << predicateCommand << "]" << std::endl;

        stringCommands.push_back(CommandTriplet(childCommand, nameCommand, predicateCommand));

        childCommand.clear();
        nameCommand.clear();
        predicateCommand.clear();
    }

    // Error checking
    for (int i = 0; i < stringCommands.size(); i++) {
        if ((stringCommands[i].nameCommand[0] == '@') && (i != stringCommands.size() - 1)) {
            throw std::invalid_argument("Attribute nodes do not have children to select.");
        }
        if ((stringCommands[i].nameCommand[0] == '@') && (stringCommands[i].predicateCommand != "")) {
            throw std::invalid_argument("Attribute nodes cannot have predicates applied to them.");
        }
        if (stringCommands[i].childCommand == "") throw std::invalid_argument("Child selection command cannot be empty.");
        if (stringCommands[i].nameCommand == "") throw std::invalid_argument("Name selection command cannot be empty.");
    }

    // sets the commands after error checking
    commands = stringCommands;

    // Adds the initial context for selection, i.e. the root of the XML tree
    currentlySelectedNodes.push_back(xmlTree);
}

bool XPathParser::predicateCheck_comparison_attribute(XMLNode* nodeToCheck, std::string attributeName, std::string comparisonOperator, std::string comparisonValue) {
    std::vector<AttributeNode*> attributes = nodeToCheck->getAllAttributes();
            
    if (comparisonOperator == "!=") {
        bool noMatchingAttributesFound = true;

        for (int i = 0; i < attributes.size(); i++) {
            if (attributes[i]->getKey() == attributeName || attributeName == "*") {
                if (attributes[i]->getValue() == comparisonValue) noMatchingAttributesFound = false;
            }
        }

        if (noMatchingAttributesFound) return true;
    }


    for (int i = 0; i < attributes.size(); i++) {
        if (attributes[i]->getKey() == attributeName || attributeName == "*") {
            if (comparisonOperator == ">=" && attributes[i]->getValue() >= comparisonValue) return true;
            else if (comparisonOperator == "<=" && attributes[i]->getValue() <= comparisonValue) return true;
            else if (comparisonOperator == "<" && attributes[i]->getValue() < comparisonValue) return true;
            else if (comparisonOperator == ">" && attributes[i]->getValue() > comparisonValue) return true;
            else if (comparisonOperator == "=" && attributes[i]->getValue() == comparisonValue) return true;
        }
    }

    return false;
}

bool XPathParser::predicateCheck_comparison_node(XMLNode* nodeToCheck, std::string variableName, std::string comparisonOperator, std::string comparisonValue) {
    std::vector<XMLNode*> xmlChildren = nodeToCheck->getXMLChildren();
    std::vector<XMLNode*> matchingNameChildren;
    for (int i = 0; i < xmlChildren.size(); i++) {
        if (xmlChildren[i]->getName() == variableName || variableName == "*") matchingNameChildren.push_back(xmlChildren[i]);
    }
    
    for (int i = 0; i < matchingNameChildren.size(); i++) {
        std::vector<StringNode*> stringChildren = matchingNameChildren[i]->getStringChildren();

        if (comparisonOperator == "!=") {
            bool noMatchingStringNodesFound = true;

            for (int j = 0; j < stringChildren.size(); j++) {
                if (stringChildren[j]->getText() == comparisonValue) noMatchingStringNodesFound = false;
            }

            if (noMatchingStringNodesFound) return true;
        }

        for (int j = 0; j < stringChildren.size(); j++) {
            if (comparisonOperator == ">=" && stringChildren[j]->getText() >= comparisonValue) return true;
            else if (comparisonOperator == "<=" && stringChildren[j]->getText() <= comparisonValue) return true;
            else if (comparisonOperator == "<" && stringChildren[j]->getText() < comparisonValue) return true;
            else if (comparisonOperator == ">" && stringChildren[j]->getText() > comparisonValue) return true;
            else if (comparisonOperator == "=" && stringChildren[j]->getText() == comparisonValue) return true;
        }
    }

    return false;
}

bool XPathParser::predicateCheck_existance_attribute(XMLNode* nodeToCheck, std::string attributeName) {
    std::vector<AttributeNode*> attributes = nodeToCheck->getAllAttributes();

    for (int i = 0; i < attributes.size(); i++) {
        if (attributes[i]->getKey() == attributeName || attributeName == "*") return true;
    }

    return false;
}

bool XPathParser::predicateCheck_existance_node(XMLNode* nodeToCheck, std::string variableName) {
    std::vector<XMLNode*> xmlChildren = nodeToCheck->getXMLChildren();

    for (int i = 0; i < xmlChildren.size(); i++) {
        if (xmlChildren[i]->getName() == variableName || variableName == "*") return true;
    }

    return false;
}

bool XPathParser::predicateCheck(XMLNode* nodeToCheck, std::string stringPredicate) {
    if (stringPredicate == "") return true;

    bool isAttributePredicate = (stringPredicate[0] == '@');

    // read the name of the variable being checked
    std::string variableName;
    int stringPredicateIndex = 0;
    while (stringPredicate[stringPredicateIndex] != '>' && stringPredicate[stringPredicateIndex] != '<' &&
           stringPredicate[stringPredicateIndex] != '!' && stringPredicate[stringPredicateIndex] != '=' &&
           stringPredicateIndex < stringPredicate.length()) {

        variableName = variableName + stringPredicate[stringPredicateIndex];
        stringPredicateIndex++;
    }

    // if the variable is an attribute, delete its initial '@'
    if (isAttributePredicate) variableName.erase(0, 1);

    // comparison predicate
    if (stringPredicate.contains(">=") || stringPredicate.contains("<=") || stringPredicate.contains("!=") ||
        stringPredicate.contains("<") || stringPredicate.contains(">") || stringPredicate.contains("=")) {

        // search the stringPredicate and construct a string containing the comparison operator
        std::string comparisonOperator;        
        while (stringPredicate[stringPredicateIndex] != '\"') { 
            comparisonOperator = comparisonOperator + stringPredicate[stringPredicateIndex];
            stringPredicateIndex++; 
        }

        // search the stringPredicate and construct a string containing the value being compared to (the one on the right)
        std::string comparisonValue;
        stringPredicateIndex++; // move index past the opening quotation mark (")
        while (stringPredicate[stringPredicateIndex] != '\"') { // read the comparison value
            comparisonValue = comparisonValue + stringPredicate[stringPredicateIndex];
            stringPredicateIndex++;
        }

        if (isAttributePredicate) { // attribute predicate
            return predicateCheck_comparison_attribute(nodeToCheck, variableName, comparisonOperator, comparisonValue);
        }
        else { // node name predicate
            return predicateCheck_comparison_node(nodeToCheck, variableName, comparisonOperator, comparisonValue);
        }
    }

    // existance predicate
    else {
        if (isAttributePredicate) { // has attribute with the specified name
            return predicateCheck_existance_attribute(nodeToCheck, variableName);
        }

        else { // has child node with the specified name
            return predicateCheck_existance_node(nodeToCheck, variableName);
        }
    }
}


std::vector<TreeNode*> XPathParser::selectionCommand_attribute_children(std::vector<TreeNode*> inputList, std::string attributeName) {
    std::vector<TreeNode*> outputList;

    for (int i = 0; i < inputList.size(); i++) {
        if (inputList[i]->getNodeType() != "XMLNode") throw std::invalid_argument("Tried to perform child selection on non-XMLNode."); // validity check

        XMLNode* currentXMLNode = dynamic_cast<XMLNode*>(inputList[i]);
        std::vector<AttributeNode*> currentAttributes = currentXMLNode->getAllAttributes();
        for (int j = 0; j < currentAttributes.size(); j++) {
            if (currentAttributes[j]->getKey() == attributeName || attributeName == "*") outputList.push_back(currentAttributes[j]);
        }
    }

    return outputList;
}

std::vector<TreeNode*> XPathParser::selectionCommand_attribute_descendants(std::vector<TreeNode*> inputList, std::string attributeName) {
    std::vector<TreeNode*> outputList;
    std::vector<AttributeNode*> outputListAttributes;

    for (int i = 0; i < inputList.size(); i++) {
        if (inputList[i]->getNodeType() != "XMLNode") throw std::invalid_argument("Tried to perform child selection on non-XMLNode."); // validity check

        XMLNode* currentXMLNode = dynamic_cast<XMLNode*>(inputList[i]);
        std::vector<XMLNode*> currentDescendantNodes;
        currentXMLNode->getXMLDescendants(currentDescendantNodes);
        
        for (int j = 0; j < currentDescendantNodes.size(); j++) {
            std::vector<AttributeNode*> currentAttributes = currentDescendantNodes[j]->getAllAttributes();

            for (int k = 0; k < currentAttributes.size(); k++) {
                if ((currentAttributes[k]->getKey() == attributeName || attributeName == "*") &&
                     std::find(outputListAttributes.begin(), outputListAttributes.end(), currentAttributes[k]) == outputListAttributes.end()) { // checks if the outputList does NOT contain the current attribute

                    outputListAttributes.push_back(currentAttributes[k]);
                }
            }
        }
    }

    for (int i = 0; i < outputListAttributes.size(); i++) {
        outputList.push_back(outputListAttributes[i]);
    }

    return outputList;
}

std::vector<TreeNode*> XPathParser::selectionCommand_node_children_position(std::vector<TreeNode*> inputList, std::string nodeName, int nodePosition) {
    std::vector<TreeNode*> outputList;

    for (int i = 0; i < inputList.size(); i++) {
        if (inputList[i]->getNodeType() != "XMLNode") throw std::invalid_argument("Tried to perform child selection on non-XMLNode."); // validity check

        XMLNode* currentXMLNode = dynamic_cast<XMLNode*>(inputList[i]);
        std::vector<XMLNode*> xmlChildren = currentXMLNode->getXMLChildren();
        std::vector<XMLNode*> matchingNameChildren;
        for (int j = 0; j < xmlChildren.size(); j++) {
            if (xmlChildren[j]->getName() == nodeName || nodeName == "*") matchingNameChildren.push_back(xmlChildren[j]);
        }

        if (nodePosition < matchingNameChildren.size()) outputList.push_back(matchingNameChildren[nodePosition]);
    }

    return outputList;
}

std::vector<TreeNode*> XPathParser::selectionCommand_node_descendants_position(std::vector<TreeNode*> inputList, std::string nodeName, int nodePosition) {
    std::vector<TreeNode*> outputList;

    for (int i = 0; i < inputList.size(); i++) {
        if (inputList[i]->getNodeType() != "XMLNode") throw std::invalid_argument("Tried to perform child selection on non-XMLNode."); // validity check

        XMLNode* currentXMLNode = dynamic_cast<XMLNode*>(inputList[i]);
        
        std::vector<XMLNode*> xmlDescendants;
        currentXMLNode->getXMLDescendants(xmlDescendants);
        xmlDescendants.erase(xmlDescendants.begin() + 0); // removes the first node, since getXMLDescendants() returns all XML descendants of the node it is called on AND the node itself

        std::vector<XMLNode*> matchingNameDescendants;
        for (int j = 0; j < xmlDescendants.size(); j++) {
            if ((xmlDescendants[j]->getName() == nodeName || nodeName == "*")) { //&&
                 //std::find(matchingNameDescendants.begin(), matchingNameDescendants.end(), xmlDescendants[j]) == matchingNameDescendants.end()) { // checks if matchingNameDescendants does NOT contain the current xmlDescendants[j]

                matchingNameDescendants.push_back(xmlDescendants[j]);
            }
        }

        if (nodePosition < matchingNameDescendants.size() && 
            std::find(outputList.begin(), outputList.end(), matchingNameDescendants[nodePosition]) == outputList.end()) { // checks if outputList does NOT contain the current matching name descendant at nodePosition
            
            outputList.push_back(matchingNameDescendants[nodePosition]);
        }
    }

    return outputList;
}

std::vector<TreeNode*> XPathParser::selectionCommand_node_children_predicate(std::vector<TreeNode*> inputList, std::string nodeName, std::string predicate) {
    std::vector<TreeNode*> outputList;

    for (int i = 0; i < inputList.size(); i++) {
        if (inputList[i]->getNodeType() != "XMLNode") throw std::invalid_argument("Tried to perform child selection on non-XMLNode."); // validity check

        XMLNode* currentXMLNode = dynamic_cast<XMLNode*>(inputList[i]);
        std::vector<XMLNode*> xmlChildren = currentXMLNode->getXMLChildren();
        
        for (int j = 0; j < xmlChildren.size(); j++) {
            if ((xmlChildren[j]->getName() == nodeName || nodeName == "*") && predicateCheck(xmlChildren[j], predicate)) {
                outputList.push_back(xmlChildren[j]);
            }
        }
    }

    return outputList;
}

std::vector<TreeNode*> XPathParser::selectionCommand_node_descendants_predicate(std::vector<TreeNode*> inputList, std::string nodeName, std::string predicate) {
    std::vector<TreeNode*> outputList;

    for (int i = 0; i < inputList.size(); i++) {
        if (inputList[i]->getNodeType() != "XMLNode") throw std::invalid_argument("Tried to perform child selection on non-XMLNode."); // validity check

        XMLNode* currentXMLNode = dynamic_cast<XMLNode*>(inputList[i]);

        std::vector<XMLNode*> xmlDescendants;
        currentXMLNode->getXMLDescendants(xmlDescendants);
        xmlDescendants.erase(xmlDescendants.begin() + 0); // removes the first node, since getXMLDescendants() returns all XML descendants of the node it is called on AND the node itself

        for (int j = 0; j < xmlDescendants.size(); j++) {
            if ((xmlDescendants[j]->getName() == nodeName || nodeName == "*") && predicateCheck(xmlDescendants[j], predicate) &&
                 std::find(outputList.begin(), outputList.end(), xmlDescendants[j]) == outputList.end()) { // checks if outputList does NOT contain the current xmlDescendants[j]
                
                outputList.push_back(xmlDescendants[j]);
            }
        }
    }

    return outputList;
}

std::vector<TreeNode*> XPathParser::selectionCommand(std::vector<TreeNode*> inputList, CommandTriplet commandTriplet) {
    if (commandTriplet.nameCommand[0] == '@') {
        std::string attributeName = commandTriplet.nameCommand;
        attributeName.erase(0, 1); // erase the '@' at the beginning of the attribute

        if (commandTriplet.childCommand == "/") {
            return selectionCommand_attribute_children(inputList, attributeName);
        }
        else if (commandTriplet.childCommand == "//") {
            return selectionCommand_attribute_descendants(inputList, attributeName);
        }
        else throw std::invalid_argument("Unrecognized child selection command. Must be either / or //.");
    }

    else {
        bool predicateIsNumber = true;
        if (commandTriplet.predicateCommand == "") predicateIsNumber = false;
        for (int i = 0; i < commandTriplet.predicateCommand.length(); i++) {
            if (commandTriplet.predicateCommand[i] < '0' || commandTriplet.predicateCommand[i] > '9') predicateIsNumber = false;
        }

        std::string nodeName = commandTriplet.nameCommand;

        if (predicateIsNumber) {
            int nodePosition = std::stoi(commandTriplet.predicateCommand);

            if (commandTriplet.childCommand == "/") return selectionCommand_node_children_position(inputList, nodeName, nodePosition);
            else if (commandTriplet.childCommand == "//") return selectionCommand_node_descendants_position(inputList, nodeName, nodePosition);
            else throw std::invalid_argument("Unrecognized child selection command. Must be either / or //.");
        }
        else {
            if (commandTriplet.childCommand == "/") return selectionCommand_node_children_predicate(inputList, nodeName, commandTriplet.predicateCommand);
            else if (commandTriplet.childCommand == "//") return selectionCommand_node_descendants_predicate(inputList, nodeName, commandTriplet.predicateCommand);
            else throw std::invalid_argument("Unrecognized child selection command. Must be either / or //.");
        }
    }
}


XPathParser::CommandTriplet::CommandTriplet(std::string _childCommand, std::string _nameCommand, std::string _predicateCommand):
childCommand(_childCommand), nameCommand(_nameCommand), predicateCommand(_predicateCommand) {

}

void XPathParser::execute() {
    for (int i = 0; i < commands.size(); i++) {
        currentlySelectedNodes = selectionCommand(currentlySelectedNodes, commands[i]);
    }

    // prints from the first to the second to last element
    std::cout << "{";
    for (int i = 0; i < (int)(currentlySelectedNodes.size()) - 1; i++) {
        if (currentlySelectedNodes[i]->getNodeType() == "AttributeNode") {
            AttributeNode* currentNode = dynamic_cast<AttributeNode*>(currentlySelectedNodes[i]);
            std::cout << currentNode->getKey() << "=\"" << currentNode->getValue() << "\", ";
        }
        else if (currentlySelectedNodes[i]->getNodeType() == "XMLNode") {
            XMLNode* currentNode = dynamic_cast<XMLNode*>(currentlySelectedNodes[i]);
            std::vector<StringNode*> stringChildren = currentNode->getStringChildren();


            std::cout << "{";
            for (int j = 0; j < (int)(stringChildren.size()) - 1; j++) {
                std::cout << stringChildren[j]->getText() << ", ";
            }

            if (stringChildren.size() > 0) std::cout << stringChildren[(int)(stringChildren.size()) - 1]->getText();
            std::cout << "}, ";
        }
        else throw std::logic_error("Invalid nodes selected. Only AttributeNode and XMLNode objects may be part of the final selection."); 
    }

    // prints the last element
    if (currentlySelectedNodes.size() > 0) {
        if (currentlySelectedNodes[(int)(currentlySelectedNodes.size()) - 1]->getNodeType() == "AttributeNode") {
            AttributeNode* currentNode = dynamic_cast<AttributeNode*>(currentlySelectedNodes[(int)(currentlySelectedNodes.size()) - 1]);
            std::cout << currentNode->getKey() << "=\"" << currentNode->getValue() << "\"";
        }
        else if (currentlySelectedNodes[(int)(currentlySelectedNodes.size()) - 1]->getNodeType() == "XMLNode"){
            XMLNode* currentNode = dynamic_cast<XMLNode*>(currentlySelectedNodes[(int)(currentlySelectedNodes.size()) - 1]);
            std::vector<StringNode*> stringChildren = currentNode->getStringChildren();

            std::cout << "{";
            for (int j = 0; j < (int)(stringChildren.size()) - 1; j++) {
                std::cout << stringChildren[j]->getText() << ", ";
            }
            if (stringChildren.size() > 0) std::cout << stringChildren[stringChildren.size() - 1]->getText();
            std::cout << "}";
        }
        else throw std::logic_error("Invalid nodes selected. Only AttributeNode and XMLNode objects may be part of the final selection."); 
    }
    
    std::cout << "}\n";

    // resets the context, allowing for repeated execution
    currentlySelectedNodes.clear();
    currentlySelectedNodes.push_back(xmlTree);

    //std::cout << "End of execute() reached.\n";
}