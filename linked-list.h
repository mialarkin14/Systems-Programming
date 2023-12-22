typedef struct node_tag {
   int    v;				// data
   struct node_tag * next;	// A pointer to this type of struct
} node;						// Define a type. Easier to use.

//create a node with value v
node * create_node(int v) 
{
    node	* p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);		// you can be nicer
    
    // Set the value in the node.
    p->v = v;		
    p->next = NULL;  
    return p;			// return
}

//add newnode the last of the linked list determined by *head and *tail
//note **head, **tail
void add_last(node **head, node **tail, node *newnode)
{
	if((*head) == NULL)
	{
		(*head) = (*tail) = newnode;
	}
	else
	{
		(*tail)->next = newnode;
		(*tail) = newnode;
	}
}

//remove the first node from the list
//note **head

node * remove_first(node **head, node **tail) 
{
	node *p;

	p = (*head);
	if((*head)!=NULL) (*head) = (*head)->next;
	if((*tail) == p) (*tail) = NULL; 
	return p;
}

//print the list pointed by head
void print_list(node *head)
{
	node *p = head;

	while(p!=NULL)
	{
		printf("%d ", p->v);
		p = p->next;
	}
	printf("\n");
}

void free_all(node **head, node **tail)
{
        while((*head)!=NULL) free(remove_first(head, tail));
}

