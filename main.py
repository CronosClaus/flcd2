import re

import sympy.ntheory as np


class FA:
    def __init__(self):
        self.__states = []
        self.__initial_state = ''
        self.__alphabet = set()
        self.__transitions = {}
        self.__finalStates = []

    def isAccepted(self, DFA):
        currState = self.__initial_state
        for symbol in DFA:
            ok = False
            for tr in self.__transitions:
                for trr in self.__transitions[tr]:
                    if isinstance(self.__transitions[tr][trr], list):
                        if symbol in self.__transitions[tr][trr] and tr == currState:
                            ok = True
                            currState = trr
                            continue
                    else:
                        if symbol == self.__transitions[tr][trr] and tr == currState:
                            ok = True
                            currState = trr
                            continue
            if not ok:
                return False
        return True

    def readFromFile(self, fileName):
        with open(fileName, "r") as f:
            line = f.readline()
            while line:
                line = line.split()
                if line[0][0] == '*':
                    self.__finalStates.append(line[0][1])
                    self.__states.append(line[0][1])
                else:
                    self.__states.append(line[0][0])

                if line[0][-1] == "+":
                    self.__initial_state = line[0][0]

                d = eval(line[2])

                for i in d:
                    if not isinstance(d[i], list):
                        self.__alphabet.add(d[i])
                    else:
                        for j in d[i]:
                            self.__alphabet.add(j)

                if line[0][0] == '*':
                    self.__transitions[line[0][1]] = d
                else:
                    self.__transitions[line[0][0]] = d

                line = f.readline()

    def display(self):
        print(" s - states\n a - alphabet\n t - transitions\n fs - final states\n all - all elements")
        display = input("What elements of the finite automata you want to see?\n")
        if display == "s":
            print("states : ", end=" ")
            print(self.__states)
        elif display == "a":
            print("alphabet : ", end=" ")
            print(list(self.__alphabet))
        elif display == "t":
            print("transitions : ", end=" ")
            print(self.__transitions)
        elif display == "fs":
            print("finalStates : ", end=" ")
            print(self.__finalStates)
        elif display == "all":
            print("states : ", end=" ")
            print(self.__states)
            print("alphabet : ", end=" ")
            print(list(self.__alphabet))
            print("transitions : ", end=" ")
            print(self.__transitions)

            print("finalStates : ", end=" ")
            print(self.__finalStates)

    def checkDFA(self, fileName):
        with open(fileName, "r") as f:
            line = f.readline()
            while line:
                if self.isAccepted(line.strip()):
                    print(line.strip() + " is accepted bu the FA")
                line = f.readline()


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
        self.__identifier = FA()
        self.__identifier.readFromFile("FA_identifier.in")
        self.__string_constant = "(\"[a-zA-Z_]{2,}\")|(\'[a-zA-Z_]\')"
        # 0 | ([+\-] *[1-9][0-9] *) |
        self.__int_constant = FA()
        self.__int_constant.readFromFile("FA_integer_constant.in")
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
            elif self.__identifier.isAccepted(token):
                index = self.__sti.add(token)
                self.__pif.append(("id", index))
            elif self.__int_constant.isAccepted(token) or re.fullmatch(self.__string_constant, token):
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
    file = "p1err.TXT"
    s = Scanner()
    s.scan(file)
