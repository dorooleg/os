#ifndef __PAGEA_H__
#define __PAGEA_H__

#include <stdint.h>

void *pagea_alloc(uint64_t order);
void pagea_free(void *ptr);

void* pagea_alloc_concurrent(uint64_t order);
void pagea_free_concurrent(void* ptr);

#endif //__PAGEA_H__
