#include "Lexer.hpp"

using namespace std;

Lexer::Lexer(string file_name) {
    this->file_name = file_name;
    this->current_loc.line = 1;
    this->current_loc.column = 1;
    this->eof = false;
    this->new_line = true;
    fopen_s(&this->file, file_name.c_str(), "r");
    this->current_index = 0;
    this->comments_enabled = true;

    if (!this->file) {
        throw FileNotFoundException(file_name);
    }

    Global::getInstance()->setFile(file_name);
}

Lexer::~Lexer() {
    if (this->file) {
        fclose(this->file);
    }
}

bool Lexer::isEof() const {
    return this->eof;
}

void Lexer::setKeys(vector<string> keys) {
    this->keys = keys;
}

void Lexer::setSkips(vector<string> skips) {
    for (auto & it : skips) {
        this->skips.insert(pair<string, bool>(it, true));
    }
}

void Lexer::setComs(vector<pair<string, string> > coms) {
    this->coms = coms;
}

bool Lexer::isSkip(TokenPtr t) const {
    auto it = this->skips.find(t->value);
    if (it != this->skips.end()) {
        if (it->second)
            return true;
        return false;
    }
    return false;
}

void Lexer::setSkipEnabled(const string & skip, bool value) {
    auto it = this->skips.find(skip);
    if (it != this->skips.end())
        it->second = value;
}

void Lexer::setCommentsEnabled(bool value) {
    this->comments_enabled = value;
}

TokenPtr Lexer::isCom(TokenPtr t) const {
    for (auto it : this->coms) {
        if (it.first == t->value) {
            return Token::make(it.second, { t->line, t->column });
        }
    }
    return Token::makeEof(this->current_loc);
}

TokenPtr Lexer::getWord(int index) {
    if (index >= 0) {
        if (index == this->current_line.size()) {
            this->current_line.push_back(getWord());
        }
        return this->current_line[index];
    }
    else {
        throw OutOfRangeException(this->file_name);
    }
}

void Lexer::rewind(int count) {
    while (count > 0 && this->current_index > 0) {
        this->current_index--;
        if (!this->isSkip(this->getWord(this->current_index))) {
            count--;
        }
    }
}


TokenPtr Lexer::next() {
    TokenPtr t = this->getWord(this->current_index++);
    TokenPtr com = isCom(t);
    while (isSkip(t) || !com->isEof()) {
        if (!com->isEof() && this->comments_enabled) {
            do {
                t = this->getWord(this->current_index++);
            } while (t->value != com->value && !t->isEof());
            com = Token::makeEof(this->current_loc);
            if (!t->isEof())
                t = this->getWord(this->current_index++);
        }
        else {
            t = this->getWord(this->current_index++);
        }
        com = isCom(t);
    }
    return t;
}

TokenPtr Lexer::getWord() {
    int current_pos = ftell(this->file);
    string line;
    this->getLine(line);

    if (line.size() == 0) {
        this->eof = true;
        return Token::makeEof(this->current_loc);
    }
    else {
        if (this->new_line) {
            Global::getInstance()->addLine(line);
            this->new_line = false;
        }
    }

    int max = 0, pos = line.size(), index = -1;
    string tok = "";
    for (auto it : this->keys) {
        index = line.find(it);
        if (index > -1) {
            if (index < pos) {
                pos = index;
                max = it.size();
                tok = it;
            }
            else if (index == pos) {
                if (max < it.size()) {
                    max = it.size();
                    tok = it;
                }
            }
        }
    }

    location_t loc;
    loc.line = this->current_loc.line;
    loc.column = this->current_loc.column;

    if (pos == line.size()) {
        mfseek(line, current_pos + line.size());
        this->current_loc.column += line.size();
        return Token::make(line, loc);
    }
    else if (pos == 0) {
        mfseek(tok, current_pos + tok.size());
        this->current_loc.column += tok.size();
        return Token::make(tok, loc);
    }
    else {
        string str = line.substr(0, pos);
        mfseek(str, current_pos + pos);
        this->current_loc.column += pos;
        return Token::make(str, loc);
    }
}

void Lexer::getLine(string & line) {
    unsigned int size = 256;
    line = "";
    char * buffer = NULL;
    while (1) {
        buffer = new char[size]; buffer[0] = '\0'; // Valgrind needs this fucking initialisation...
        if (buffer) {
            fgets(buffer, size, this->file);
            if (!buffer) {
                line = "";
                break;
            }
            line += string(buffer);
            if (line.size() < size || line[line.size() - 1] != '\n')
                break;
            delete[] buffer;
            buffer = NULL;
        }
    }
    if (buffer)
        delete[] buffer;
}

void Lexer::mfseek(const string & tok, unsigned int offset) {
    if (tok == "\n" || tok == "\r") {
        this->current_loc.line++;
        this->current_loc.column = 0;
        this->new_line = true;
    }
    fseek(this->file, offset, SEEK_SET);
}
