#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "XMLNode.h"

TEST_CASE("Constuctor test") {
    //std::string constructorString = ;
    XMLNode node1 = XMLNode(nullptr, "Name id=\"4\" type=\"fruit\"");
    CHECK(node1.getName() == "Name");
    CHECK(node1.hasAttribute("id"));
    CHECK(node1.hasAttribute("id", "4"));
    CHECK(node1.hasAttribute("type", "fruit"));
}

TEST_CASE("Attribute functions test") {
    XMLNode node1 = XMLNode(nullptr, "Name id=\"4\" type=\"fruit\"");
    node1.addAttribute("att1", "val1");
    CHECK(node1.hasAttribute("att1", "val1"));
    node1.removeAttribute("att1");
    CHECK(!node1.hasAttribute("att1"));
    node1.setAttribute("id", "5");
    CHECK(node1.hasAttribute("id", "5"));
}

TEST_CASE("Node type test") {
    XMLNode node1 = XMLNode(nullptr, "Name id=\"4\" type=\"fruit\"");
    CHECK(node1.getNodeType() == "XMLNode");
}

TEST_CASE("Construction and toString() test") {
    XMLNode* tree = XMLNode::constructTree("testxml.txt");
    std::string treeManualString = "<root id=\"0\">\n\t<person id=\"9\">\n\t\tlol\n\t\t<newline/>\n\t\t<address id=\"10\">\n\t\t</address>\n\t</person>\n</root>";
    std::string treeAutoString;
    tree->toString(treeAutoString);
    CHECK(treeAutoString == treeManualString);
    delete tree;
}

TEST_CASE("getXMLChildren() test") {
    XMLNode* tree = XMLNode::constructTree("testxml.txt");
    std::vector<XMLNode*> xmlChildren = tree->getXMLChildren();
    for (int i = 0; i < xmlChildren.size(); i++) {
        CHECK(xmlChildren[i]->getNodeType() == "XMLNode");
        CHECK(xmlChildren[i]->getName() == "person");
    }
    delete tree;
}

TEST_CASE("getStringChildren() test") {
    XMLNode* tree = XMLNode::constructTree("testxml.txt");
    std::vector<StringNode*> stringChildren = tree->getStringChildren();
    for (int i = 0; i < stringChildren.size(); i++) {
        CHECK(stringChildren[i]->getNodeType() == "StringNode");
        CHECK(stringChildren[i]->getText() == "lol");
    }
    delete tree;
}

TEST_CASE("getXMLDescendants() test") {
    XMLNode* tree = XMLNode::constructTree("testxml.txt");
    std::vector<XMLNode*> xmlDescendants;
    tree->getXMLDescendants(xmlDescendants);
    for (int i = 0; i < xmlDescendants.size(); i++) {
        bool toCheck = (xmlDescendants[i]->getNodeType() == "XMLNode" && 
                        (xmlDescendants[i]->getName() == "root" || xmlDescendants[i]->getName() == "person" || xmlDescendants[i]->getName() == "address"));
        CHECK(toCheck);
    }
    delete tree;
}