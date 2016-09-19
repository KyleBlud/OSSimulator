//
//  CommandPrompt.cpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/19/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "CommandPrompt.hpp"

CommandPrompt::CommandPrompt()
{
    cout << "Welcome to the O- interface!" << endl;
    prompt();
}

void CommandPrompt::prompt()
{
    string input;
    
    while (1)
    {
        cout << "kyle@blud ~ $ ";
        cin >> input;
        inputTokens = inputTokenizer(input);
        
    }
}

vector<string> CommandPrompt::inputTokenizer(string input)
{
    vector<string> tokens;
    string token;
    
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != ' ')
            token += input[i];
        else
            tokens.push_back(token);
    }
    
    return tokens;
}
