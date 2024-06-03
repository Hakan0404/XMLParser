#pragma once

#include "TreeNode.h"
#include <string>


class SingleTagNode: public TreeNode {
    private:
        std::string name;

    public:
        SingleTagNode(TreeNode*, std::string);
        
        void testPrint(int = 0) const;
        void toString(std::string&, int = 0) const;

        std::string getNodeType() const;
};