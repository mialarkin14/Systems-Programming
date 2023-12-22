// Linked list demo program in CSE 3100
// The APIs may not be the best

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"

// print an error message by an error number, and return
// the function does not exit from the program
// the function does not return a value
void error_message(enum ErrorNumber errno) {
  char *messages[] = {
      "OK",
      "Memory allocaton failed.",
      "Deleting a node is not supported.",
      "The number is not on the list.",
      "Sorting is not supported.",
      "Reversing is not supported.",
      "Token is too long.",
      "A number should be specified after character d, a, or p.",
      "Token is not recognized.",
      "Invalid error number."};

  if (errno < 0 || errno > ERR_END)
    errno = ERR_END;
  printf("linkedlist: %s\n", messages[errno]);
}

node *new_node(int v) {
  node *p = malloc(sizeof(node)); // Allocate memory
  if (p == NULL) {
    error_message(ERR_NOMEM);
    exit(-1);
  }

  // Set the value in the node.
  p->v = v; // you could do (*p).v
  p->next = NULL;
  return p; // return
}

node *prepend(node *head, node *newnode) {
  newnode->next = head;
  return newnode;
}

node *find_node(node *head, int v) {
  while (head != NULL) {
    if (head->v == v)
      return head;
    head = head->next;
  }
  return head;
}

node *find_last(node *head) {
  if (head != NULL) {
    while (head->next != NULL)
      head = head->next;
  }
  return head;
}

node *append(node *head, node *newnode) {
  node *p = find_last(head);

  newnode->next = NULL;
  if (p == NULL)
    return newnode;
  p->next = newnode;
  return head;
}

node *delete_list(node *head) {
  while (head != NULL) {
    node *p = head->next;
    free(head);
    head = p;
  }
  return head;
}

void print_list(node *head) {
  printf("[");
  while (head) {
    printf("%d, ", head->v);
    head = head->next;
  }
  printf("]\n");
}

void print_node(node *p) {
    printf("%p: v=%-5d next=%p\n", p, p->v, p->next);
}

void print_list_details(node *head) {
  while (head) {
    print_node(head);
    head = head->next;
  }
}

// functions that have not been implemented

node *delete_node(node *head, int v) {
    // Function that iterates through the linked list and deletes the desired node

    // s is the original head
    node *s = head;
    // While the head is not equal to NULL keep making head the next node to loop through the nodes
    while (head != NULL) 
    {   
      //If the node is the first node:
      if (head->v == v)
      {
          // and if the node is the only node
          if (head->next == NULL)
            {
              // Get rid of that node and simply return
              free(s);
              return;
            }
          // h is a pointer to the new head 
          node *h = head->next;
          // Get rid of the head and return the linked list
          free(s);
          return h;
      }

      // If the node is not in the list give the error message and return the list 
      if (head->next == NULL)
      {
        error_message(ERR_NOTFOUND);
        return s;
      }  
        
      // The node is not the head  
      if ((head->next)->v == v)
      {
        // Head is previous node
        // p is the node you want to delete
        // temp is the next node
        node *p = head->next;
        // If the node is the last node:
        if (((head->next)->next) == NULL)
        {
          
          free(p);                     // Then free the last node
          head->next = NULL;           // Set the previous' next to NULL   
          return s;                    // Return the linked list
        }
        node *temp = (head->next)->next; 
        // Delete that node, set previous node equal to the next node and free that memory
        free(p); 
        head->next = temp; 
        return s;
        }   
      head = head->next;
    }
}

/*
 * Given a pointer to the head node of an acyclic list, change the
 * next links such that the nodes are linked in reverse order.
 * Allocating new nodes or copying values from one node to another
 * is not allowed, but you may use additional pointer variables.
 * Return value is a pointer to the new head node.
 */
node *reverse_list(node *head) {
  // Function that reverses a linked list
  node *prev = head;
  node *next = head->next;
    while (head != NULL)
    {
      next = prev;
      prev = head;
      head = next;
    }
  return head;
}