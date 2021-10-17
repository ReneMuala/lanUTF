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
    UTF8CharList string;
    
    string.append(" Hello world 🤗🥰");
    
    string.append("😝😄🙃", 0);
    
    string.remove(0);
        
    for(int i = 0 ; i < string.size() ; i++){
        std::cout << string[i] << ",";
    }
    std::cout << std::endl;
    
    char * c_string = string.alloc_c_str();
    
    std::cout << c_string << std::endl;
    
    string.free_c_str(c_string);
        
    return 0;
}

