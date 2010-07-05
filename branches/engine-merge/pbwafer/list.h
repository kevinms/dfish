#ifndef _LIST_H_
#define _LIST_H_

typedef struct link_s
{
	struct link_s *next;  /* Next link in the list */
	struct link_s *prev;  /* Previous link in the list */
	void   *item;  /* Item owned by this link */
} link_t;

typedef struct list_s
{
	int len;       /* Number of elements in list */
	link_t  *head; /* First link in the list */
	link_t  *tail; /* Last link in the list  */
} list_t;

/* list management */
list_t *list_init(void);
void list_add(list_t *list, void *item);
void list_del_item(list_t *list, void *entity);
int list_del_head(list_t *list);
int list_del_tail(list_t *list);
void list_del(list_t *list);
void list_tail_to_head(list_t *list);
void list_head_to_tail(list_t *list);

/* returns 0 for no, 1 for yes */
int list_is_in(list_t *list, void *item);

#endif /* !_LIST_H_ */
