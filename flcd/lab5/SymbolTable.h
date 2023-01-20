#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <list>
#include <iostream>


class SymbolTable
{
private:
    std::string* hash_table;
    int size;

    int hash(std::string Element);

public:
    SymbolTable(int Size);

    int add(std::string Element);
    std::string toString();

    ~SymbolTable();
};

#endif
