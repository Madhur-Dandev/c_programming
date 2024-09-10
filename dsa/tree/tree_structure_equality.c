#include <stdio.h>
#include "btree.h"

int check(node *, node *);

int main(void) {
	node *root1, *root2;
	root1 = root2 = NULL;

	insert(&root1, 1);
	insert(&root1, 2);
	insert(&root1, 3);
	insert(&root1, 4);
	insert(&root1, 5);
	insert(&root1, 6);

	insert(&root2, 1);
	insert(&root2, 2);
	insert(&root2, 3);
	insert(&root2, 4);
	insert(&root2, 5);
	insert(&root2, 6);

	printf("%d\n", check(root1, root2));
}

int check(node *r1, node *r2) {
	if(r1 == NULL && r2 == NULL)
		return 1;
	if(r1 != NULL && r2 != NULL) {
		if(r1->data == r2->data) {
			int val1 = check(r1->left, r2->left);
			int val2 = check(r1->right, r2->right);

			return val1 == 1 && val2 == 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}
