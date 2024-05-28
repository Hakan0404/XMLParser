#include "XMLNode.h"
#include "StringNode.h"
#include "XPathParser.h"
#include <iostream>
#include <fstream>
#include <algorithm>


int main() {
    //int* stringIndex = new int(69);
    //std::cout << *stringIndex << std::endl;

    /*
    if (true) {
        throw std::invalid_argument("you dun fucked up");
    }
    */

    //std::string testStr = "lol";
    //std::cout << testStr.size() << std::endl;
    //std::cout << testStr.find('o') << std::endl;

    //XMLNode testNode = XMLNode(nullptr, "NodeName key=\"value\" weebness=\"9001\"");
    //testNode.testPrint();
    //std::cout << std::endl;

    //std::string emptyString;
    //emptyString.clear();
    //std::cout << emptyString.empty() << std::endl;

    XMLNode* tree = XMLNode::constructTree("testxml.txt");
    std::cout << "Printing of constructed tree:" << std::endl;
    tree->testPrint(0);
    std::cout << "\n\n";

    //std::string xPathExpression;
    //std::cin >> xPathExpression;
    
    //XPathParser testParser = XPathParser(tree, xPathExpression);

    std::vector<int> intVector = {1,2,3,4,5};
    /*intVector.erase(intVector.begin() + 2);
    for (int i = 0; i < intVector.size(); i++) {
        std::cout << intVector[i] << " ";
    }
    std::cout << std::endl;*/

    std::cout << "Vector find: " << (std::find(intVector.begin(), intVector.end(), 3) != intVector.end()) << std::endl;

    std::string endString = "end of main reached";
    std::cout << endString << std::endl;
    //std::cout << endString.substr(0, endString.length() - 1);
    return 0;
}