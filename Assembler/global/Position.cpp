#include "Position.hpp"
#include <string>

using namespace std;

Position::Position(unsigned int line, unsigned int column) : line(line), column(column) {}

Position::Position(const Position& p) : line(p.line), column(p.column) {}

Position::~Position() {  }

string Position::toString() const {
    return "Line " + std::to_string(line) + ", column " + std::to_string(column);
}

DefaultPosition::DefaultPosition() : Position(DEFAULT_LINE, DEFAULT_COLUMN) {}
