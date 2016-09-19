//
//  CommandHandler.hpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/19/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef CommandHandler_hpp
#define CommandHandler_hpp

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

class CommandHandler
{
private:
    vector<string> commandTokens;
    
public:
    CommandHandler(vector<string> inputTokens);
};

#endif /* CommandHandler_hpp */
