#pragma once

#include "TreeNode.h"
#include "AttributeNode.h"
#include "StringNode.h"
#include <iostream>
#include <vector>
#include <string>


class XMLNode: public TreeNode {
    private:
        /*struct XMLAttribute {
            std::string key;
            std::string value;
            
            XMLAttribute(std::string, std::string);
        };*/

        std::string name;
        std::vector<TreeNode*> children;
        std::vector<AttributeNode*> attributes;

        static std::string convertFileToString(const std::string);

        static XMLNode* constructNode(XMLNode*, std::string&, int*);

        void setUniqueIds();


    public:
        // parent + name and attributes string
        XMLNode(XMLNode*, std::string);
        
        void addChild(TreeNode*);
        
        void addAttribute(std::string, std::string);
        void setAttribute(std::string, std::string);
        void setAttribute(int, std::string);
        AttributeNode* getAttribute(std::string) const;
        AttributeNode* getAttribute(int) const;
        std::vector<AttributeNode*> getAllAttributes();

        bool hasAttribute(std::string) const;
        bool hasAttribute(std::string, std::string) const;

        std::string getName() const;

        // temporary function for testing; to remove later
        void testPrint(int = 0) const;

        // UNSAFE; remove later
        std::vector<TreeNode*> getChildren() const;
        std::vector<XMLNode*> getXMLChildren();
        std::vector<StringNode*> getStringChildren();

        // UNSAFE
        void getXMLDescendants(std::vector<XMLNode*>&);

        // the string to construct from and the node's parent
        static XMLNode* constructTree(const std::string&);

        std::string getNodeType() const;
};