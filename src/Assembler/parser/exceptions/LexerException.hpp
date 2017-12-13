#pragma once

#include <iostream>

class LexerException {
public:
    LexerException(const std::string& file_name) throw() : file_name(file_name) {}
    LexerException(const std::string& file_name, unsigned int line, unsigned int column) : file_name(file_name), line(line), column(column) {}
    LexerException(const std::string& file_name, unsigned int line, unsigned int column, std::string& msg) : file_name(file_name), line(line), column(column), msg(msg) {}
    virtual std::string toString() const {
        return "[!] Error in file " + file_name + ".";
    }
protected:
    std::string file_name;
    std::string msg;
    unsigned int line;
    unsigned int column;
};

class FileNotFoundException : public LexerException {
public:
    FileNotFoundException(const std::string& file_name) throw() : LexerException(file_name) {}
    std::string toString() const {
        return "[!] Error : " + file_name + " not found.";
    }
};

class OutOfRangeException : public LexerException {
public:
    OutOfRangeException(const std::string& file_name) throw () : LexerException(file_name) {}
    std::string toString() const {
        return "[!] Fatal error : out of range !";
    }
};
