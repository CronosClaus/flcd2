The class Scanner has:

* self.__reserved_word 
* self.__operator 
* self.__identifier 
* self.__constant 
* self.__separator 

that are regex strings for matching their respective token
  
* self.__tokens = [] 

list of tokens

* self.__pif = [] 
* self.__sti = SymbolTable()
* self.__stc = SymbolTable()

pif is the program internal form a list of tuples (token, number)
if token is reserved word or operator or separator number is -1 else 
is the position in which you find the token in ST

sti and stc are the simbol tables for identifiers and constants respectively 

the only method of this class is 
*       def scan(filename)
that opens the file reads line by line and tokenizes lines into atoms, then for each atom 
it tries to find it's type and if it doesn't find it then it prints on stdout the lexical error
and if there's no lexical error then it prints "Lexically correct"


**FA.in**

>ebnf:
>* file = {line} 
>* line = state **'='** **'{'** state **':'** (alphabetElem | listElems) 
   > { **','** state **':'** (alphabetElem | listElems) } **'}'**
>* state = \[**'\*'**\]char\['+']
>* char = lowerCase | upperCase
>* lowerCase = 'a' | 'b' | ... | 'z'
>* upperCase = 'A' | 'B' | ... | 'Z'
>* int = '0' | '1' | ... | '9'
>* specialChar = '-' | '+' | '_' | '\' | '/' | ...
>* alphabetElem = char | int | specialChar
>* listElems = '\[' alphabetElem { ',' alphabetElem} ']'

I iterate through the given sequence in the file DFA.in and for each char I want to see if
in the transition dictionary I have a transition with the current char from the current state
to another, if yes then we change the current state and go to the next char, if not then the sequence 
is not valid