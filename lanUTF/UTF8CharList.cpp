//
//  UTF8CharList.cpp
//  lanUTF
//
//  Created by René Descartes Domingos Muala on 16/10/21.
//

#include "UTF8CharList.hpp"
#include <iostream>

size_t UTF8CharList::size() const {
    size_t size = 0;
    UTF8Char * ptr = first;
    while (ptr) {
        size++;
        ptr = ptr->_next;
    } return size;
}

bool UTF8CharList::empty() const {
    return first;
}

const char * UTF8CharList::operator[](size_t index) const {
    size_t subIndex = 0;
    UTF8Char * ptr = first;
    while (ptr) {
        if(subIndex == index) return ptr->c_char();
        subIndex++;
        ptr = ptr->_next;
    } return nullptr;
}

void UTF8CharList::remove(size_t index){
    size_t subIndex = 0;
    UTF8Char * ptr = first, * last = nullptr;
    while (ptr) {
        if(subIndex == index) {
            if(last) last->_next = ptr->_next;
            if(ptr == first) {
                if(last)
                    first = last;
                else
                    first = ptr->_next;
            } delete ptr;
        } subIndex++;
        last = ptr;
        ptr = ptr->_next;
    }
}

char * UTF8CharList::alloc_c_str() const {
    UTF8Char * ptr = first;
    char * c_str = new char('\0');
    while (ptr) {
        strcat(c_str, ptr->c_char());
        ptr = ptr->_next;
    } return c_str;
}

void UTF8CharList::free_c_str(char * str){
    free(str);
}

void UTF8CharList::append(const char * src){
    UTF8Char * last_ptr = first ? first->_last() : nullptr;
    char src_cpy [strlen(src)], *src_cpy_ptr = src_cpy;
    strcpy(src_cpy, src);
    while (*src_cpy_ptr) {
        if(last_ptr){
            last_ptr->_createNext();
            last_ptr=last_ptr->_next;
        } else {
            last_ptr = first = new UTF8Char;
        }
        
        last_ptr->composeUTF8Char(src_cpy_ptr);
        
        switch (last_ptr->octet()) {
            case UTF8Char::UTF8_1o:
                src_cpy_ptr++;
                break;
            case UTF8Char::UTF8_2o:
                src_cpy_ptr+=2;
                break;
            case UTF8Char::UTF8_3o:
                src_cpy_ptr+=3;
                break;
            case UTF8Char::UTF8_4o:
                src_cpy_ptr+=4;
                break;
            default:
                src_cpy_ptr++;
                break;
        }
    }
}

void UTF8CharList::append(const char * src, size_t index) {
    UTF8Char * last_ptr = first->_at(index);
    char src_cpy [strlen(src)], *src_cpy_ptr = src_cpy;
    strcpy(src_cpy, src);
    if(last_ptr){
        while (*src_cpy_ptr) {
            
            last_ptr->_appendNext();
            last_ptr->_next->copy(*last_ptr);
            last_ptr->composeUTF8Char(src_cpy_ptr);
            switch (last_ptr->octet()) {
                case UTF8Char::UTF8_1o:
                    src_cpy_ptr++;
                    break;
                case UTF8Char::UTF8_2o:
                    src_cpy_ptr+=2;
                    break;
                case UTF8Char::UTF8_3o:
                    src_cpy_ptr+=3;
                    break;
                case UTF8Char::UTF8_4o:
                    src_cpy_ptr+=4;
                    break;
                default:
                    src_cpy_ptr++;
                    break;
            } last_ptr=last_ptr->_next;
        }
    } else {
        printf("%s", ("[" + std::string(lanUTFVersion) + " Error]: " + "invalid range [" + std::to_string(index) +"], using default append(...) method.").c_str());
        append(src);
    }
}

void UTF8CharList::clear(){
    UTF8Char * ptr = first, * buff = nullptr;
    while (ptr) {
        buff = ptr->_next;
        delete ptr;
        ptr = buff;
    } first = nullptr;
}

UTF8CharList::UTF8CharList(): first(nullptr){
    
}

UTF8CharList::~UTF8CharList(){
    clear();
}
