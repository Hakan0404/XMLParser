#pragma once

#include "XMLNode.h"
#include <string>


class ConsoleInterface {
    private:
        XMLNode* xmlTree;


        static void open(XMLNode*&, bool&, std::string, std::string&);
        static void close(XMLNode*, bool&);
        static void saveas(XMLNode*, std::string, bool&);
        static void help();
        static void exit(XMLNode*, bool&, bool&);

        static void print(XMLNode*, bool&);
        static void select(XMLNode*, std::string, std::string, bool&);
        static void set(XMLNode*, std::string, std::string, std::string, bool&);
        static void child(XMLNode*, std::string, int, bool&);
        static void children(XMLNode*, std::string, bool&);
        static void text(XMLNode*, std::string, bool&);
        static void deleteAttribute(XMLNode*, std::string, std::string, bool&);
        static void newChild(XMLNode*, std::string, std::string, bool&);
        static void xpath(XMLNode*, std::string, bool&);

    public:
        static void parseCommand(XMLNode*&, bool&, std::string, bool&, std::string&);
};