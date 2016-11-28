#include <test.h>
#include <list.h>
#include <printf.h>
#include <assert.h>
#include <fast_slab.h>
#include <stdint.h>
#include <thread.h>
#include <ints.h>
#include <mutex.h>
#include <ioport.h>

#define NULL 0

extern int counter_threads;
void print_thread(void* value);

static fast_slab_metadata test_allocator;  

static list_t * test_list = NULL;

static uint64_t acc = 0;

static void test_list_empty()
{
    assert(list_top(&test_list) == NULL, "list_top is not null (test_list_empty)");
    assert(list_size(&test_list) == 0, "list_size is not 0 (test_list_empty)");
}

static void free_test_value_list(void * p)
{
    free_fast_slab_concurrent(&test_allocator, p);
}

static void test_list1()
{
    uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
    *v = 0xf;
    list_push_front(&test_list, v);
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0xf, "list_top is not 0xf (test_list1)");
    assert(list_size(&test_list) == 1, "list_size is not 1 (test_list1)");
    list_pop_front(&test_list, free_test_value_list);
    assert(list_top(&test_list) == NULL, "list_top is not NULL (test_list1)");
    assert(list_size(&test_list) == 0, "list_size is not 0 (test_list1)");
}

static void test_list2()
{
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 0x7;
        list_push_front(&test_list, v);
    }
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 0xf;
        list_push_front(&test_list, v);
    }
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0xf, "list_top is not 0xf (test_list2)");
    assert(list_size(&test_list) == 2, "list_size is not 2 (test_list2)");
    list_pop_front(&test_list, free_test_value_list);
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0x7, "list_top is not 0x7 (test_list2)");
    assert(list_size(&test_list) == 1, "list_size is not 1 (test_list2)");
    list_pop_front(&test_list, free_test_value_list);
    assert(list_top(&test_list) == NULL, "list_top is not NULL (test_list2)");
    assert(list_size(&test_list) == 0, "list_size is not 0 (test_list2)");
}

static void test_list3()
{
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 0x7;
        list_push_front(&test_list, v);
    }
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 0xf;
        list_push_back(&test_list, v);
    }
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0x7, "list_top is not 0xf (test_list3)");
    assert(list_size(&test_list) == 2, "list_size is not 2 (test_list3)");
    list_pop_front(&test_list, free_test_value_list);
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0xf, "list_top is not 0x7 (test_list3)");
    assert(list_size(&test_list) == 1, "list_size is not 1 (test_list3)");
    list_pop_front(&test_list, free_test_value_list);
    assert(list_top(&test_list) == NULL, "list_top is not NULL (test_list3)");
    assert(list_size(&test_list) == 0, "list_size is not 0 (test_list3)");
}

static void test_list4()
{
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 0x7;
        list_push_front(&test_list, v);
    }
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 0xf;
        list_push_back(&test_list, v);
    }
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0x7, "list_top is not 0x7 (test_list4)");
    assert(list_size(&test_list) == 2, "list_size is not 2 (test_list4)");
    list_pop_back(&test_list, free_test_value_list);
    assert(*(uint64_t*)(list_top(&test_list)->value) == 0x7, "list_top is not 0x7 (test_list4)");
    assert(list_size(&test_list) == 1, "list_size is not 1 (test_list4)");
    list_pop_back(&test_list, free_test_value_list);
    assert(list_top(&test_list) == NULL, "list_top is not NULL (test_list4)");
    assert(list_size(&test_list) == 0, "list_size is not 0 (test_list4)");
}

static void sum(void *value) {
    acc += *(uint64_t*)value;
}
static char eq5(void *value) { return 5 == *(uint64_t*)value;
}

static char eq1(void *value) {
    return 1 == *(uint64_t*)value;
}

static char eq8(void *value) {
    return 8 == *(uint64_t*)value;
}

static char odd(void *value) {
    return *(uint64_t*)value % 2;
}

static void test_list5()
{
    for (uint32_t i = 0; i < 10; i++) {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = i;
        list_push_front(&test_list, v);
    }

    acc = 0;
    list_map(&test_list, sum);
    assert(acc == 45, "sum != 45 (test_list5)");

    list_remove_first(&test_list, eq5, free_test_value_list);

    acc = 0;
    list_map(&test_list, sum);
    assert(acc == 45 - 5, "sum != 40 (test_list5)");

    list_remove_first(&test_list, eq5, free_test_value_list);

    acc = 0;
    list_map(&test_list, sum);
    assert(acc == 45 - 5, "sum != 40 (test_list5)");

    list_remove_first(&test_list, eq1, free_test_value_list);

    acc = 0;
    list_map(&test_list, sum);
    assert(acc == 45 - 5 - 1, "sum != 39 (test_list5)");

    list_remove_all(&test_list, odd, free_test_value_list);

    acc = 0;
    list_map(&test_list, sum);
    assert(acc == 20, "sum != 20 (test_list5)");

    assert(*(uint64_t*)(list_find(&test_list, eq8)->value) == 8, "not found element 8 (test_list5)");
    assert(*(uint64_t*)(list_at(&test_list, 0)->value) == 8, "not found at 0 (test_list5)");
    assert(*(uint64_t*)(list_at(&test_list, 4)->value) == 0, "not found at 4 (test_list5)");
    {
        uint64_t * v = alloc_fast_slab_concurrent(&test_allocator);
        *v = 9;
        list_insert_after(&test_list, eq8, v);
    }
    assert(*(uint64_t*)(list_at(&test_list, 1)->value) == 9, "not found at 0 after insert (test_list5)");

    while (list_size(&test_list)) {
        list_pop_front(&test_list, free_test_value_list);
    }

    assert(list_top(&test_list) == NULL, "list_top is not NULL (test_list5)");
    assert(list_size(&test_list) == 0, "list_size is not 0 (test_list5)");
}

struct param
{
    int n;
    int res;
};

void* factorial(void* arg)
{
    printf("FAC begin\n");
    struct param *p = arg;    
    p->res = 1;
    for (int i = 1; i <= p->n; i++) {
        p->res *= i;
    }
    printf("FAC end\n");
    return arg;
}

void* fib(void* arg)
{
    printf("FIB begin\n");
    struct param *p = arg;    
    if (p->n <= 1) {
        p->res = p->n;
        return arg;
    }
    int v0 = 0;
    int v1 = 1;
    for (int i = 2; i <= p->n; i++) {
        v0 = v1 + v0;
        if (&v0 != &v1)
        {
            v0 ^= v1;
            v1 ^= v0;
            v0 ^= v1;
        }
    }
    p->res = v1;
    printf("FIB end\n");
    return arg;
}

static void print_thread_statistics()
{
    print_current_thread();
    print_init_threads();
    print_running_threads();
    print_terminated_threads();
}

void thread_test1()
{
    printf("begin thread test\n");
    struct param fac_param = { 10, 0 };
    struct param fib_param = { 10, 0 };

    struct thread_t* t1 = thread_create(factorial, &fac_param);
    struct thread_t* t2 = thread_create(fib, &fib_param);

    printf("-----> Create <----\n");
    print_thread_statistics();

    printf("-----> Start <----\n");
    thread_start(t2);
    thread_start(t1);

    printf("-----> Start(*) <----\n");
    print_thread_statistics();

    printf("-----> Join <----\n");
    void* res;
    thread_join(t2, &res);
    print_thread_statistics();

    thread_destroy(t1);
    thread_destroy(t2);

    printf("----> Destroy <----\n");
    print_thread_statistics();

    assert((((struct param *)res)->res) == 55, "thread fib is not correct (test_thread1)");
}

#define DELAY 10000000
static void* massiv_calc1(void* arg)
{
    (void)arg;
    int n = 0;
    while (1) {
        n++;
        if (n % DELAY == 0) {
            printf("massiv_calc1\n");
            n = 0;
        } 
    }
    return 0;
}

static void* massiv_calc2(void* arg)
{
    (void)arg;
    int n = 0;
    while (1) {
        n++;
        if (n % DELAY == 0) {
            printf("massiv_calc2\n");
            n = 0;
        } 
    }
    return 0;
}

static void* massiv_calc3(void* arg)
{
    (void)arg;
    int n = 0;
    while (1) {
        n++;
        if (n % DELAY == 0) {
            printf("massiv_calc3\n");
            n = 0;
        } 
    }
    return 0;
}

void scheduler_test1()
{
    struct thread_t* t1 = thread_create(massiv_calc1, NULL);
    struct thread_t* t2 = thread_create(massiv_calc2, NULL);
    struct thread_t* t3 = thread_create(massiv_calc3, NULL);

    thread_start(t2);
    thread_start(t1);
    thread_start(t3);
    print_thread_statistics();

    while (1) {
        __asm__ volatile ("" : : : "memory");
    }

    thread_destroy(t1);
    thread_destroy(t2);
    thread_destroy(t3);
} 

struct mutex_t global_mutex;
static void* mutex_func(void* arg)
{
    (void)arg;
    int n = 0;
    mutex_lock(&global_mutex);
    while (1) {
        n++;
        if (n % DELAY == 0) {
            out8(0xA0, 0x20);
            out8(0x20, 0x20);
            enable_ints();
            counter_threads = 0;
            list_map(&global_mutex.locked_threads, print_thread);    
            printf("mutex\n");
            n = 0;
            break;
        } 
    }
    mutex_unlock(&global_mutex);
    return 0;
}

void mutex_test()
{
    struct thread_t* t1 = thread_create(mutex_func, NULL);
    struct thread_t* t2 = thread_create(mutex_func, NULL);

    thread_start(t2);
    thread_start(t1);
    print_thread_statistics();

    while (1) {
        __asm__ volatile ("" : : : "memory");
    }

    thread_destroy(t1);
    thread_destroy(t2);
}

void test_main()
{
    test_allocator = create_fast_slab_allocator_concurrent(sizeof(uint64_t));
    test_list_empty();
    test_list1();
    test_list2();
    test_list3();
    test_list4();
    test_list5();
    print_init_threads();
    thread_test1();
//    mutex_test();
//    scheduler_test1();
}
