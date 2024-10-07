/* 计科 2152131 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include<string.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

struct student {
	int* no; 
	char* name; 
	int* score; 
	struct student* next;
};

void output(struct student* head) {
	struct student* stu = head->next;
	while (stu != NULL){
		printf("%d %s %d\n", *(stu->no), stu->name, *(stu->score));
		struct student* old = stu;
		stu = stu->next;
		free(old->no);
		free(old->score);
		free(old->name);
		free(old);
	}
	stu = head->next;
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

	struct student* start = head;

	while (1) {
		struct student* p = (struct student*)malloc(sizeof(struct student));;
		if (p == NULL) {
			printf("Memory allocation using malloc failed!\n");
			fclose(infile);
			return -1;
		}
		int no = 0, score = 0;
		char name[9];

		fscanf(infile, "%d %s %d", &no, name, &score);
		if (no == 9999999) {
			free(p);
			break;
		}


		// 二次申请 成员
		p->no = (int*)malloc(sizeof(int));
		if (p->no == NULL) {
			printf("Memory allocation for no using malloc failed!\n");
			fclose(infile);
			return -1;
		}
		p->score = (int*)malloc(sizeof(int));
		if (p->score == NULL) {
			printf("Memory allocation for score using malloc failed!\n");
			fclose(infile);
			return -1;
		}
		p->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
		if (p->name == NULL) {
			printf("Memory allocation for name using malloc failed!\n");
			fclose(infile);
			return -1;
		}
		// 赋值
		*(p->no) = no;
		*(p->score) = score;
		strcpy(p->name, name);

		head->next = p;
		head = p;
		head->next = NULL;
	}
	
	output(start);
	free(start);
	fclose(infile);
	
	return 0;
}

