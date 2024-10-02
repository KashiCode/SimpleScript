#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------------------------------------
// ---------------          PARSER           -------------------
// -responsible for understand the structure and meaning of code-
// -------------------------------------------------------------


// Token definitions
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

// AST node types
typedef enum {
    NODE_ASSIGNMENT,
    NODE_OUTPUT,
    NODE_COMMENT
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char variable[100];
    char value[100];
    struct ASTNode *next;
} ASTNode;

//Tokenise function from the lexer source code. 
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
    tokens[tokenCount].type = TOKEN_END;
}


// Parser structure. 
typedef struct {
    Token *tokens;
    int currentToken;
} Parser;

Token* getCurrentToken(Parser *parser) {
    return &parser->tokens[parser->currentToken];
}

void advanceToken(Parser *parser) {
    parser->currentToken++;
}

int matchToken(Parser *parser, TokenType type) {
    return getCurrentToken(parser)->type == type;
}

// Function to create a new AST node
ASTNode* createASTNode(NodeType type) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->next = NULL;
    return node;
}

// Function to parse assignment statements
// TODO: add type checking here. 

ASTNode* parseAssignment(Parser *parser) {
    ASTNode *node = createASTNode(NODE_ASSIGNMENT);
    strcpy(node->variable, getCurrentToken(parser)->lexeme);
    advanceToken(parser);

    if (matchToken(parser, TOKEN_TYPE)) {
        advanceToken(parser);
    }

    if (matchToken(parser, TOKEN_ASSIGNMENT)) {
        advanceToken(parser);
    }

    if (matchToken(parser, TOKEN_STRING) || matchToken(parser, TOKEN_INT)) {
        strcpy(node->value, getCurrentToken(parser)->lexeme);
        advanceToken(parser);
    }

    return node;
}

// Function to parse output statements
ASTNode* parseOutput(Parser *parser) {
    ASTNode *node = createASTNode(NODE_OUTPUT);
    advanceToken(parser); // Skip "Output"

    if (matchToken(parser, TOKEN_OPEN_BRACE)) {
        advanceToken(parser);
    }

    // Parse values inside the braces
    while (!matchToken(parser, TOKEN_CLOSE_BRACE) && !matchToken(parser, TOKEN_END)) {
        if (matchToken(parser, TOKEN_STRING) || matchToken(parser, TOKEN_VARIABLE) || matchToken(parser, TOKEN_INT)) {
            strcat(node->value, getCurrentToken(parser)->lexeme);
            strcat(node->value, " ");
            advanceToken(parser);
        }

        if (matchToken(parser, TOKEN_COMMA)) {
            advanceToken(parser);
        }
    }

    if (matchToken(parser, TOKEN_CLOSE_BRACE)) {
        advanceToken(parser);
    }

    return node;
}

// Function to parse comments.
ASTNode* parseComment(Parser *parser) {
    ASTNode *node = createASTNode(NODE_COMMENT);
    strcpy(node->value, getCurrentToken(parser)->lexeme + 1);
    advanceToken(parser);
    return node;
}

// Function to parse the entire token stream
ASTNode* parse(Parser *parser) {
    ASTNode *head = NULL;
    ASTNode *current = NULL;

    while (!matchToken(parser, TOKEN_END)) {
        ASTNode *node = NULL;

        if (matchToken(parser, TOKEN_VARIABLE)) {
            node = parseAssignment(parser);
        } else if (matchToken(parser, TOKEN_OUTPUT)) {
            node = parseOutput(parser);
        } else if (matchToken(parser, TOKEN_COMMENT)) {
            node = parseComment(parser);
        } else {
            advanceToken(parser); //Error checking to skip unknown tokens. 
        }

        if (node) {
            if (head == NULL) {
                head = node;
            } else {
                current->next = node;
            }
            current = node;
        }
    }

    return head;
}

// Function to print the AST
void printAST(ASTNode *node) {
    while (node) {
        switch (node->type) {
            case NODE_ASSIGNMENT:
                printf("Assignment: Variable = %s, Value = %s\n", node->variable, node->value);
                break;
            case NODE_OUTPUT:
                printf("Output: Values = %s\n", node->value);
                break;
            case NODE_COMMENT:
                printf("Comment: %s\n", node->value);
                break;
            default:
                printf("Unknown Node\n");
        }
        node = node->next;
    }
}

// The example code. 
int main() {
    char code[] = "Value1(Int): 100\n"
                  "Value2(String): 'Hello'\n"
                  "Output{Value1}\n"
                  "Output{Value1,Value2}\n"
                  "Output{'this is a test'}\n"
                  "@ This line is a comment\n"
                  "Output{'enter your age'}";

    Token tokens[100];
    tokenize(code, tokens, 100);

    Parser parser = {tokens, 0};
    ASTNode *ast = parse(&parser);

    printAST(ast);

    return 0;
}
