#pragma once

#include "TreeNode.h"
#include <string>


class StringNode: public TreeNode {
    private:
        std::string text;


    public:
        StringNode(TreeNode*, std::string);

        std::string getText() const;

        // temporary function for testing; to remove later
        void testPrint(int = 0) const;
        void toString(std::string&, int = 0) const;
        
        std::string getNodeType() const;
};