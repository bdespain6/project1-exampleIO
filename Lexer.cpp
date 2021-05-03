#include <iostream>
#include "Lexer.h"
#include "ColonAutomaton.h"
#include "ColonDashAutomaton.h"
#include "CommaAutomaton.h"
#include "UndefinedAutomaton.h"
#include "PeriodAutomaton.h"
#include "LeftParen.h"
#include "RightParen.h"
#include "MultiplyAutomaton.h"
#include "AddAutomaton.h"
#include "SchemesAutomaton.h"
#include "FactsAutomaton.h"
#include "RulesAutomaton.h"
#include "QueriesAutomaton.h"
#include "QuestionMarkAutomaton.h"
#include "IDAutomaton.h"
#include "StringAutomaton.h"
#include "CommentAutomaton.h"
#include <cctype>
using namespace std;
Lexer::Lexer() {
    CreateAutomata();
}

Lexer::~Lexer() {
    // TODO: need to clean up the memory in `automata` and `tokens`
}

void Lexer::CreateAutomata() {
    automata.push_back(new ColonAutomaton());
    automata.push_back(new ColonDashAutomaton());
    automata.push_back(new CommaAutomaton());
    // TODO: Add the other needed automata here
    automata.push_back(new PeriodAutomaton());
    automata.push_back(new LeftParen());
    automata.push_back(new RightParen());
    automata.push_back(new AddAutomaton());
    automata.push_back(new MultiplyAutomaton());
    automata.push_back(new SchemesAutomaton());
    automata.push_back(new FactsAutomaton());
    automata.push_back(new RulesAutomaton());
    automata.push_back(new QueriesAutomaton());
    automata.push_back(new QuestionMarkAutomaton());
    automata.push_back(new IDAutomaton());
    automata.push_back(new StringAutomaton());
    automata.push_back(new CommentAutomaton());
    automata.push_back(new UndefinedAutomaton());
}

void Lexer::Run(std::string& input) {
   // CreateAutomata();
   int lineNumber = 1;
    vector <int> maxRead;
    for (unsigned int i = 0; i < automata.size(); i++){
        //        automata.at(i)->setLineNumber(begin);
        maxRead.push_back(0);
    }
    unsigned int placeHolder = 0;
    while (input.size()>1) {
       //increments line number
        while(input[0]=='\n'||isspace(input[0])){
            if(input[0] =='\n') {
                lineNumber++;
            }
            input = input.substr(1);
        }
        //Skips unimportant spaces
        /*while(isspace(input[0]))
        {
            input = input.substr(1);
        }*/

        for (unsigned int i = 0; i < automata.size(); i++) {
            maxRead.at(i)=(automata.at(i)->Start(input));
        }
        unsigned int j = 0;
        for (unsigned int i = 1; i < automata.size(); i++){
            if(maxRead.at(i) > maxRead.at(j))
            {
                j = i;
            }
        }

        //To avoid trying to push something back when there is only an end of line in the string
        if(input.size() > 1) {

            tokens.push_back(automata.at(j)->CreateToken(input.substr(0, maxRead.at(j)), lineNumber));
            lineNumber=lineNumber + automata.at(j)->NewLinesRead();
            placeHolder = maxRead.at(j);
            input = input.substr(placeHolder);
        }
    }
        tokens.push_back(new Token(TokenType::ENDOFFILE, "",lineNumber));
    for(unsigned int i = 0; i < tokens.size();i++)
    {
        cout << tokens.at(i)->toString();
    }
    cout <<"Total Tokens = " << tokens.size();
    // TODO: convert this pseudo-code with the algorithm into actual C++ code
    /*
    set lineNumber to 1
    // While there are more characters to tokenize
    loop while input.size() > 0 {
        set maxRead to 0
        set maxAutomaton to the first automaton in automata

        // TODO: you need to handle whitespace inbetween tokens

        // Here is the "Parallel" part of the algorithm
        //   Each automaton runs with the same input
        foreach automaton in automata {
            inputRead = automaton.Start(input)
            if (inputRead > maxRead) {
                set maxRead to inputRead
                set maxAutomaton to automaton
            }
        }
        // Here is the "Max" part of the algorithm
        if maxRead > 0 {
            set newToken to maxAutomaton.CreateToken(...)
                increment lineNumber by maxAutomaton.NewLinesRead()
                add newToken to collection of all tokens
        }
        // No automaton accepted input
        // Create single character undefined token
        else {
            set maxRead to 1
                set newToken to a  new undefined Token
                (with first character of input)
                add newToken to collection of all tokens
        }
        // Update `input` by removing characters read to create Token
        remove maxRead characters from input
    }
    add end of file token to all tokens
    */
}
