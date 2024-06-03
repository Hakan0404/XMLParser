#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "XMLNode.h"
#include "XPathParser.h"

TEST_CASE("attribute_children") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "/@*");
    CHECK(parser.execute() == "{id=\"0\"}");
}

TEST_CASE("attribute_descendants") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "//@*");
    CHECK(parser.execute() == "{id=\"0\", id=\"1_0_0\", id=\"1_0_1\", type=\"home\", id=\"1_1\", type=\"work\", id=\"2\", id=\"3\", id=\"1_2\", id=\"4\", id=\"5\", id=\"1\"}");
}

TEST_CASE("node_children_position") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "/person[0]");
    CHECK(parser.execute() == "{{personText1, personText2}}"); 
}

TEST_CASE("node_descendatns_position") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "//*[3]");
    CHECK(parser.execute() == "{{Manhattan, NYC}}");
}

TEST_CASE("node_children_predicate") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "/*[@id=\"1_2\"]");
    CHECK(parser.execute() == "{{personText3, personText4}}");
}

TEST_CASE("node_descendants_predicate") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "//*[@id=\"3\"]");
    CHECK(parser.execute() == "{{Dentist}}");
}

TEST_CASE("comparison_node") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "//*[occupation=\"Dentist\"]");
    CHECK(parser.execute() == "{{personText1, personText2}}");
}

TEST_CASE("existance_attribute") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "//*[@type]");
    CHECK(parser.execute() == "{{Brooklyn, NY}, {Manhattan, NYC}}");
}

TEST_CASE("existance_node") {
    XMLNode* tree = XMLNode::constructTree("xpathtestxml.txt");
    XPathParser parser(tree, "//*[occupation]");
    CHECK(parser.execute() == "{{personText1, personText2}}");
}