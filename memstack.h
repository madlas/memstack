#ifndef _STACK_MANAGER_H
#define _STACK_MANAGER_H

typedef struct _STACK_NODE
{
	int data_size;
	char *node_data;
	char *prev_node;
	char *next_node;
}stack_node, *lpstack_node;

typedef struct _STACK_HEAD
{
	int count;
	lpstack_node *p_index_map;
	stack_node *head_node;
	stack_node *tail_node;
}stack_head;

stack_head *stack_init();

int stack_add(stack_head *stack_head, char * p_data, int data_len);
void stack_free(stack_head *stack_head);

int stack_addptr(stack_head *stack_handle, char * p_data, int data_len);
void stack_freeptr(stack_head *stack_handle);

int stack_del(stack_head *stack_head, int index);
void stack_delall(stack_head *stack_handle);
char *stack_getdata(stack_head *stack_head, int index);
char *stack_getdata_fast(stack_head *stack_head, int index, int fast);
int stack_setdata(stack_head *stack_handle, int index, char *data, int data_len);
int stack_dumptomem(stack_head *stack_handle, char **out_mem, int piplen);

char *stack_next(stack_node **node);
stack_node *stack_first(stack_head *stack_handle);
int stack_add_nolike(stack_head *stack_handle, char *data, int data_len);//保证添加的数据块在链表中无重复

int stack_build_index(stack_head *stack_handle);

#define _MALLOC(x) malloc(x)
#define _FREE(x) free(x)

#endif
