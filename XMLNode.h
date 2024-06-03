#pragma once

#include "TreeNode.h"
#include "AttributeNode.h"
#include "StringNode.h"
#include <iostream>
#include <vector>
#include <string>


class XMLNode: public TreeNode {
    private:
        std::string name;
        std::vector<TreeNode*> children;
        std::vector<AttributeNode*> attributes;

        static std::string convertFileToString(const std::string);

        static XMLNode* constructNode(XMLNode*, std::string&, int*);

        void setUniqueIds();


    public:
        XMLNode(XMLNode*, std::string);
        
        void addChild(TreeNode*);
        
        void addAttribute(std::string, std::string);
        void setAttribute(std::string, std::string);
        AttributeNode* getAttribute(std::string) const;
        AttributeNode* getAttribute(int) const;
        void removeAttribute(std::string);
        std::vector<AttributeNode*> getAllAttributes();

        bool hasAttribute(std::string) const;
        bool hasAttribute(std::string, std::string) const;

        std::string getName() const;

        // temporary function for testing; to remove later
        void testPrint(int = 0) const;
        void toString(std::string&, int = 0) const;

        std::vector<TreeNode*> getChildren() const;
        std::vector<XMLNode*> getXMLChildren();
        std::vector<StringNode*> getStringChildren();

        void getXMLDescendants(std::vector<XMLNode*>&);

        static XMLNode* constructTree(const std::string&);

        std::string getNodeType() const;

        ~XMLNode();
};