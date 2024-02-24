#include "Tokenizer.h"
#include <iostream>
#include <sstream>

Tokenizer::Tokenizer(const std::vector<char>& file) : _file(file), _size(file.size())
{
}

bool Tokenizer::isOperator(Token t) {

    std::string value = t.value();

    if (value == "+" || value == "-" || value == "*" || value == "/" || value == "%" || value == "^")
        return true;
    return false;
}

void Tokenizer::tokenizeVector() {
    _currentPos = 0;
    _currentState = START;
    while (_currentPos < _size) {
        Token token = getToken(); // Gonna get the next token
        if (token.type() != Token::Type::Unknown) { // Skiping unknown tokens for now
            _tokens.push_back(token);
        }
    }
}

Token Tokenizer::getToken() {
    std::string tokenValue;
    Token::Type tokenType = Token::Type::Unknown;
    bool tokenFound = false;

    while (_currentPos < _size && !tokenFound) {
        char currentChar = _file[_currentPos];
        switch (_currentState) {
        case START:
            if (currentChar == '\n') {
                _lineNum++;
            }
            else if (std::isspace(currentChar)) {
                // Ignore whitespace
            }
            else if (std::isalpha(currentChar)) {
                tokenType = Token::Type::Identifier;
                _currentState = IDENTIFIER;
                tokenValue += currentChar;
            }
            else if (std::isdigit(currentChar)) {
                tokenType = Token::Type::Integer; // I think needs to be updated cause a sign could come before (so for ints)
                _currentState = INTEGER;
                tokenValue += currentChar;
            }
            else {
                // Handling for single char tokens (ops, punctuation, ect...)
                switch (currentChar) {
                case '(':
                    tokenType = Token::Type::LParen;
                    tokenValue = currentChar;
                    tokenFound = true;
                    break;
                case ')':
                    tokenType = Token::Type::RParen;
                    tokenValue = currentChar;
                    tokenFound = true;
                    break;
                    // Add cases for other single-character tokens here
                case '+':
                    // check if last token was an op
                    if (isOperator(_tokens.back()) || _tokens.back().value() == "(") {
                        // make char part of int
                        tokenType = Token::Type::Integer;
                        tokenValue += currentChar;
                    }
                    else {
                        tokenType = Token::Type::Plus;
                        tokenValue = currentChar;
                        tokenFound = true;
                    }
                    break;
                case '-':
                    // check if last token was an op
                    if (isOperator(_tokens.back()) || _tokens.back().value() == "(") {
                        // make char part of int
                        tokenType = Token::Type::Integer;
                        tokenValue += currentChar;
                    }
                    else {
                        tokenType = Token::Type::Minus;
                        tokenValue = currentChar;
                        tokenFound = true;
                    }
                    break;
                case '"':
                    tokenType = Token::Type::DoubleQuote;
                    tokenValue = currentChar;
                    tokenFound = true;
                    _currentState = DQ_STRING;
                    break;
                case '\'':
                    tokenType = Token::Type::SingleQuote;
                    tokenValue = currentChar;
                    tokenFound = true;
                    _currentState = SQ_STRING;
                    break;
                default:
                    tokenType = Token::Type::Unknown;
                    tokenValue = currentChar;
                    tokenFound = true;
                    break;
                }
            }
            break;
        case IDENTIFIER:
            if (std::isalnum(currentChar) || currentChar == '_') {
                tokenValue += currentChar;
            }
            else {
                _currentState = START; // End of identifier (Which is just a string realle)
                tokenFound = true;
                --_currentPos; // Re-evaluate the character in the next state
            }
            break;
        case INTEGER:
            if (std::isdigit(currentChar)) { // Keep appending as long as its a number
                tokenValue += currentChar;
            }
            else {
                _currentState = START; // Ending number
                tokenFound = true;
                --_currentPos; // Re-evaluate this character in the next state
            }
            break;
        case DQ_STRING:
            if (currentChar != '"') {
                tokenValue += currentChar;
            }
            else {
                _currentState = DQ_END; // so next token can be read as end of string.
                tokenType = Token::Type::String;    // can combine to token: 'DOUBLE_QUOTED_STRING' but assignment just labels strings
                tokenFound = true;
                --_currentPos;   // Re-evaluate this character in the next state
            }
            break;
        case DQ_END:
            // Error for not finishing quote
            if (currentChar != '"') {
                std::cerr << "Syntax error on line " << _lineNum << ": incomplete quote\n";
                exit(1);
            }

            _currentState = START; 
            tokenType = Token::Type::DoubleQuote;
            tokenValue = currentChar;
            tokenFound = true;
            break;

        case SQ_STRING:
            if (currentChar != '\'') {
                tokenValue += currentChar;
            }
            else {
                _currentState = DQ_END; // so next token can be read as end of string.
                tokenType = Token::Type::String;    // can combine to token: 'SINGLE_QUOTED_STRING' but assignment just labels strings
                tokenFound = true;
                --_currentPos;   // Re-evaluate this character in the next state
            }
            break;

        case SQ_END:
            // Error for not finishing quote
            if (currentChar != '\'') {
                std::cerr << "Syntax error on line " << _lineNum << ": incomplete quote\n";
                exit(1);
            }

            _currentState = START;
            tokenType = Token::Type::SingleQuote;
            tokenValue = currentChar;
            tokenFound = true;
            break;

        }
        ++_currentPos;
    }

    if (tokenFound || !tokenValue.empty()) {
        return Token(tokenType, tokenValue);
    }

    return Token(Token::Type::Unknown, "");
}

// Method to return the tokens vector
std::vector<Token> Tokenizer::getTokens() const {
    return _tokens;
}