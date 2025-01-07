#ifndef LLIST_T
#define LLIST_T

typedef struct singly_node s_ll;
typedef struct doubly_node d_ll;
typedef struct ll ll;

// values for flow of printing linked-list
enum
{
	DEF, // default flow (straight)
	REV	 // reverse flow
};

// structure for node of singly linked-list
struct singly_node
{
	int32_t val; // value of the node
	s_ll *next;	 // address of the next node
};

// structure for node of doubly linked-list
struct doubly_node
{
	int32_t val; // value of the node
	d_ll *prev;	 // address of the previous node
	d_ll *next;	 // address of the next node
};

// common structure of linked-list,its operations and its data members
struct ll
{
	int32_t size; // size of linked-list
	union
	{
		s_ll *s_head; // singly linked-list head node
		d_ll *d_head; // doubly linked-list head node
	};
	union
	{
		s_ll *s_tail; // singly linked-list tail node
		d_ll *d_tail; // doubly linked-list tail node
	};
	void (*insert_at_front)(ll *, int32_t);
	void (*insert_at_index)(ll *, int32_t, int32_t);
	void (*insert_at_end)(ll *, int32_t);
	int32_t (*delete_from_front)(ll *);
	int32_t (*delete_from_index)(ll *, int32_t);
	int32_t (*delete_from_end)(ll *);
	int32_t (*fetch)(ll *, int32_t);
	int32_t (*search)(ll *, int32_t);
	void (*print_list)(ll *, int32_t);
	void (*destroy)(ll *);
};

ll *init_list();
void insert_at_front(ll *, int32_t);
void insert_at_index(ll *, int32_t, int32_t);
void insert_at_end(ll *, int32_t);
int32_t delete_from_front(ll *);
int32_t delete_from_index(ll *, int32_t);
int32_t delete_from_end(ll *);
int32_t fetch(ll *, int32_t);
int32_t search(ll *, int32_t);
void print_list(ll *, int32_t);
void print_singly_def(s_ll *);
void print_def(ll *);
void print_rev(ll *);
void destroy(ll *);

#endif
