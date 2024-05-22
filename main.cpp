#include "XMLNode.h"
#include "StringNode.h"
#include <iostream>
#include <fstream>


int main() {
    
    /*
    std::ifstream inputStream("testxml.txt");

    std::string fileText;
    std::string current;

    while (!inputStream.eof()) {
        getline(inputStream, current);
        fileText = fileText + current;
    }
    //std::cout << fileText << std::endl;


    while (fileText.contains("\t")) {
        int tabIndex = fileText.find("\t");
        //std::cout << tabIndex << std::endl;
        fileText.erase(tabIndex, 1);
    }
    //std::cout << fileText << std::endl;

    fileText = "<root>" + fileText + "</root>";
    std::cout << "Printing of modified file string:" << std::endl;
    std::cout << fileText << "\n\n";
    */

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
    

    std::string endString = "end of main reached";
    std::cout << endString << std::endl;
    //std::cout << endString.substr(0, endString.length() - 1);
    return 0;
}