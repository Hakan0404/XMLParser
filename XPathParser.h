#pragma once

#include "XMLNode.h"
#include <string>
#include <vector>
#include <functional>

class XPathParser {
    private:
        XMLNode* xmlTree;
        std::vector<std::string> selectionCommands;
        std::vector<XMLNode*> currentlySelectedNodes;


    public:
        XPathParser(XMLNode*, std::string);


    private:
        std::function<std::vector<XMLNode*>(std::vector<XMLNode*>)> createSelectionCommand(std::string);
};