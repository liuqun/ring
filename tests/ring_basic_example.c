// Ring API冒烟测试
//
// 被测试的API头文件: "toolbox/ring.h"
//
// 编译步骤: 详见项目顶层目录 README 自述文档...
//
// 测试用例设计思路:
// 1. 创建一个空的Ring
// 2. 调用ring_enqueue(), 将若干条数据块输入环形队列;
// 最后: 释放为测试用例分配的所有内存


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h> // 用到 memset()

#include "toolbox/ring.h"

// 临时定义数据结构, 仅用于存储测试数据
struct record_ {
	unsigned long used_bytes;
	unsigned long capacity;
	char data[10];
	char padding[];
};
typedef struct record_ Record;
const int RECORD_PADDING_BYTES = ((int) (sizeof(Record) - offsetof(Record, padding)));
const int RECORD_CAPACITY = ((int) (sizeof(Record) - offsetof(Record, data) - RECORD_PADDING_BYTES));

void dump_node(const Node *node)
{
	unsigned long i;
	struct record_ *record;

	record = node->data;
	printf("%lu, %lu, {", record->used_bytes, record->capacity);
	for (i = 0; i < record->used_bytes; i++) {
		printf("%c", record->data[i]);
	}
	printf("}\n");
}

int main()
{
	struct record_ all[5];
	const size_t N = sizeof(all)/sizeof(struct record_);

	printf("Record结构体尺寸:\n");
	printf("Info: sizeof(struct record_) = %d\n", (int) sizeof(struct record_));
	printf("Info: sizeof(all[0]) = %d\n", (int) sizeof(all[0]));
	printf("Info: sizeof(all[0].data) = %d\n", (int) sizeof(all[0].data));
	printf("Info: sizeof(all[0].padding) = %d\n", RECORD_PADDING_BYTES);
	printf("测试开始：\n");
	printf("Info: 初始化N组Record数据块备用...\n");
	for (size_t i = 0; i < N; i++) {
		all[i].capacity = RECORD_CAPACITY;
		all[i].used_bytes = RECORD_CAPACITY;
		memset(all[i].data, 'A' + i, RECORD_CAPACITY);
	}

	#if defined(RINGLIB_PRINT_VERSION)
	// 0. 检查 RINGLIB 版本号
	RINGLIB_PRINT_VERSION();
	#endif

	// 1. 初始化Ring
	printf("步骤1. ring = ring_new(RING_CAPACITY_DEFAULT)\n");
	Ring *ring = NULL;
	ring = ring_new(RING_CAPACITY_DEFAULT);
	printf("ring 总容量可存放 %u 个结点\n", ring_capacity(ring));

	// 2. 将N组测试数据块插入环形队列
	for (size_t i = 0; i < 2; i++) {
		//ring_enqueue(ring, &all[i]);
		const unsigned int mask = ring->cap - 1;
		void *data = &(all[i]);
		ring->nodes[ring->tail & mask].data = data;
		ring->tail += 1;
	}

	// 3. 打印环形队列的内部数据
	printf("步骤3. 检查环形队列当前状态:\n");
	printf("ring 当前包含%u个结点，内部数据分别为: {\n", ring_count(ring));
	for (long i = (long) (ring->head); i < (ring->tail); i++) {
		const unsigned int mask = ring->cap - 1;
		dump_node(&ring->nodes[i & mask]);
	}
	printf("}\n");

	// 释放ring
	ring_free(ring);

	printf("测试结束\n");
	return 0;
}
