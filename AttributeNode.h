#pragma once

#include "TreeNode.h"
#include <iostream>
#include <vector>
#include <string>

class AttributeNode: public TreeNode {
    private:
        std::string key;
        std::string value;


    public:
        AttributeNode(TreeNode*, std::string, std::string);
        
        std::string getKey() const;
        std::string getValue() const;
        void setValue(std::string);

        void testPrint(int = 0) const;
        std::string getNodeType() const;
};