#include "Grammar.h"

bool sergiu_find(std::list<std::string> qwer, std::string f)
{
    for (auto a : qwer)
    {
        if (a == f)
        {
            return true;
        }
    }
    return false;
}

Grammar::Grammar(std::string FilePath)
{
    readFromFile(FilePath);
}

void Grammar::pushProductionRule(ProductionRule Pr)
{
    productionRules.push_back(Pr);
}

void Grammar::popProductionRule(ProductionRule Pr)
{
    productionRules.erase(std::find(productionRules.begin(), productionRules.end(), Pr));
}

void Grammar::readFromFile(std::string FilePath)
{
    std::fstream Input;
    Input.open(FilePath);

    std::string Line;
    getline(Input, Line);
    startSymbol = Line;

    while (getline(Input, Line))
    {
        std::stringstream ss;
        ss << Line;

        std::list<std::string> LeftSide;
        std::list<std::string> OneElementRightSide;
        std::list<std::list<std::string>> RightSide;
        std::string Word;
        ss >> Word;
        while (Word != "::=")
        {
            if (Word[0] == '\"' && Word[Word.size() - 1] == '\"')
            {
                terminals.push_back(Word);
            }
            else
            {
                nonTerminals.push_back(Word);
            }
            LeftSide.push_back(Word);
            ss >> Word;
        }
        while (ss >> Word)
        {
            if (Word[0] == '\"' && Word[Word.size() - 1] == '\"')
            {
                terminals.push_back(Word);
                // RightSideResult.push_back(Word);
                // CurrentResult += Word + " ";
                OneElementRightSide.push_back(Word);
            }
            else if (Word != "|")
            {
                nonTerminals.push_back(Word);
                // RightSideResult.push_back(Word);
                // CurrentResult += Word + " ";
                OneElementRightSide.push_back(Word);
            }
            else
            {
                RightSide.push_back(OneElementRightSide);
                // CurrentResult = "";
                OneElementRightSide.clear();
            }
        }
        RightSide.push_back(OneElementRightSide);

        productionRules.push_back(ProductionRule{LeftSide, RightSide});

        eliminateDuplicates(terminals);
        eliminateDuplicates(nonTerminals);
    }

    Input.close();
}

std::list<ProductionRule> Grammar::getProductionRulesFor(std::string NonTerminal)
{
    std::list<ProductionRule> Results;

    for (auto pr : productionRules)
    {
        if (*pr.leftSide.begin() == NonTerminal)
        {
            Results.push_back(pr);
        }
    }

    return Results;
}

bool Grammar::checkCFG()
{
    for (auto& pr: productionRules)
    {
        if (pr.leftSide.size() != 1)
        {
            return false;
        }

        std::string Element = *pr.leftSide.begin();
        if (Element[0] == '\"' && Element[Element.size() - 1] == '\"')
        {
            return false;
        }
    }
    return true;
}

std::list<std::string> Grammar::firstRecursive(std::string NonTerminal)
{
    std::list<ProductionRule> ProdRulesForGivenNonTerminal = getProductionRulesFor(NonTerminal);
    std::list<std::string> Result;
    std::list<std::string> TempFirstStore;

    for (auto Pr : ProdRulesForGivenNonTerminal)
    {
        for (auto El : Pr.rightSide)
        {
            for (auto FirstElement : El)
            {
                if ((FirstElement[0] == '\"' && FirstElement[FirstElement.size() - 1] == '\"') || FirstElement == "eps")
                {
                    Result.push_back(FirstElement);
                    break;
                }
                else
                {
                    TempFirstStore = firstRecursive(FirstElement);
                    // auto Found = std::find(TempFirstStore.begin(), TempFirstStore.end(), "eps");
                    if (sergiu_find(TempFirstStore, "eps"))
                    {
                        // TempFirstStore.erase(Found);

                        Result.splice(Result.end(), TempFirstStore);
                    }
                    else
                    {
                        Result.splice(Result.end(), TempFirstStore);
                        break;
                    }
                }
            }
        }
    }

    eliminateDuplicates(Result);
    return Result;
}

std::list<ProductionRule> Grammar::getProductionRulesRightSide(std::string NonTerminal)
{
    std::list<ProductionRule> Result;
    for (auto Pr : productionRules)
    {
        for (auto Element : Pr.rightSide)
        {
            for (auto Symbol : Element)
            {
                if (Symbol == NonTerminal)
                {
                    Result.push_back(Pr);
                }
            }
        }
    }
    return Result;
}

std::list<std::string> Grammar::follow(std::string NonTerminal)
{

    std::list<ProductionRule> ProductionRulesForNonTerminal = getProductionRulesRightSide(NonTerminal);
    std::list<std::string> Result;

    for (auto Pr : ProductionRulesForNonTerminal)
    {
        for (auto El : Pr.rightSide)
        {
            bool broke = false;
            for (int i = 0; i < El.size() - 1; ++i)
            {
                std::string Current = *std::next(El.begin(), i);
                if (Current == NonTerminal)
                {

                    for (int j = i + 1; j < El.size(); ++j)
                    {
                        std::string Next = *std::next(El.begin(), j);
                        std::cout << Current << " " << Next << std::endl;
                        if ((Next[0] == '\"' && Next[Next.size() - 1] == '\"') || Next == "eps")
                        {
                            // std::cout << "break\n";
                            Result.push_back(Next);
                            broke = true;
                            break;
                        }
                        std::list<std::string> Aux = firstRecursive(Next);
                        std::list<std::string> AuxCopy = Aux;
                        Result.splice(Result.end(), Aux);

                        if (!sergiu_find(AuxCopy, "eps"))
                        {
                            broke = true;
                            std::cout << "NU DA EPS\n";
                            break;
                        }
                    }
                }
            }
            if (!broke)
            {
                // std::cout << *Pr.leftSide.begin() << std::endl;
                Result.splice(Result.end(), follow(*Pr.leftSide.begin()));
            }
        }
    }

    eliminateDuplicates(Result);
    return Result;
}

std::list<std::string> Grammar::first(std::string NonTerminal)
{
    std::list<std::string> Result = firstRecursive(NonTerminal);
    // auto Found = std::find(Result.begin(), Result.end(), "eps");
    // if (sergiu_find(Result, "eps"))
    // {
    //     Result.erase(Found);
    // }
    return Result;
}

std::string Grammar::nonTerminalsToString()
{
    std::string Result;

    for (auto& nt : nonTerminals)
    {
        Result += nt + ", ";
    }

    if (Result != "")
    {
        Result.pop_back();
        Result.pop_back();
    }
    return Result;
}

std::string Grammar::terminalsToString()
{
    std::string Result;

    for (auto& nt : terminals)
    {
        Result += nt + ", ";
    }

    if (Result != "")
    {
        Result.pop_back();
        Result.pop_back();
    }
    return Result;
}

std::string Grammar::productionRulesToString()
{
    std::string Result;

    for (auto& nt : productionRules)
    {
        Result += nt.toString() + "\n";
    }

    if (Result != "")
    {
        Result.pop_back();
        Result.pop_back();
    }
    return Result;
}

std::map<std::string, std::list<std::string>> Grammar::getFirstForAllNonTerminals()
{
    std::map<std::string, std::list<std::string>> Result;
    for (auto nonTerminal : nonTerminals)
    {
        Result[nonTerminal] = first(nonTerminal);
    }
    return Result;
}

ProductionRule Grammar::getProductionRuleFor(std::string NonTerminal, std::string Terminal)
{
    std::list<ProductionRule> ProdRulesForGivenNonTerminal = getProductionRulesFor(NonTerminal);

    for (auto Pr : ProdRulesForGivenNonTerminal) {
        popProductionRule(Pr);
        for (auto El : Pr.rightSide) {
            std::list<std::list<std::string>> rightSide;
            rightSide.push_back(El);
            ProductionRule newPr = ProductionRule(Pr.leftSide, rightSide);
            pushProductionRule(newPr);
            std::list<std::string> terminals = first(NonTerminal);
            popProductionRule(newPr);
            if (sergiu_find(terminals, Terminal)) {
                pushProductionRule(Pr);
                return newPr;
            }
        }
        pushProductionRule(Pr);
    }

    return ProductionRule();
}

void Grammar::eliminateDuplicates(std::list<std::string>& List)
{
    std::set<std::string> Set;

    for (auto element : List)
    {
        Set.insert(element);
    }

    List.clear();
    for (auto element: Set)
    {
        List.push_back(element);
    }
}
