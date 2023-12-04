#include <stdio.h>
#include <string.h>
#include <ctype.h>

// -----------------------------------------------------------
// ---------------          LEXER          -------------------
// ---  Responsible for producing tokens from the source   ---
// -----------------------------------------------------------




typedef enum {
    TOKEN_VARIABLE,
    TOKEN_TYPE,
    TOKEN_ASSIGNMENT,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_COMMENT,
    TOKEN_OUTPUT,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_COMMA,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
} Token;

void tokenize(char *source, Token *tokens, int maxTokens) {
    int tokenCount = 0;
    char *tokenStart;

    while (*source != '\0' && tokenCount < maxTokens) {
        if (isspace(*source)) { // Skip whitespace
            source++;
        } else if (strncmp(source, "Output", 6) == 0 && (isspace(*(source + 6)) || *(source + 6) == '{')) { // Output keyword
            strcpy(tokens[tokenCount].lexeme, "Output");
            tokens[tokenCount].type = TOKEN_OUTPUT;
            source += 6; // Move past "Output"
            tokenCount++;
        } else if (*source == '{') {
            strcpy(tokens[tokenCount].lexeme, "{");
            tokens[tokenCount].type = TOKEN_OPEN_BRACE;
            source++;
            tokenCount++;
            // Handle contents within braces
            while (*source != '}' && *source != '\0') {
                if (*source == '\'') { // String literal
                    tokenStart = ++source;
                    while (*source != '\'' && *source != '\0') source++;
                    strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
                    tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
                    tokens[tokenCount].type = TOKEN_STRING;
                    tokenCount++;
                    if (*source == '\'') source++;
                } else if (isdigit(*source)) { // Integer literal
                    tokenStart = source;
                    while (isdigit(*source)) source++;
                    strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
                    tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
                    tokens[tokenCount].type = TOKEN_INT;
                    tokenCount++;
                } else if (isalnum(*source) || *source == '_') { // Variable name
                    tokenStart = source;
                    while (isalnum(*source) || *source == '_') source++;
                    strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
                    tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
                    tokens[tokenCount].type = TOKEN_VARIABLE;
                    tokenCount++;
                } else if (*source == ',') { // Comma separator
                    source++; // Skip the comma
                } else {
                    source++; // Skip any unrecognized characters
                }
            }
            if (*source == '}') {
                strcpy(tokens[tokenCount].lexeme, "}");
                tokens[tokenCount].type = TOKEN_CLOSE_BRACE;
                source++;
                tokenCount++;
            }
        } else if (*source == ':') { // Assignment Operator
            tokens[tokenCount].lexeme[0] = ':';
            tokens[tokenCount].lexeme[1] = '\0';
            tokens[tokenCount].type = TOKEN_ASSIGNMENT;
            source++;
            tokenCount++;
        } else if (*source == '(') { // Type declaration
            source++; // Skip '('
            tokenStart = source;
            while (*source != ')' && *source != '\0') source++;
            strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
            tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
            tokens[tokenCount].type = TOKEN_TYPE;
            tokenCount++;
            if (*source == ')') source++; // Skip ')'
        } else if (isdigit(*source)) { // Integer literal
            tokenStart = source;
            while (isdigit(*source)) source++;
            strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
            tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
            tokens[tokenCount].type = TOKEN_INT;
            tokenCount++;
        } else if (isalnum(*source) || *source == '_') { // Variable name
            tokenStart = source;
            while (isalnum(*source) || *source == '_') source++;
            strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
            tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
            tokens[tokenCount].type = TOKEN_VARIABLE;
            tokenCount++;
        } else if (*source == '\'') { // String literal
            tokenStart = ++source;
            while (*source != '\'' && *source != '\0') source++;
            strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
            tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
            tokens[tokenCount].type = TOKEN_STRING;
            tokenCount++;
            if (*source == '\'') source++;
        } else if (*source == '@') { // Comment
            tokenStart = source;
            while (*source != '\n' && *source != '\0') source++;
            strncpy(tokens[tokenCount].lexeme, tokenStart, source - tokenStart);
            tokens[tokenCount].lexeme[source - tokenStart] = '\0'; // Null terminate the string
            tokens[tokenCount].type = TOKEN_COMMENT;
            tokenCount++;
            if (*source == '\n') source++;
        } else {
            source++; // Skip unrecognized characters
        }
    }
    tokens[tokenCount].type = TOKEN_END; // Mark the end of the token array
}


int main() {
  char code[] = "Value1(Int): 100\n"
    "Value2(String): 'Hello'\n"
    "\n"
    "Output{Value1}\n"
    "Output{Value1,Value2}\n"
    "Output{'this is a test'}\n"
    "@ comment\n"
    "Output{'enter your age'}";

    Token tokens[100];
    tokenize(code, tokens, 100);

    for (int i = 0; tokens[i].type != TOKEN_END; i++) {
        printf("Token: %s, Type: %d\n", tokens[i].lexeme, tokens[i].type);
    }

    return 0;
}