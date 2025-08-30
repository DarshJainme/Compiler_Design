#ifndef TOKENS_H
#define TOKENS_H
#include "y.tab.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Provide a helper to convert token values to readable strings.
   We use a switch over token symbols (names) so we don't rely on numeric order. */
static inline const char* token_to_string(int token) {
    switch(token) {
        /* Keywords */
        case KEYWORD_INT:            return "KEYWORD_INT";
        case KEYWORD_CHAR:           return "KEYWORD_CHAR";
        case KEYWORD_FLOAT:          return "KEYWORD_FLOAT";
        case KEYWORD_DOUBLE:         return "KEYWORD_DOUBLE";
        case KEYWORD_VOID:           return "KEYWORD_VOID";
        case KEYWORD_IF:             return "KEYWORD_IF";
        case KEYWORD_ELSE:           return "KEYWORD_ELSE";
        case KEYWORD_FOR:            return "KEYWORD_FOR";
        case KEYWORD_WHILE:          return "KEYWORD_WHILE";
        case KEYWORD_RETURN:         return "KEYWORD_RETURN";
        case KEYWORD_BREAK:          return "KEYWORD_BREAK";
        case KEYWORD_CONTINUE:       return "KEYWORD_CONTINUE";
        case KEYWORD_DO:             return "KEYWORD_DO";
        case KEYWORD_SWITCH:         return "KEYWORD_SWITCH";
        case KEYWORD_CASE:           return "KEYWORD_CASE";
        case KEYWORD_DEFAULT:        return "KEYWORD_DEFAULT";
        case KEYWORD_GOTO:           return "KEYWORD_GOTO";
        case KEYWORD_ENUM:           return "KEYWORD_ENUM";
        case KEYWORD_STRUCT:         return "KEYWORD_STRUCT";
        case KEYWORD_UNION:          return "KEYWORD_UNION";
        case KEYWORD_TYPEDEF:        return "KEYWORD_TYPEDEF";
        case KEYWORD_STATIC:         return "KEYWORD_STATIC";
        case KEYWORD_AUTO:           return "KEYWORD_AUTO";
        case KEYWORD_CONST:          return "KEYWORD_CONST";
        case KEYWORD_SIZEOF:         return "KEYWORD_SIZEOF";
        case KEYWORD_CLASS:          return "KEYWORD_CLASS";
        case KEYWORD_PUBLIC:         return "KEYWORD_PUBLIC";
        case KEYWORD_PRIVATE:        return "KEYWORD_PRIVATE";
        case KEYWORD_PROTECTED:      return "KEYWORD_PROTECTED";
        case KEYWORD_NEW:            return "KEYWORD_NEW";
        case KEYWORD_DELETE:         return "KEYWORD_DELETE";
        case KEYWORD_USING:          return "KEYWORD_USING";
        case KEYWORD_NAMESPACE:      return "KEYWORD_NAMESPACE";
        case KEYWORD_BOOL:           return "KEYWORD_BOOL";
        case KEYWORD_LONG:           return "KEYWORD_LONG";
        case KEYWORD_TRUE:           return "KEYWORD_TRUE";
        case KEYWORD_FALSE:          return "KEYWORD_FALSE";

        /* Identifiers & literals */
        case IDENTIFIER:            return "IDENTIFIER";
        case INTEGER_LITERAL:       return "INTEGER_LITERAL";
        case FLOAT_LITERAL:         return "FLOAT_LITERAL";
        case CHAR_LITERAL:          return "CHAR_LITERAL";
        case STRING_LITERAL:        return "STRING_LITERAL";

        /* Operators */
        case OP_PLUS:               return "OP_PLUS";
        case OP_MINUS:              return "OP_MINUS";
        case OP_MULTIPLY:           return "OP_MULTIPLY";
        case OP_DIVIDE:             return "OP_DIVIDE";
        case OP_MODULO:             return "OP_MODULO";
        case OP_INCREMENT:          return "OP_INCREMENT";
        case OP_DECREMENT:          return "OP_DECREMENT";
        case OP_ASSIGN:             return "OP_ASSIGN";
        case OP_ADD_ASSIGN:         return "OP_ADD_ASSIGN";
        case OP_SUB_ASSIGN:         return "OP_SUB_ASSIGN";
        case OP_MUL_ASSIGN:         return "OP_MUL_ASSIGN";
        case OP_DIV_ASSIGN:         return "OP_DIV_ASSIGN";
        case OP_MOD_ASSIGN:         return "OP_MOD_ASSIGN";
        case OP_LSHIFT:             return "OP_LSHIFT";
        case OP_RSHIFT:             return "OP_RSHIFT";
        case OP_AND:                return "OP_AND";
        case OP_OR:                 return "OP_OR";
        case OP_XOR:                return "OP_XOR";
        case OP_NOT:                return "OP_NOT";
        case OP_EQUAL:              return "OP_EQUAL";
        case OP_NOT_EQUAL:          return "OP_NOT_EQUAL";
        case OP_LESS_THAN:          return "OP_LESS_THAN";
        case OP_GREATER_THAN:       return "OP_GREATER_THAN";
        case OP_LESS_THAN_EQUAL:    return "OP_LESS_THAN_EQUAL";
        case OP_GREATER_THAN_EQUAL: return "OP_GREATER_THAN_EQUAL";
        case OP_LOGICAL_AND:        return "OP_LOGICAL_AND";
        case OP_LOGICAL_OR:         return "OP_LOGICAL_OR";
        case OP_LOGICAL_NOT:        return "OP_LOGICAL_NOT";
        case OP_BITWISE_AND:        return "OP_BITWISE_AND";
        case OP_BITWISE_OR:         return "OP_BITWISE_OR";
        case OP_BITWISE_XOR:        return "OP_BITWISE_XOR";
        case OP_BITWISE_NOT:        return "OP_BITWISE_NOT";

        /* Punctuation */
        case SEMICOLON:             return "SEMICOLON";
        case COMMA:                 return "COMMA";
        case OPEN_PARENTHESIS:      return "OPEN_PARENTHESIS";
        case CLOSE_PARENTHESIS:     return "CLOSE_PARENTHESIS";
        case OPEN_BRACKET:          return "OPEN_BRACKET";
        case CLOSE_BRACKET:         return "CLOSE_BRACKET";
        case OPEN_CURLY_BRACKET:    return "OPEN_CURLY_BRACKET";
        case CLOSE_CURLY_BRACKET:   return "CLOSE_CURLY_BRACKET";
        case COLON:                 return "COLON";
        case POINTER_ARROW:         return "POINTER_ARROW";
        case DOT:                   return "DOT";
        case REFERENCE_AMPERSAND:   return "REFERENCE_AMPERSAND";
        case POINTER_STAR:          return "POINTER_STAR";
        case QUESTION_MARK:         return "QUESTION_MARK";

        /* Preprocessor / comments / whitespace / errors */
        case PREPROCESSOR_DIRECTIVE: return "PREPROCESSOR_DIRECTIVE";
        case SINGLE_LINE_COMMENT:    return "SINGLE_LINE_COMMENT";
        case MULTI_LINE_COMMENT:     return "MULTI_LINE_COMMENT";
        case NEWLINE:                return "NEWLINE";
        case WHITESPACE:             return "WHITESPACE";
        case ERROR_TOKEN:            return "ERROR_TOKEN";
        case UNKNOWN:                return "UNKNOWN";
        case END_OF_FILE:            return "END_OF_FILE";

        default:
            return "UNKNOWN";
    }
}

#ifdef __cplusplus
}
#endif

#endif