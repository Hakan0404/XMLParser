#pragma once

#include "XMLNode.h"
#include <string>
#include <vector>

class XPathParser {
    private:
        XMLNode* xmlTree;
        std::vector<std::string> selectionCommands;
        std::vector<XMLNode*> currentlySelectedNodes;


    public:
        XPathParser(XMLNode*, std::string);
};