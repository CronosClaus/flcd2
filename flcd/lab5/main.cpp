#include "SymbolTable.h"
#include "Grammar.h"
#include "LL_ParsingTable.h"


int main(int argc, char** argv)
{
    Grammar g{ "Inputs/Grammar/g1.txt" };

    std::cout << "Start symbol: " << g.startSymbol << std::endl;
    std::cout << "\nTerminals\n";
    std::cout << g.terminalsToString() << std::endl;
    std::cout << "\nNon terminals\n";
    std::cout << g.nonTerminalsToString() << std::endl;
    std::cout << "\nProduction rules\n";
    std::cout << g.productionRulesToString() << std::endl;
    std::cout << "\nCheck if CFG: " << (g.checkCFG() ? "true" : "false") << std::endl;



    std::list<std::string> First = g.first("ifstmt");
    //std::list<std::string> Follow = g.follow("B");

    std::cout<< "First:\n";
    for (auto asdf : First)
    {
        std::cout << asdf << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Table:\n";

    LL_ParsingTable table = LL_ParsingTable(g);
    std::cout << table.toString();



    return 0;
}

//std::cout << g.getProductionRuleFor("test", "\"qwer\"").toString();

    /*for (auto it : g.nonTerminals) {
        for (auto terminal : g.terminals) {
            auto result = g.getProductionRuleFor(it, terminal);
            if (!result.isEmpty()) {
                //std::cout << "rule for " << it << " " << terminal << ":\n " << g.getProductionRuleFor(it, terminal).toString() << '\n';
            }
        }
        std::cout << '\n';
    }*/
