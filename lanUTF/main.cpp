//
//  main.cpp
//  lanUTF
//
//  Created by René Descartes Domingos Muala on 14/10/21.
//

#include <iostream>
#include <string>
#include <bitset> // <3

#include "UTF8CharList.hpp"

int main(int argc, const char * argv[]) {
    UTF8CharList list;
    
    list.composeUTF8Char("á");
    
    list.append("álls");
    
    std::cout << list.c_char();

    return 0;
}

