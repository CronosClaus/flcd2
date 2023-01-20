#include "ProductionRule.h"

ProductionRule::ProductionRule(std::list<std::string> LeftSide, std::list<std::list<std::string>> RightSide)
{
    leftSide = LeftSide;
    rightSide = RightSide;
}

ProductionRule::ProductionRule()
{
}

bool ProductionRule::isEmpty()
{
    return rightSide.size() == 0;
}

std::string ProductionRule::toString()
{
    std::string Result;
    for (auto& element: leftSide)
    {
        Result += element + " ";
    }
    Result += "-> ";
    for (auto& res : rightSide)
    {
        for (auto& el : res)
        {
            Result += el + " ";
        }
        Result += "| ";
    }
    if (Result != "")
    {
        Result.pop_back();
        Result.pop_back();
    }
    return Result;
}
