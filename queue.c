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

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* Assures there are more than one element in the queue */
    if (!q || !q->head)
        return;

    for (list_ele_t **front = &q->head; (*front)->next != NULL;
         front = &(*front)->next) {
        list_ele_t **prev_of_min = front;
        /* Finds the previous element of minimum element in the list */
        for (list_ele_t **ele = &(*front)->next; *ele != NULL;
             ele = &(*ele)->next) {
            bool is_smaller =
                strnatcasecmp((*ele)->value, (*prev_of_min)->value) < 0;
            if (is_smaller)
                prev_of_min = ele;
        }
        if (prev_of_min != front) {
            list_ele_t *min = *prev_of_min;
            list_ele_t *next = min->next;

            if (min->next == NULL)
                q->tail = *front;

            /* Adjacent elements */
            if ((*front)->next == *prev_of_min) {
                min->next = *front;
                (*front)->next = next;
            } else {
                min->next = (*front)->next;
                (*front)->next = next;
                *prev_of_min = *front;
            }
            *front = min;
        }
    }
}

/* Case insensitive string comparisons using a "natural order" algorithm */
int strnatcasecmp(const char *s1, const char *s2)
{
    /* Converts the first letters to lowercase */
    char c1 = s1[0] | ' ', c2 = s2[0] | ' ';

    if (c1 < c2)
        return -1;
    if (c1 > c2)
        return 1;

    size_t s1_len = strlen(s1);
    size_t s2_len = strlen(s2);

    if (s1_len < s2_len)
        return -1;
    if (s1_len > s2_len)
        return 1;

    for (int i = 1; i < s1_len; i++) {
        c1 = s1[i] | ' ';
        c2 = s2[i] | ' ';
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
    }
    return 0;
}
