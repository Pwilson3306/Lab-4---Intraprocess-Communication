#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_t *list_alloc(void) {
    list_t *list = malloc(sizeof(list_t));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void list_free(list_t *list) {
    if (!list) return;
    node_t *current = list->head;
    while (current) {
        node_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int list_length(list_t *list) {
    return list ? list->length : 0;
}

void list_add_to_front(list_t *list, int value) {
    if (!list) return;
    node_t *node = malloc(sizeof(node_t));
    if (!node) return;
    node->value = value;
    node->next = list->head;
    list->head = node;
    if (!list->tail) list->tail = node;
    list->length++;
}

void list_add_to_back(list_t *list, int value) {
    if (!list) return;
    node_t *node = malloc(sizeof(node_t));
    if (!node) return;
    node->value = value;
    node->next = NULL;
    if (!list->tail) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->length++;
}

int list_add_at_index(list_t *list, int index, int value) {
    if (!list || index < 0 || index > list->length) return -1;
    if (index == 0) {
        list_add_to_front(list, value);
        return 0;
    }
    if (index == list->length) {
        list_add_to_back(list, value);
        return 0;
    }
    node_t *previous = list->head;
    for (int i = 0; i < index - 1; i++) previous = previous->next;
    node_t *node = malloc(sizeof(node_t));
    if (!node) return -1;
    node->value = value;
    node->next = previous->next;
    previous->next = node;
    list->length++;
    return 0;
}

int list_remove_at_index(list_t *list, int index) {
    if (!list || index < 0 || index >= list->length) return -1;
    node_t *removed;
    if (index == 0) {
        removed = list->head;
        list->head = removed->next;
        if (!list->head) list->tail = NULL;
    } else {
        node_t *previous = list->head;
        for (int i = 0; i < index - 1; i++) previous = previous->next;
        removed = previous->next;
        previous->next = removed->next;
        if (removed == list->tail) list->tail = previous;
    }
    int value = removed->value;
    free(removed);
    list->length--;
    return value;
}

int list_remove_from_front(list_t *list) {
    return list_remove_at_index(list, 0);
}

int list_remove_from_back(list_t *list) {
    if (!list || list->length == 0) return -1;
    return list_remove_at_index(list, list->length - 1);
}

int list_get_elem_at(list_t *list, int index) {
    if (!list || index < 0 || index >= list->length) return -1;
    node_t *current = list->head;
    for (int i = 0; i < index; i++) current = current->next;
    return current->value;
}

char *listToString(list_t *list) {
    static char buffer[4096];
    size_t used = (size_t)snprintf(buffer, sizeof(buffer), "[");
    node_t *current = list ? list->head : NULL;

    while (current && used < sizeof(buffer) - 2) {
        int n = snprintf(buffer + used, sizeof(buffer) - used,
                         "%s%d", current == list->head ? "" : ", ",
                         current->value);
        if (n < 0) break;
        used += (size_t)n;
        current = current->next;
    }

    if (used < sizeof(buffer) - 1) {
        snprintf(buffer + used, sizeof(buffer) - used, "]");
    } else {
        buffer[sizeof(buffer) - 2] = ']';
        buffer[sizeof(buffer) - 1] = '\0';
    }
    return buffer;
}
