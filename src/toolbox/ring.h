/**
 * @brief 环形队列(ring)数据结构
 */

#pragma once

#define RINGLIB_VER_X 0
#define RINGLIB_VER_Y 10
#define RINGLIB_VERSION_NUMBER_X_Y (RINGLIB_VER_X*1000 + RINGLIB_VER_Y) /* 版本号格式为"X.00Y"，当X=1 Y=0时表示1.0版本；当X=1 Y=10时表示1.01版本；当X=1 Y=100时表示1.10版本，以此类推 */
#define RINGLIB_PRINT_VERSION() \
	printf("Ring lib version=%.2f\n", RINGLIB_VERSION_NUMBER_X_Y / 1000.0f)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct ring_ {
#if defined(RING_ENABLE_LOCKFREE_FEATURE)
	int producer_head_idx;
	int producer_tail_idx;
	int consumer_head_idx;
	int consumer_tail_idx;
#endif
	unsigned int max_ring_nodes;
	unsigned int head;
	unsigned int tail;
	struct node_ {
		union {
			void *data;
			char *debug; // Reserved for debug only
		};
	} nodes[];
};
typedef struct ring_ Ring;
typedef struct node_ Node;

Ring * ring_new(unsigned int how_many_nodes);// 环形队列结点总个数必须是2^N, 例如128，256，512，1024等等
void ring_free(Ring *ring);
#define ring_free_gracefully(p) \
	do { \
		ring_free(p);\
		p = NULL; \
	} while (0)

unsigned int ring_count_nodes(const Ring *ring);
unsigned int ring_max_nodes(const Ring *ring);

void ring_enqueue(Ring *ring, /*const*/ void *data);
void ring_dequeue(Ring *ring);
void * ring_peek_head(const Ring *ring);

int ring_is_empty(const Ring *ring);

typedef void (*operator_fn_t)(void *node_data, void *extra_arg);
void exec_func_for_each_node(
	const Ring *ring, /// 对象指针
	void *extra_arg, /// 由应用层编写对应函数 fn() 的附加参数
	operator_fn_t fn /// 需要执行的函数名, 函数本身由应用层负责编写
	);

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus */
