#pragma once

#include "XMLNode.h"
#include "TreeNode.h"
#include "AttributeNode.h"
#include <string>
#include <vector>
#include <functional>

class XPathParser {
    private:
        struct CommandTriplet {
            std::string childCommand;
            std::string nameCommand;
            std::string predicateCommand;

            CommandTriplet(std::string, std::string, std::string);
        };

        XMLNode* xmlTree;
        
        std::vector<CommandTriplet> commands;
        
        std::vector<TreeNode*> currentlySelectedNodes;


    public:
        XPathParser(XMLNode*, std::string);


    private:
        bool predicateCheck_comparison_attribute(XMLNode*, std::string, std::string, std::string);
        bool predicateCheck_comparison_node(XMLNode*, std::string, std::string, std::string);
        bool predicateCheck_existance_attribute(XMLNode*, std::string);
        bool predicateCheck_existance_node(XMLNode*, std::string);
        bool predicateCheck(XMLNode*, std::string);


        std::vector<TreeNode*> selectionCommand_attribute_children(std::vector<TreeNode*>, std::string);
        std::vector<TreeNode*> selectionCommand_attribute_descendants(std::vector<TreeNode*>, std::string);

        std::vector<TreeNode*> selectionCommand_node_children_position(std::vector<TreeNode*>, std::string, int);
        std::vector<TreeNode*> selectionCommand_node_descendants_position(std::vector<TreeNode*>, std::string, int);
        std::vector<TreeNode*> selectionCommand_node_children_predicate(std::vector<TreeNode*>, std::string, std::string);
        std::vector<TreeNode*> selectionCommand_node_descendants_predicate(std::vector<TreeNode*>, std::string, std::string);

        std::vector<TreeNode*> selectionCommand(std::vector<TreeNode*>, CommandTriplet);


    public:
        std::string execute();
};