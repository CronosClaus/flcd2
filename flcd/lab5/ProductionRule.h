#pragma once

#include <list>
#include <string>


class ProductionRule
{
public:
    std::list<std::string> leftSide;
    std::list<std::list<std::string>> rightSide;

    ProductionRule(std::list<std::string> LeftSide, std::list<std::list<std::string>> RightSide);
    ProductionRule();

    bool isEmpty();

    std::string toString();

    bool operator==(const ProductionRule& rhs) {
        std::string result1, result2;
        result1 = this->toString();
        ProductionRule temp = ProductionRule(rhs.leftSide, rhs.rightSide);
        result2 = temp.toString();
        return result1 == result2;
        /*if (this->leftSide.size() == rhs.leftSide.size()) {
            for (int i = 0; i < this->leftSide.size(); i++) {
                if (*std::next(this->leftSide.begin(), i) != *std::next(rhs.leftSide.begin(), i)) {
                    return false;
                }
            }
            if (this->rightSide.size() == rhs.rightSide.size()) {
                for (int j = 0; j < this->rightSide.size(); j++) {
                    auto elem1 = *std::next(this->rightSide.begin(), j);
                    auto elem2 = *std::next(rhs.rightSide.begin(), j);
                    if (elem1.size() == elem2.size()) {
                        for (int i = 0; i < elem1.size(); i++) {
                            if (*std::next(elem1.begin(), i) != *std::next(elem2.begin(), i)) {
                                return false;
                            }
                        }
                    }
                    else {
                        return false;
                    }
                }
            }
            return true;
        }
        else {
            return false;
        }*/
    };
};
