//
//  UTF8CharList.hpp
//  lanUTF
//
//  Created by René Descartes Domingos Muala on 15/10/21.
//

#ifndef UTF8CharList_hpp
#define UTF8CharList_hpp

#include <stdio.h>
#include <stdlib.h>
#include <bitset>

class UTF8CharList {
public:
    
    const std::bitset<8> UTF8_intermetidate_byte = 0x80;
    
    const std::bitset<8> UTF8_1o_format = 0x00;
    
    const std::bitset<8> UTF8_2o_format = 0xC0;
    
    const std::bitset<8> UTF8_3o_format = 0xE0;
    
    const std::bitset<8> UTF8_4o_format = 0xF0;
    
    typedef enum {
        /// 0XXXXXXX ; Simple ASCII
        UTF8_1o,
        /// 110YYYYY + 10XXXXXX
        UTF8_2o,
        /// 1110ZZZZ + 10YYYYYY + 10XXXXXX
        UTF8_3o,
        /// 11110UUU + 10UUZZZZ + 10YYYYYY + 10XXXXXX
        UTF8_4o,
        /// Invalid Octet, may be a result of bad encodig
        UTF8_Io,
    } UTF8_OctetType;
    
private:
    
    char data[4];
    
    UTF8_OctetType octetType;
    
    UTF8CharList * next;
    
    UTF8CharList * generateNext();
    
    void removeNext();
    
public:
    
    bool byte_match(const std::bitset<8> src, const std::bitset<8> fmt, size_t n);
    
    UTF8_OctetType const get_octetType(uint8_t const);
    
    void readUTF8_4o(char * source);
    
    void readUTF8_3o(char * source);
    
    void readUTF8_2o(char * source);
    
    void readUTF8_1o(char * source);
    
    void composeUTF8Char(char * source);
    
    void append(const char * other);
    
    char const * c_char() const;
    
    UTF8_OctetType const octet() const;
    
    // TODO: IMPLEMENT
    size_t c_str(char * dst) const;
    
    /// Clear all Chars in the list
    void clearAll(){
        clear();
        if(next){
            next->clearAll();
        }
    }
    
    /// Clear this char
    void clear(){
        for(int i = 0 ; i < 4 ; i++)
        data[i] = '\0';
    }
    
    UTF8CharList * last() const;
        
    UTF8CharList();
    
    ~UTF8CharList();
    
};

#endif /* UTF8CharList_hpp */
