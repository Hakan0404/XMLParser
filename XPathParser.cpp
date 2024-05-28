#include "XPathParser.h"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

XPathParser::XPathParser(XMLNode* _xmlTree, std::string expressionString) {
    xmlTree = _xmlTree; // TO REPLACE with xmlTree = _xmlTree->clone();
    
    if ((expressionString[0] >= 'a' && expressionString[0] <= 'z') ||
        (expressionString[0] >= 'A' && expressionString[0] <= 'Z') ||
        (expressionString[0] == '@')) {
        expressionString = "//" + expressionString;
    }



    int expressionStringIndex = 0;
    std::vector<std::string> stringCommands;
    std::string currentCommand;

    while (expressionStringIndex < expressionString.length()) {
        if (expressionString[expressionStringIndex] == '/') { // a child or descendant command has been hit
            if (expressionStringIndex < (expressionString.length() - 1) && expressionString[expressionStringIndex + 1] == '/') {
                stringCommands.push_back("//");
                expressionStringIndex += 2;
            }
            else {
                stringCommands.push_back("/");
                expressionStringIndex++;
            }

            continue;
        }
        else if (expressionString[expressionStringIndex] == '[') { // a predicate command has been hit
            while (expressionString[expressionStringIndex] != ']') {
                currentCommand = currentCommand + expressionString[expressionStringIndex];
                expressionStringIndex++;
            }
            currentCommand = currentCommand + expressionString[expressionStringIndex]; // adds the closing ']'
            expressionStringIndex++; // moves the string index to the character after the closing ']'
            stringCommands.push_back(currentCommand);
            currentCommand.clear();
        }
        else {  // a name command has been hit
            while (expressionString[expressionStringIndex] != '/' && 
                   expressionString[expressionStringIndex] != '[' && 
                   expressionStringIndex < expressionString.length()) {
                
                currentCommand = currentCommand + expressionString[expressionStringIndex];
                expressionStringIndex++;
            }
            stringCommands.push_back(currentCommand);
            currentCommand.clear();
        }
    }

    for (int i = 0; i < stringCommands.size(); i++) {
        std::cout << stringCommands[i] << std::endl;
    }
}


std::function<std::vector<XMLNode*>(std::vector<XMLNode*>)> XPathParser::createSelectionCommand(std::string stringCommand) {
    if (stringCommand == "/") {
        return [](std::vector<XMLNode*> inputList)->std::vector<XMLNode*> {
            std::vector<XMLNode*> outputList;
            
            for (int i = 0; i < inputList.size(); i++) {
                std::vector<TreeNode*> currentChildNodes = inputList[i]->getChildren();
                
                for (int j = 0; j < currentChildNodes.size(); j++) {
                    if (currentChildNodes[j]->getNodeType() == "XMLNode") {
                        outputList.push_back(dynamic_cast<XMLNode*>(currentChildNodes[j]));
                    }
                }
            }

            return outputList;
        };
    }

    if (stringCommand == "//") {
        return [](std::vector<XMLNode*> inputList)->std::vector<XMLNode*> {
            std::vector<XMLNode*> outputList;

            for (int i = 0; i < inputList.size(); i++) {
                std::vector<XMLNode*> currentDescendantNodes;
                inputList[i]->getXMLNodes(currentDescendantNodes);
                currentDescendantNodes.erase(currentDescendantNodes.begin() + 0); // getXMLNodes returns a list containing the node calling the method itself and all its descendant XMLNode-s; we only want the descendant nodes here

                for (int j = 0; j < currentDescendantNodes.size(); j++) {
                    if (std::find(outputList.begin(), outputList.end(), currentDescendantNodes[j]) == outputList.end()) { // check that outputList doesn't already contain the node to be added to it
                        outputList.push_back(currentDescendantNodes[j]); 
                    }
                }
            }

            return outputList;
        };
    }

    if (stringCommand[0] == '[') {
        if (stringCommand[stringCommand.length() - 1] != ']') throw std::invalid_argument("Malformed XPath predicate.");

        stringCommand.erase(0, 1); // removes the opening '[' from the command
        stringCommand.erase(stringCommand.length() - 1, 1); // removes the closing ']' from the command
    }
}