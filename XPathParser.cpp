#include "XPathParser.h"
#include <string>
#include <vector>
#include <functional>

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


/*
class SelectionCommand {
    public:
        virtual void execute(std::vector<XMLNode*> contextNodes) = 0;
}
class SelectChildren: public SelectionCommand {
    
}
*/