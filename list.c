/*
 * Implementation of the generic linked list.
 */
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

List* list_create() {
    List* list = (List*)calloc(1, sizeof(List));
    if (!list) {
        fprintf(stderr, "Fatal: Could not allocate memory for List.\n");
        exit(1);
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

void list_destroy(List* list) {
    if (!list) return;
    list_clear(list);
    free(list);
}

void list_clear(List* list) {
    if (!list) return;
    ListNode* current = list->head;
    while (current) {
        ListNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

void list_clear_and_free_data(List* list) {
    if (!list) return;
    ListNode* current = list->head;
    while (current) {
        ListNode* next = current->next;
        if (current->data) {
            free(current->data);
        }
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

void list_append(List* list, void* data) {
    if (!list) return;
    ListNode* node = (ListNode*)calloc(1, sizeof(ListNode));
    if (!node) {
        fprintf(stderr, "Fatal: Could not allocate memory for ListNode.\n");
        exit(1);
    }
    node->data = data;
    node->next = NULL;

    if (list->tail) {
        list->tail->next = node;
        list->tail = node;
    } else {
        list->head = node;
        list->tail = node;
    }
    list->count++;
}

void list_prepend(List* list, void* data) {
    if (!list) return;
    ListNode* node = (ListNode*)calloc(1, sizeof(ListNode));
    if (!node) {
        fprintf(stderr, "Fatal: Could not allocate memory for ListNode.\n");
        exit(1);
    }
    node->data = data;
    node->next = list->head;

    list->head = node;
    if (!list->tail) {
        list->tail = node;
    }
    list->count++;
}

bool list_remove(List* list, void* data) {
    if (!list || !list->head) return false;

    ListNode* current = list->head;
    ListNode* prev = NULL;

    while (current) {
        if (current->data == data) {
            if (prev) {
                prev->next = current->next;
            } else {
                list->head = current->next;
            }

            if (current == list->tail) {
                list->tail = prev;
            }
            
            free(current);
            list->count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

bool list_contains(List* list, void* data) {
    if (!list) return false;
    for (ListNode* n = list->head; n; n = n->next) {
        if (n->data == data) {
            return true;
        }
    }
    return false;
}

bool list_is_empty(List* list) {
    return (list == NULL) || (list->count == 0);
}

ListNode* list_begin(List* list) {
    if (!list) return NULL;
    return list->head;
}

ListNode* list_next(ListNode* node) {
    if (!node) return NULL;
    return node->next;
}

void* list_get_data(ListNode* node) {
    if (!node) return NULL;
    return node->data;
}