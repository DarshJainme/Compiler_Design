#ifndef ST_H
#define ST_H

/* This struct defines an entry in our simple symbol table */
typedef struct TokenEntry {
    char *token;
    char *token_type;
    struct TokenEntry *next;
} TokenEntry;

/* Global head of the symbol table list */
extern TokenEntry *token_list;

/* Function prototypes */
void add_token(char *token, char *type);
void print_tokens();
struct TokenEntry* reverse_list(struct TokenEntry* head);
// Add this to symbol_table.h
void add_typename(const char* name);
int is_typename(const char* name);


#endif /* ST_H */
