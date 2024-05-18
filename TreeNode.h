#pragma once

#include <iostream>
#include <string>


class TreeNode {
    private:
        TreeNode* parent;

        static TreeNode* constructNodeHelper(std::string, TreeNode*);

    public:
        TreeNode();
        TreeNode(TreeNode*);

        //friend std::ostream& operator<<(std::ostream&, const TreeNode&) = 0;
        // the string to construct from and the node's parent
        static TreeNode* constructNode(std::string, TreeNode*);
};