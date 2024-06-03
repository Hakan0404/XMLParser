#pragma once

#include <iostream>
#include <string>


class TreeNode {
    private:
        TreeNode* parent;

    public:
        TreeNode(TreeNode*);

        virtual void testPrint(int = 0) const = 0;
        virtual std::string getNodeType() const = 0;
        virtual void toString(std::string&, int = 0) const = 0;
        //friend std::ostream& operator<<(std::ostream&, const TreeNode&) = 0;

        virtual ~TreeNode();
};