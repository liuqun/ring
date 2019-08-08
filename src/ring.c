#include <stddef.h> // 标准宏 offsetof()
#include <stdlib.h> // 标准函数 free() / malloc() / calloc() / abort() / exit()
#include <string.h> // 标准函数 strerror()
#include <assert.h> // 标准宏 assert()
#include <errno.h>  // 标准错误码查询
#include "toolbox/ring.h"

#if defined(RINGLIB_DEBUG_ALLOW_PRINTF_DEBUG_MSG)
#include <stdio.h>
#define PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
#define PRINTF(...)
#endif


void ring_free(Ring *ring)
{
    PRINTF("Debug: In function %s(): LINE=%d\n", __func__, __LINE__);
    PRINTF("       ring->capacity = %d\n", (int)ring->cap);
    PRINTF("       ring->head = %d\n", (int)ring->head);
    PRINTF("       ring->tail = %d\n", (int)ring->tail);
    free(ring);
    return;
}

Ring * ring_new(enum RingCapacity capacity)
{
    const size_t cap = (size_t) capacity;
    size_t i;
    Ring *obj = NULL;

    obj = malloc(offsetof(struct ring_, nodes) + cap * sizeof(struct node_));
    assert(NULL!=obj && "ERROR: An OUT-OF-MEMEORY Exception is raised!! malloc() returns NULL!\n");
    PRINTF("Debug: In function %s(): LINE=%d\n", __func__, __LINE__);
    PRINTF("       malloc() returns pointer = %p\n", obj);
    if (!obj) {
        PRINTF("ERROR: malloc() failed in function %s()!\n", __func__);
        PRINTF("ERROR: errno=%d: %s\n", errno, strerror(errno));
        return NULL;
    }

    obj->cap = capacity;
    obj->head = 0;
    obj->tail = 0;

    /* 请保留下列内部调试代码 */
    if ((int) RING_CAPACITY_DEFAULT <= 4) {
        PRINTF("Debug: In function %s(): LINE=%d\n", __func__, __LINE__);
        PRINTF("       正在向所有%d个结点中填充初始值以便辅助调试...\n", (int)cap);

        /* 为辅助调试，此处将所有结点的内容全部指向下列自定义字符串 */
        static char UNDEFINED_STRING[10] = "Undefined";
        void *UNDEFINED_POINTER = (void *) UNDEFINED_STRING;
        for (i=0; i<cap; i++) {
            obj->nodes[i].data = UNDEFINED_POINTER;
        }
    }
    return obj;
}

/* vi: set tabstop=4 shiftwidth=4 expandtab : */
