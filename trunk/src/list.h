#ifndef _LIST_H_
#define _LIST_H_

struct link_t
{
   struct link_t *next;  /* Next link in the list */
   struct link_t *prev;  /* Previous link in the list */
   void          *item;  /* Item owned by this link */
};

struct list_t
{
   int len;              /* Number of elements in list */
   struct link_t  *head; /* First link in the list */
   struct link_t  *tail; /* Last link in the list  */
};

/* list management */
struct list_t *list_init(void);
void list_add(struct list_t *list, void *item);
void list_del_item(struct list_t *list, void *entity);
int list_del_head(struct list_t *list);
void list_del(struct list_t *list);
void list_tail_to_head(struct list_t *list);
void list_head_to_tail(struct list_t *list);

#endif /* !_LIST_H_ */
