#ifndef HELPER_FUNC_H
#define HELPER_FUNC_H
#ifdef BT
node *find_parent(node *);
node *handle_deletion(node **);
#else
node *find_parent(node *, int32_t);
node *handle_deletion(node **, int32_t);
#endif

#endif
