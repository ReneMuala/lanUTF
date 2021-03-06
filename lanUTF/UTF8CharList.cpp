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

size_t UTF8CharList::c_str_size() const {
    size_t size = 0;
    UTF8Char * ptr = first;
    while (ptr) {
        size += strnlen(ptr->c_char(), 4);
        ptr = ptr->_next;
    } return size;
}

size_t UTF8CharList::c_index_at(const size_t index) const {
    size_t size = 0, subindex = 0;
    UTF8Char * ptr = first;
    while (ptr && subindex <= index) {
        size += strnlen(ptr->c_char(),4);
        ptr = ptr->_next;
        subindex++;
    } return size;
}

size_t UTF8CharList::c_index_before(const size_t index) const {
    size_t size = 0, subindex = 0;
    UTF8Char * ptr = first;
    while (ptr && subindex < index) {
        size += strnlen(ptr->c_char(),4);
        ptr = ptr->_next;
        subindex++;
    } return size;
}

bool UTF8CharList::empty() const {
    return first;
}

const char * UTF8CharList::operator[](size_t index) const {
    return char_at(index);
}

const char * UTF8CharList::char_at(size_t index) const {
    size_t subIndex = 0;
    UTF8Char * ptr = first;
    while (ptr) {
        if(subIndex == index) return ptr->c_char();
        subIndex++;
        ptr = ptr->_next;
    } return nullptr;
}

unsigned int UTF8CharList::charcode_at(size_t index) const {
    size_t subIndex = 0;
    UTF8Char * ptr = first;
    while (ptr) {
        if(subIndex == index) return ptr->get_charcode();
        subIndex++;
        ptr = ptr->_next;
    } return 0;
}

void UTF8CharList::remove(size_t index){
    size_t subIndex = 0;
    UTF8Char * ptr = first, * last = nullptr;
    if(ptr){
        while (ptr && subIndex < index) {
            subIndex++;
            last = ptr;
            ptr = ptr->_next;
        }
        
        if(subIndex == index) {
            if(ptr == first) {
                first = ptr->_next;
            } else if(last)
                last->_next = ptr->_next;
            
            delete ptr;
        }
#ifdef LANUTF_DEBUG_MODE
        else
            fprintf(stderr,"%s\n", ("[" + std::string(lanUTFVersion) + " Warning]: " + "invalid range [" + std::to_string(index) +"], unable to remove char.").c_str());
#endif
    }
}

void UTF8CharList::remove_last(){
    UTF8Char * ptr = first, * last = nullptr;
    if(ptr){
        while (ptr->_next) {
            last = ptr;
            ptr = ptr->_next;
        }
        
        if(ptr == first) {
            first = nullptr;
        } else {
            last->_next = nullptr;
        } delete ptr;
    }
}

char * UTF8CharList::alloc_c_str() const {
    UTF8Char * ptr = first;
    const size_t size = c_str_size() + 1;
    char * c_str = new char[size];
    bzero(c_str, size);
    while (ptr) {
        strcat(c_str, ptr->c_char());
        ptr = ptr->_next;
    } c_str[(size) ? size-1 : 0] = '\0';
    return c_str;
}

void UTF8CharList::composeCStr(char *str, size_t size) const {
    UTF8Char * ptr = first;
    size_t subIndex = 0;
    bzero(str, size);
    while (ptr && subIndex < size) {
        strcat(str, ptr->c_char());
        subIndex += strnlen(ptr->c_char(), 4);
        ptr = ptr->_next;
    }
}

void UTF8CharList::free_c_str(char * str){
    if(str) delete [] str;
}

size_t UTF8CharList::append(const char * src){
    UTF8Char * last_ptr = first ? first->_last() : nullptr;
    char src_cpy [strlen(src)], *src_cpy_ptr = src_cpy;
    strcpy(src_cpy, src);
    size_t append_len = 0;
    while (*src_cpy_ptr) {
        append_len++;
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
    } return append_len;
}

size_t UTF8CharList::append(std::list<const unsigned int> codes){
    UTF8Char * last_ptr = first ? first->_last() : nullptr;
    size_t append_len = 0;
    for(auto code : codes) {
        append_len++;
        if(last_ptr){
            last_ptr->_createNext();
            last_ptr=last_ptr->_next;
        } else {
            last_ptr = first = new UTF8Char;
        } last_ptr->composeUTF8Char(code);
    } return append_len;
}

size_t UTF8CharList::append(const char * src, size_t index) {
    UTF8Char * last_ptr = first->_at(index);
    char src_cpy [strlen(src)], *src_cpy_ptr = src_cpy;
    strcpy(src_cpy, src);
    size_t append_len = 0;
    if(last_ptr){
        while (*src_cpy_ptr) {
            append_len++;
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
#ifdef LANUTF_DEBUG_MODE
        fprintf(stderr,"%s\n", ("[" + std::string(lanUTFVersion) + " Warning]: " + "invalid range [" + std::to_string(index) +"], using default append(...) method.").c_str());
#endif
        return append(src);
    } return append_len;
}

size_t UTF8CharList::append(std::list<const unsigned int> codes, size_t index){
    UTF8Char * last_ptr = first->_at(index);
    size_t append_len = 0;
    if(last_ptr){
        for(auto code : codes) {
            append_len++;
            append_len++;
            last_ptr->_appendNext();
            last_ptr->_next->copy(*last_ptr);
            last_ptr->composeUTF8Char(code);
            last_ptr=last_ptr->_next;
        } return append_len;
    } else {
#ifdef LANUTF_DEBUG_MODE
        fprintf(stderr,"%s\n", ("[" + std::string(lanUTFVersion) + " Warning]: " + "invalid range [" + std::to_string(index) +"], using default append(...) method.").c_str());
#endif
        return append(codes);
    }
}

void UTF8CharList::clear(){
    if(first) {
        UTF8Char * ptr = first, * buff = nullptr;
        while (ptr) {
            buff = ptr->_next;
            delete ptr;
            ptr = buff;
        }
    } first = nullptr;
}

UTF8CharList::UTF8CharList(): first(nullptr){}

UTF8CharList::UTF8CharList(char * str): first(nullptr){
    append(str);
}

UTF8CharList::~UTF8CharList(){
    clear();
}
