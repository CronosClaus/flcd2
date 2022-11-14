import re

import sympy.ntheory as np


class SymbolTable:
    def __init__(self):
        self.__m = 11
        self.__st = [[] for _ in range(self.__m)]
        self.__occupancy = 0

    def hash(self, token):
        s = 0
        for character in token:
            s += ord(character)
        return s % self.__m

    def rehash(self):
        self.__m = np.nextprime(self.__m)  # get the next prime number
        st = [[] for _ in range(self.__m)]

        for ll in self.__st:
            for elem in ll:
                st[self.hash(elem)].append(elem)

        self.__st = st

    def add(self, token):
        key = self.hash(token)
        if token in self.__st[key]:
            return key, self.__st[key].index(token)
        self.__st[key].append(token)
        self.__occupancy += 1

        if self.__occupancy / self.__m > 0.8:
            self.rehash()
            key = self.hash(token)

        return key, self.__st[key].index(token)

    def getst(self):
        return self.__st


class Scanner:
    def __init__(self):
        self.__reserved_word = "f|int|char|main|let|read|check|write|for|ret|else|const|while|and|or"
        self.__operator = "(\+|-){0,1}<-|=|%|\+|-|\*|\||<|>|(<=)|(\.\.\.)|(>=)|(<-)|(\+<-)|(-<-)|/"
        self.__identifier = "(_)*([a-z]+|[A-Z]+)([0-9]+)*"
        self.__constant = "0|([+\-]*[1-9][0-9]*)|(\"[a-zA-Z_]{2,}\")|(\'[a-zA-Z_]\')"
        self.__separator = "\(|\)|\[|\]|\||;|,|(\.\.\.)|#"
        self.__tokens = []
        self.__pif = []
        self.__sti = SymbolTable()
        self.__stc = SymbolTable()

    def scan(self, filename):
        with open(filename) as f:
            while True:
                line = f.readline()
                if not line:
                    break
                token = re.search(
                    "\s*-{0,1}[a-zA-Z0-9_\"]+\s*|\s*(<=)|(\.\.\.)|(>=)|(<-)|(\+<-)|(-<-)|[^a-zA-Z0-9_;\s]\s*|\s*;+\s*",
                    line)
                start = 0
                while token:
                    self.__tokens.append(token.group().strip())
                    start += token.end()
                    token = re.search(
                        "\s*-{0,1}[a-zA-Z0-9_\"]+\s*|\s*(<=)|(\.\.\.)|(>=)|(<-)|(\+<-)|(-<-)|[^a-zA-Z0-9_;\s]\s*|\s*;+\s*",
                        line[start:])
        correct = True

        for token in self.__tokens:
            if token == '':
                continue

            if re.fullmatch(self.__reserved_word, token) or re.fullmatch(self.__operator, token) \
                    or re.fullmatch(self.__separator, token):
                self.__pif.append((token, -1))
            elif re.fullmatch(self.__identifier, token):
                index = self.__sti.add(token)
                self.__pif.append(("id", index))
            elif re.fullmatch(self.__constant, token):
                index = self.__stc.add(token)
                self.__pif.append(("const", index))
            else:
                print("Lexical error at " + token)
                correct = False

        if correct:
            print("Lexically correct")

        with open("PIF.out", "w") as f:
            for p in self.__pif:
                f.write(str(p))
                f.write("\n")
        with open("ST.out", "w") as f:
            f.write(str(self.__sti.getst()))
            f.write("\n")
            f.write(str(self.__stc.getst()))


if __name__ == '__main__':
    file = "p1.TXT"
    s = Scanner()
    s.scan(file)
