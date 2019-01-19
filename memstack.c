/************************************************************************/
/* 模块名称：memstack.c                                                 */
/* 功能描述：内存栈管理                                                 */
/* 作者：madlas                                                         */
/* 日期：2010-08-16                                                     */
/* 修改历史：                                                           */
/************************************************************************/
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "typedef.h"
#include "memstack.h"
#include "Util.h"

void stack_free(stack_head *stack_handle)
{
	stack_head *p_head = stack_handle;
	stack_node *p_node;

	if (!p_head)
	{
		return;
	}

	p_node = p_head->head_node;

	while (p_node)
	{
		stack_node *temp_node = p_node;
		p_node = (stack_node *)(p_node->next_node);
		if (temp_node->node_data)
		{
			_FREE(temp_node->node_data);
		}
		_FREE(temp_node);
	};

	_FREE(p_head);
	p_head = NULL;
	stack_handle = NULL;
	return;

}
/*
链表不负责释放内容，由调用程序自行释放
*/
void stack_freeptr(stack_head *stack_handle)
{
	stack_head *p_head = stack_handle;
	stack_node *p_node;

	if (!p_head)
	{
		return;
	}

	p_node = p_head->head_node;

	while (p_node)
	{
		stack_node *temp_node = p_node;
		p_node = (stack_node *)(p_node->next_node);

		_FREE(temp_node);
	};

	_FREE(p_head);
	p_head = NULL;
	stack_handle = NULL;
	return;
}

stack_head *stack_init()
{
	stack_head *new_stack = (stack_head *)_MALLOC(sizeof (stack_head));
	memset(new_stack, 0, sizeof (stack_head));
	return new_stack;
}
/*
只添加指针到链表,链表不负责内存分配
*/
int stack_addptr(stack_head *stack_handle, char * p_data, int data_len)
{
	stack_node *p_tail = (stack_node *)_MALLOC(sizeof (stack_node));
	if (!p_tail)
	{
		return -1;
	}

	memset(p_tail, 0, sizeof (stack_node));
	p_tail->node_data = p_data;
	p_tail->data_size = data_len;

	if (!stack_handle->head_node)
	{
		stack_handle->head_node = p_tail;
		stack_handle->tail_node = p_tail;
		stack_handle->count = 1;
		return 0;
	}

	//添加新节点到链表末尾
	stack_handle->tail_node->next_node = (char *)p_tail;
	p_tail->prev_node = (char *)(stack_handle->tail_node);
	stack_handle->tail_node = p_tail;

	stack_handle->count++;
	return 0;

}
/*
复制内容到链表，链表负责分配内存
*/
int stack_add(stack_head *stack_handle, char * p_data, int data_len)
{
	char *node_data = _MALLOC(data_len);

	if (!node_data)
	{
		return -1;
	}
	memcpy(node_data, p_data, data_len);
	if (stack_addptr(stack_handle, node_data, data_len) < 0)
	{
		_FREE(node_data);
			return -1;
	}
	return 0;
}
int stack_add_str(stack_head *stack_handle, char * p_str)
{
	int str_len = strlen(p_str)  + 1;
	char *node_data = _MALLOC(str_len);

	if (!node_data)
	{
		return -1;
	}
	strcpy(node_data, p_str);
	if (stack_addptr(stack_handle, node_data, str_len) < 0)
	{
		_FREE(node_data);
			return -1;
	}
	return 0;
}

int stack_del(stack_head *stack_handle, int index)
{
	stack_node *p_data = stack_handle->head_node;
	int i;


	for (i= 0; p_data != NULL; p_data = (stack_node *)(p_data->next_node))
	{
		if (i == index)
		{
			if (p_data->prev_node)
			{
				((stack_node *)(p_data->prev_node))->next_node = p_data->next_node;
			}else
			{
				stack_handle->head_node = (stack_node *)(p_data->next_node);
			}
			if (p_data->next_node)
			{
				((stack_node *)(p_data->next_node))->prev_node = p_data->prev_node;
			}else
			{
				stack_handle->tail_node = (stack_node *)(p_data->prev_node);
			}

			stack_handle->count --;
			if (p_data->node_data)
			{
				_FREE(p_data->node_data);
			}
			_FREE(p_data);
			return 0;
		}
		i++;
	}
	return -1;
}

void  stack_delall(stack_head *stack_handle)
{
	int i;
	int count = stack_handle->count;
	for (i = 0; i < count; i++)
	{
		stack_del(stack_handle, 0);
	}
}

char *stack_getdata(stack_head *stack_handle, int index)
{
	stack_node *p_data = stack_handle->head_node;
	int i;

	if (index > stack_handle->count - 1)
	{
		return NULL;
	}

	for (i= 0; p_data != NULL; p_data = (stack_node *)(p_data->next_node))
	{
		if (i == index)
		{
			return p_data->node_data;
		}
		i++;
	}

	return NULL;
}

int stack_setdata(stack_head *stack_handle, int index, char *data, int data_len)
{
	stack_node *p_data = stack_handle->head_node;
	int i;

	if (index > stack_handle->count - 1)
	{
		return -1;
	}

	for (i= 0; p_data != NULL; p_data = (stack_node *)(p_data->next_node))
	{
		if (i == index)
		{
			memcpy(p_data->node_data, data, data_len);
			return 0;
		}
		i++;
	}
	return -1;
}

int stack_dumptomem(stack_head *stack_handle, char **out_mem, int piplen)
{
	int i, memsize;

	if (stack_handle == NULL)
	{
		return -1;
	}
	memsize = stack_handle->count * piplen;
	*out_mem = _MALLOC(memsize);
	for (i = 0; i < stack_handle->count; i++)
	{
		memcpy(*out_mem + i * piplen, stack_getdata(stack_handle, i), piplen);
	}
	//stack_free(stack_handle);

	return memsize;
}

char *stack_next(stack_node **node)
{
	char *nodedata = (char *)((*node)->node_data);

	*node = (stack_node *)((*node)->next_node);

	return nodedata;
}

stack_node *stack_first(stack_head *stack_handle)
{
	return stack_handle->head_node;
}


stack_node *stack_find_node(stack_head *stack_handle, int index)
{
	return NULL;
}

int stack_add_nolike(stack_head *stack_handle, char *data, int data_len)
{
	int idx = 0;
	stack_node *node = stack_handle->head_node;
	while (node != NULL)
	{
		if (node->data_size == data_len && memcmp(node->node_data, data, data_len) == 0)
			return idx + 1;

		node = (stack_node *)node->next_node;
		idx++;
	}

	stack_add(stack_handle, data, data_len);

	return stack_handle->count;
}



