#ifndef __EXT_LIST_H__
#define __EXT_LIST_H__

#include <kernel.h>

struct ext_list_head {
	struct ext_list_head *next;
	struct ext_list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct ext_list_head name = LIST_HEAD_INIT(name)
#define LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

void ext_list_init(struct ext_list_head *head);
void ext_list_add(struct ext_list_head *new, struct ext_list_head *head);

static inline void ext_list_insert_after(struct ext_list_head *new,
			struct ext_list_head *node)
{ ext_list_add(new, node); }

void ext_list_add_tail(struct ext_list_head *new, struct ext_list_head *head);

static inline void ext_list_insert_before(struct ext_list_head *new,
			struct ext_list_head *node)
{ ext_list_add_tail(new, node); }

void ext_list_del(struct ext_list_head *entry);
void ext_list_splice(struct ext_list_head *list, struct ext_list_head *head);
int ext_list_empty(const struct ext_list_head *head);
struct ext_list_head *ext_list_first(struct ext_list_head *head);
size_t ext_list_size(const struct ext_list_head *head);

#endif /*__EXT_LIST_H__*/
