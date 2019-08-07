// Ring API冒烟测试
//
// 被测试的API头文件: "toolbox/ring.h"
//
// 编译步骤: 详见项目顶层目录 README 自述文档...
//
// 测试用例设计思路:
// 1. 创建一个空的Ring
// 2. 调用ring_enqueue(), 将若干条数据块输入环形链表;
// 最后: 释放为测试用例分配的所有内存


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h> // 用到 memset()

#include "toolbox/ring.h"

// 临时定义数据结构, 仅用于存储测试数据
struct record_st {
	unsigned long used_bytes;
	unsigned long capacity;
	char data[100];
	char padding[];
};
typedef struct record_st Record;
const int RECORD_PADDING_BYTES = ((int) (sizeof(Record) - offsetof(Record, padding)));
const int RECORD_CAPACITY = ((int) (sizeof(Record) - offsetof(Record, data) - RECORD_PADDING_BYTES));

void dump_node(const Node *node)
{
	unsigned long i;
	struct record_st *record;

	record = node->data;
	printf("%lu, %lu, {", record->used_bytes, record->capacity);
	for (i = 0; i < record->used_bytes; i++) {
		printf("%c", record->data[i]);
	}
	printf("}\n");
}

int main()
{
	struct record_st *all;
	const size_t N = 5;

	printf("Record结构体尺寸:\n");
	printf("Info: sizeof(struct record_st) = %d\n", (int) sizeof(struct record_st));
	printf("Info: sizeof(all[0]) = %d\n", (int) sizeof(all[0]));
	printf("Info: sizeof(all[0].data) = %d\n", (int) sizeof(all[0].data));
	printf("Info: sizeof(all[0].padding) = %d\n", RECORD_PADDING_BYTES);
	printf("测试开始：\n");
	printf("Info: 初始化N组Record数据块备用...\n");
	all = (Record *)calloc(sizeof(struct record_st), N);
	for (size_t i = 0; i < N; i++) {
		all[i].capacity = RECORD_CAPACITY;
		all[i].used_bytes = RECORD_CAPACITY;
		memset(all[i].data, 'A' + i, RECORD_CAPACITY);
	}

	// // 1. 初始化Ring
	// ring = ring_new(8);

	// 2. 将N组测试数据块插入双向链表deq
	for (size_t i = 0; i < N; i++) {
	}

	//// 3. 打印deq的内部数据
	// printf("ring 内部数据为: {\n");
	// printf("}\n");

	// // 释放ring
	// ring_free(ring);

	// 释放all数据块
	free(all);

	printf("测试结束\n");
	return 0;
}
