#pragma once

#include "TreeNode.h"
#include <string>


class StringNode: public TreeNode {
    private:
        std::string text;


    public:
        StringNode(std::string);
};