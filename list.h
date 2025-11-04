/*
 * A generic linked list implementation to help manage
 * descriptors in the MIPS code generator.
 */
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
    int count;
} List;

List* list_create();
void list_destroy(List* list); // Does not free data
void list_clear(List* list); // Does not free data
void list_clear_and_free_data(List* list); // Frees data

void list_append(List* list, void* data);
void list_prepend(List* list, void* data);
bool list_remove(List* list, void* data); // Returns true if found and removed
bool list_contains(List* list, void* data);
bool list_is_empty(List* list);

// Iterator-style functions
ListNode* list_begin(List* list);
ListNode* list_next(ListNode* node);
void* list_get_data(ListNode* node);

#endif // LIST_H