//
//  CommandPrompt.hpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/19/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef CommandPrompt_hpp
#define CommandPrompt_hpp

#include <iostream>
#include <vector>
#include "CommandHandler.hpp"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

class CommandPrompt
{
private:
    CommandHandler ch;
    vector<string> inputTokens;
    void prompt();
    vector<string> inputTokenizer(string input);
    
public:
    CommandPrompt();
    
};

#endif /* CommandPrompt_hpp */
