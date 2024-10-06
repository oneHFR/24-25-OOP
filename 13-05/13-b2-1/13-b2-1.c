/* 计科 2152131 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h> 
#include <string.h>

struct student {
	int no;
	char name[9];
	int score;
	int rank;
}; 

void sort(struct student* stu, int num, const char* metric)
{
	struct student temp;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num - i - 1; j++) {
			bool swapNeeded = false;
			if (strcmp(metric, "score") == 0) {
				if (stu[j].score < stu[j + 1].score)
					swapNeeded = true;
			}

			else if (strcmp(metric, "no") == 0) {
				if (stu[j].no > stu[j + 1].no)
					swapNeeded = true;
			}

			if (swapNeeded) {
				temp = stu[j];
				stu[j] = stu[j + 1];
				stu[j + 1] = temp;
			}
		}
	}
}

void rank(struct student* stu, int num)
{
	int temp_rank = 1, temp_score = stu[0].score;
	for (int i = 0; i < num; i++) {
		if (stu[i].score == temp_score)
			stu[i].rank = temp_rank;
		else {
			temp_rank = i + 1;
			stu[i].rank = temp_rank;
			temp_score = stu[i].score;
		}
	}
}

void output(struct student* stu, int num)
{
	for (int i = 0; i < num; i++)
		printf("%d %s %d %d\n", stu[i].no, stu[i].name, stu[i].score, stu[i].rank);
}


int main()
{
	FILE* infile;
	infile = fopen("./student.txt", "r");
	if (infile == NULL) {
		printf("打开文件失败\n");
		return -1;
	}
	int num;
	fscanf(infile, "%d", &num);

	// 知道数据量 - 申请动态空间 - check
	struct student* p = (struct student*)malloc(num * sizeof(struct student));
	if (p == NULL) {
		printf("Memory allocation using malloc failed!\n");
		fclose(infile);
		return -1;
	}

	// read data
	for (int i = 0; i < num; i++)
		fscanf(infile, "%d %s %d", &p[i].no, p[i].name, &p[i].score);

	// sort
	sort(p, num, "score");
	rank(p, num);
	sort(p, num, "no");
	output(p, num);

	fclose(infile);
	free(p);
	return 0;
}