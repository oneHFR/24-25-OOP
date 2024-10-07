/* 计科 2152131 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

struct student {
    int no;
    char name[9];
    int score;
    struct student* next;
};

void output(struct student* head)
{
	struct student* stu = head->next;
	while (stu != NULL) {
		printf("%d %s %d\n", stu->no, stu->name, stu->score);
		struct student* old = stu;
		stu = stu->next;
		free(old);
	}
	stu = head;
}

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FILE* infile;
	infile = fopen("./list.txt", "r");
	if (infile == NULL) {
		printf("打开文件失败\n");
		return -1;
	}

	// 先创造头部
	struct student* head = (struct student*)malloc(sizeof(struct student));
	if (head == NULL) {
		printf("Memory allocation using malloc failed!\n");
		fclose(infile);
		return -1;
	}
	// 保留一个head
	struct student* start = head; // 我发现了这里会在head有赋值之后第一时间同步但是后面就不会跟着变化了
	while (1) {
		struct student* p = (struct student*)malloc(sizeof(struct student));
		if (p == NULL) {
			printf("Memory allocation using malloc failed!\n");
			fclose(infile);
			return -1;
		}

		fscanf(infile, "%d %s %d", &p->no, p->name, &p->score);

		if (p->no == 9999999) {
			free(p);
			break;
		}

		head->next = p;
		head = p;
		head->next = NULL;
	}
	output(start);


	fclose(infile);
	free(start); 
    return 0;
}