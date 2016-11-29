#include <ext_list.h>

void ext_list_init(struct ext_list_head *head)
{ head->next = head->prev = head; }

static void ext_list_insert(struct ext_list_head *new, struct ext_list_head *prev,
			struct ext_list_head *next)
{
	new->prev = prev;
	new->next = next;
	prev->next = new;
	next->prev = new;
}

void ext_list_add(struct ext_list_head *new, struct ext_list_head *head)
{ ext_list_insert(new, head, head->next); }

void ext_list_add_tail(struct ext_list_head *new, struct ext_list_head *head)
{ ext_list_insert(new, head->prev, head); }

static void __ext_list_del(struct ext_list_head *prev, struct ext_list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

void ext_list_del(struct ext_list_head *entry)
{
	__ext_list_del(entry->prev, entry->next);
	entry->next = 0;
	entry->prev = 0;
}

static void __ext_list_splice(struct ext_list_head *list, struct ext_list_head *prev,
			struct ext_list_head *next)
{
	struct ext_list_head *first = list->next;
	struct ext_list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;
	last->next = next;
	next->prev = last;
}

void ext_list_splice(struct ext_list_head *list, struct ext_list_head *head)
{
	if (!ext_list_empty(list)) {
		__ext_list_splice(list, head, head->next);
		ext_list_init(list);
	}
}

int ext_list_empty(const struct ext_list_head *head)
{ return head->next == head; }

struct ext_list_head *ext_list_first(struct ext_list_head *head)
{ return head->next; }

size_t ext_list_size(const struct ext_list_head *head)
{
	size_t size = 0;

	for (const struct ext_list_head *pos = head->next; pos != head;
		pos = pos->next)
		++size;
	return size;
}
