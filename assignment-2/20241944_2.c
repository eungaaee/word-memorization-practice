#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct eng_kor {
	char eng[16]; // 15개 * 1byte + 1byte('\0')
	char kor[21]; // 10개 * 2byte + 1byte('\0')
};

struct node {
	struct eng_kor data;
    struct node *next;
};

struct node *head = NULL;
int dict_word_cnt = 0;

struct node *create_node(struct eng_kor data) {
	struct node *new_node = (struct node *)malloc(sizeof(struct node));
	
	/* strcpy(new_node->data->eng, data.eng);
	strcpy(new_node->data->kor, data.kor); */
	new_node->data = data;
	new_node->next = NULL;

	return new_node;
}

void push_front(struct eng_kor data) {
	struct node *new_node = create_node(data);
	
	new_node->next = head;
	head = new_node;
}

#if defined(DEBUG)
	void print_linkedlist() {
		for (struct node *i = head; i != NULL; i = i->next) {
			printf("eng: %s, kor: %s\n", i->data.eng, i->data.kor);
		}
	}
#endif

void initialize_dict() {
	FILE *dict_fp = fopen("dic.txt", "r");

	if (dict_fp == NULL) {
		fprintf(stderr, "Cannot open file.");
		exit(1);
	}

    struct eng_kor data;
    while (fscanf(dict_fp, "%s %s", data.eng, data.kor) != EOF) {
		push_front(data);
		dict_word_cnt++;
    }
	
	// bubble sort
	for (struct node *i = head; i != NULL; i = i->next) {
		_Bool swapped = 0;

		for (struct node *j = head; j->next != NULL; j = j->next) {
			if (strcmp(j->data.eng, j->next->data.eng) > 0) {
				struct eng_kor temp = j->data;
				j->data = j->next->data;
				j->next->data = temp;

				swapped = 1;
			}
		}

		if (!swapped) break;
	}

	fclose(dict_fp);

#ifdef DEBUG
	print_linkedlist();
	getchar();
	getchar();
#endif
}

void quiz() {
	printf(">> 영어 단어 맞추기 <<\n");

	struct node *cur_node = head;
	int questioned_cnt = 0, correct_cnt = 0;
	
	char input[16];
	for (int i = 0; i < dict_word_cnt; i++) {
		printf("%s -> ", cur_node->data.kor);
		scanf("%s", input);

		if (strcmp(".quit", input) == 0) {
			break;
		} else if (strcmp(cur_node->data.eng, input) == 0) {
			correct_cnt++;
			printf("correct!\n");
		} else {
			printf("incorrect!\n");
		}

		questioned_cnt++; // .quit 입력한 문제는 반영 X
		cur_node = cur_node->next;
	}

	printf("당신의 점수는 %.2f 점 입니다. ", questioned_cnt == 0 ? 0.0f : 100.0f * correct_cnt / questioned_cnt);
	getchar();
	getchar();
}

int main() {
	initialize_dict();

    int option;
    while (1) {
        system("clear");
        printf(">> 영어 단어 맞추기 프로그램 <<\n");
        printf("1. 영어 단어 맞추기\t\t2. 프로그램 종료\n\n");
        printf("번호를 선택하세요: ");
        scanf("%d", &option);

        if (option == 1) {
            system("clear");
            quiz();
        } else if (option == 2) {
            system("clear");
            break;
        } else {
            printf("잘못된 입력입니다.\n");
            return 1;
        }
    }

    return 0;
}
