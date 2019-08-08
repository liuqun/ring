/**
 * @brief 环形队列(ring)数据结构
 */

#pragma once

#define RINGLIB_VER_X 0
#define RINGLIB_VER_Y 10
#define RINGLIB_VERSION_NUMBER_X_Y (RINGLIB_VER_X*1000 + RINGLIB_VER_Y) /* 版本号格式为"X.00Y"，当X=1 Y=0时表示1.0版本；当X=1 Y=10时表示1.01版本；当X=1 Y=100时表示1.10版本，以此类推 */
#define RINGLIB_PRINT_VERSION() \
	printf("Ring lib version=%.2f\n", RINGLIB_VERSION_NUMBER_X_Y / 1000.0f)

#define RINGLIB_DEBUG_ALLOW_ALL /* 调试专用宏：统一开启全部调试开关 */

#if defined(RINGLIB_DEBUG_ALLOW_ALL)
#define RINGLIB_DEBUG_ALLOW_TINY_CAPACITY    /* 调试专用宏：允许设置特别小的环型队列容量 */
#define RINGLIB_DEBUG_ALLOW_PRINTF_DEBUG_MSG /* 调试专用宏：允许printf输出调试信息 */
#endif

#if (!defined(RINGLIB_DEBUG_ALLOW_ALL) && RINGLIB_VER_X>=1)
#define RINGLIB_IS_A_STABLE_VERSION
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


enum RingCapacity {// 环形队列容量(可容纳结点的总个数)必须是2^N(例如128，256，512，1024等等)，这么规定是为了优化执行速度
	/* 本环形队列的容量为固定常数值，并不支持自动扩容 */
	RING_CAPACITY_FIXED_TO_128=128,
	RING_CAPACITY_FIXED_TO_256=256,
	RING_CAPACITY_FIXED_TO_512=512,
	RING_CAPACITY_FIXED_TO_1024=1024,
	RING_CAPACITY_FIXED_TO_2048=2048,
	RING_CAPACITY_FIXED_TO_4096=4096,
	RING_CAPACITY_FIXED_TO_8192=8192,
#if RINGLIB_IS_A_STABLE_VERSION
	RING_CAPACITY_FIXED_TO_DEFAULT_SIZE=4096,
#elif defined(RINGLIB_DEBUG_ALLOW_TINY_CAPACITY)
	/* 以下参数仅仅用于调试环境，未经测试不得在生产环境下启用！ */
	RING_CAPACITY_FIXED_TO_DEFAULT_SIZE=2, // 测试专用
	RING_CAPACITY_FIXED_TO_4=4, // 测试专用
#endif
};

struct ring_ {
#if defined(RING_ENABLE_LOCKFREE_FEATURE)
	int producer_head_idx;
	int producer_tail_idx;
	int consumer_head_idx;
	int consumer_tail_idx;
#endif
	enum RingCapacity cap;//最大容量
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

Ring * ring_new(enum RingCapacity fixed_max_capacity);// 环形队列容量必须是2^N, 例如128，256，512，1024等等。这样规定目的是为了提高执行效率
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
