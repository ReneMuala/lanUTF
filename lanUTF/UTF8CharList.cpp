//
//  UTF8CharList.cpp
//  lanUTF
//
//  Created by René Descartes Domingos Muala on 15/10/21.
//

#include "UTF8CharList.hpp"
#include <iostream>

UTF8CharList * UTF8CharList::generateNext(){
    return (next = new UTF8CharList);
}

void UTF8CharList::removeNext(){
    UTF8CharList * nextNext = next->next;
    delete next;
    next = nextNext;
}

bool UTF8CharList::byte_match(const std::bitset<8> src, const std::bitset<8> fmt, size_t bitsCount){
    for (int i = 7 ; i > 7 - bitsCount ; i --){
        if(src[i] != fmt[i]) return false;
    } return true;
}

UTF8CharList::UTF8_OctetType const UTF8CharList::get_octetType(const uint8_t value){
    if(byte_match(value, UTF8_1o_format, 1)){
        return UTF8_1o;
    } else if (byte_match(value, UTF8_2o_format, 3)) {
        return UTF8_2o;
    } else if (byte_match(value, UTF8_3o_format, 4)) {
        return UTF8_3o;
    } else if (byte_match(value, UTF8_4o_format, 5)) {
        return UTF8_4o;
    } else {
        return UTF8_Io;
    }
}


void UTF8CharList::readUTF8_4o(char *source){
    if(byte_match(*source, UTF8_4o_format, 5)){
        data[0] = *source;
        source++;
    }
    if(byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[1] = *source;
        source++;
    }
    if(byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[2] = *source;
        source++;
    }
    if(byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[3] = *source;
        source++;
    }
}

void UTF8CharList::readUTF8_3o(char *source){
    if(byte_match(*source, UTF8_3o_format, 4)){
        data[0] = source[0];
        source++;
    }
    if(byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[1] = *source;
        source++;
    }
    if(byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[2] = *source;
        source++;
    }
}

void UTF8CharList::readUTF8_2o(char *source){
    if(byte_match(*source, UTF8_2o_format, 3)){
        data[0] = *source;
        source++;
    }
    if(byte_match(*source, UTF8_intermetidate_byte, 2)){
        data[1] = *source;
        source++;
    }
}

void UTF8CharList::readUTF8_1o(char *source){
    if(byte_match(*source, UTF8_1o_format, 1)){
        data[0] = *source;
        source++;
    }
}

void UTF8CharList::composeUTF8Char(char *source){
    clear();
    switch ((octetType = get_octetType(source[0]))) {
        case UTF8_1o:
            readUTF8_1o(source);
            break;
        case UTF8_2o:
            readUTF8_2o(source);
            break;;
        case UTF8_3o:
            readUTF8_3o(source);
            break;
        case UTF8_4o:
            readUTF8_4o(source);
            break;
        default:
            readUTF8_1o(source);
            break;
    }
}

void UTF8CharList::append(const char *other){
    UTF8CharList * last_ptr = last();
    char * other_copy = (char*)malloc(strlen(other));
    
    strcpy(other_copy, other);
    
    while (*other_copy) {
        last_ptr->generateNext();
        last_ptr=last_ptr->next;
        last_ptr->composeUTF8Char(other_copy);
        switch (last_ptr->octet()) {
            case UTF8_1o:
                other_copy++;
                break;
            case UTF8_2o:
                other_copy+=2;
                break;;
            case UTF8_3o:
                other_copy+=3;
                break;
            case UTF8_4o:
                other_copy+=4;
                break;
            default:
                other_copy++;
                break;
        }
    }
}

char const * UTF8CharList::c_char() const {
    return data;
}

UTF8CharList::UTF8_OctetType const UTF8CharList::octet() const {
    return octetType;
}

UTF8CharList * UTF8CharList::last() const {
    UTF8CharList * last = (UTF8CharList *)((void*)(this));
    while (last->next) {
        last = last->next;
    } return last;
}

UTF8CharList::UTF8CharList(): octetType(UTF8_1o), next(NULL), data("\0"){
    
}

UTF8CharList::~UTF8CharList(){
    clearAll();
}
