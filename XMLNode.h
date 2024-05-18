#pragma once

#include "TreeNode.h"
#include <iostream>
#include <vector>
#include <string>


class XMLNode: public TreeNode {
    private:
        struct XMLAttribute {
            std::string key;
            std::string value;
            
            XMLAttribute(std::string, std::string);
        };

        std::string name;
        std::vector<TreeNode*> children;
        std::vector<XMLAttribute> attributes;


    public:
        // parent and name
        XMLNode(TreeNode*, std::string);
        
        void addChild(TreeNode*);
        
        void addAttribute(XMLAttribute);
        void setAttribute(std::string, std::string);
        void setAttribute(int, std::string);
};