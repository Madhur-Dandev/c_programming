#ifndef LLIST_T
#define LLIST_T

typedef struct singly_ll s_ll;
typedef struct doubly_ll d_ll;
typedef struct ll ll;

struct singly_ll {
	int val;
	s_ll *next;
};

struct doubly_ll {
	int val;
	d_ll *prev;
	d_ll *next;
};

struct ll {
	int size;
	enum {SGL, DBL} type;
	union {
		s_ll *s_head;
		d_ll *d_head;
	};
	union {
		s_ll *s_tail;
		d_ll *d_tail;
	};
	void (*insertion)(ll *, int);
	int (*deletion)(ll *);
	int (*fetch)(ll *, int);
	int (*search)(ll *, int);
	int (*traverse)(ll *, int);
	void (*destroy)(ll *);
};

ll *init_list(int);
void insertion(ll *, int);
int deletion(ll *);
int fetch(ll *, int);
int search(ll *, int);
int traverse(ll *, int);
void destroy(ll *);

#endif
