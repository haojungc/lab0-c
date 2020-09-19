#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    for (list_ele_t *curr = q->head, *next; curr != NULL; curr = next) {
        next = curr->next;
        free(curr->value);
        free(curr);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));

    if (!newh)
        return false;

    int len = strlen(s);
    newh->value = malloc((len + 1) * sizeof(char));

    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, len + 1);

    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));

    if (!newh)
        return false;

    int len = strlen(s);
    newh->value = malloc((len + 1) * sizeof(char));

    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, len + 1);

    newh->next = NULL;
    if (q->size == 0)
        q->head = newh;
    else
        q->tail->next = newh;
    q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *target = q->head;
    q->head = q->head->next;

    /* Copies removed string to sp */
    if (sp) {
        int len = strlen(target->value);
        int max_len = len > bufsize - 1 ? bufsize - 1 : len;
        strncpy(sp, target->value, max_len);
        sp[max_len] = '\0';
    }

    q->size--;
    if (q->size == 0)
        q->tail = NULL;
    free(target->value);
    free(target);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *prev = NULL, *curr = q->tail = q->head;

    while (curr) {
        list_ele_t *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
}

/* Please reference to
 * https://www.geeksforgeeks.org/merge-two-sorted-linked-lists
 */
void insert_tail(list_ele_t **dest_ref, list_ele_t **source_ref)
{
    /* Points to the first element in the source list  */
    list_ele_t *new_node = *source_ref;

    /* Advances the source pointer */
    *source_ref = new_node->next;

    /* Links new_node to the first element in the destination list */
    new_node->next = *dest_ref;

    /* Lets new_node become the first element in the destination list */
    *dest_ref = new_node;
}

list_ele_t *merge_sorted_list(list_ele_t *a, list_ele_t *b)
{
    list_ele_t dummy; /* dummy.next points to the first element in the
                         destination list */
    list_ele_t *tail = &dummy;

    dummy.next = NULL;

    while (1) {
        if (a == NULL) {
            /* Appends the rest elements in b to the list */
            tail->next = b;
            break;
        } else if (b == NULL) {
            /* Appends the rest elements in a to the list */
            tail->next = a;
            break;
        }
        /* Inserts an element to the end of the list */
        if (strcmp(a->value, b->value) < 0) {
            insert_tail(&(tail->next), &a);
        } else {
            insert_tail(&(tail->next), &b);
        }

        tail = tail->next;
    }
    return dummy.next;
}

void front_back_split(list_ele_t *head,
                      list_ele_t **front_ref,
                      list_ele_t **back_ref)
{
    list_ele_t *slow = head;
    list_ele_t *fast = head->next;

    /* Finds the middle element (slow) in the list */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front_ref = head;
    *back_ref = slow->next;
    slow->next = NULL;
}

void merge_sort(list_ele_t **head)
{
    /* Returns if there are less than two elements */
    if (*head == NULL || (*head)->next == NULL)
        return;

    list_ele_t *a;
    list_ele_t *b;

    front_back_split(*head, &a, &b);

    merge_sort(&a); /* Merges the left part of the list starting from a */
    merge_sort(&b); /* Merges the right part of the list starting from b */

    *head = merge_sorted_list(a, b);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return;

    merge_sort(&q->head);

    // O(n) update for tail
    // Not a good way! But just leave this here
    list_ele_t *tmp = q->head;
    while (tmp->next != NULL)
        tmp = tmp->next;

    q->tail = tmp;
}
