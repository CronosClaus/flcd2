#pragma once

#include <list>
#include <string>
#include "Grammar.h"
#include "ProductionRule.h"
#include <map>
#include <utility>
#include <vector>


class LL_ParsingTable
{
public:
    std::list<std::string> nonTerminals;
    std::list<std::string> terminals;

    std::map<std::pair<std::string, std::string>, ProductionRule> table;

    LL_ParsingTable(Grammar Gr);

    std::string toString();

    bool seq_accepted(std::vector< std::string > sequence);

private:
    void buildTable(Grammar Gr);
};
