#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

enum menu { FLASH_CARD = 1, GAME, EXIT };

const struct {
    char *eng;
    char *kor;
} word_dict[7][10] = {
    // 1일차
    { {"apple", "사과"}, {"banana", "바나나"}, {"grape", "포도"}, {"peach", "복숭아"}, {"kiwi", "키위"},
      {"mango", "망고"}, {"orange", "오렌지"}, {"strawberry", "딸기"}, {"watermelon", "수박"}, {"pineapple", "파인애플"} },
    
    // 2일차
    { {"dog", "개"}, {"cat", "고양이"}, {"mouse", "쥐"}, {"bird", "새"}, {"fish", "물고기"},
      {"rabbit", "토끼"}, {"horse", "말"}, {"elephant", "코끼리"}, {"tiger", "호랑이"}, {"lion", "사자"} },
    
    // 3일차
    { {"sun", "태양"}, {"moon", "달"}, {"star", "별"}, {"cloud", "구름"}, {"rain", "비"},
      {"snow", "눈"}, {"wind", "바람"}, {"storm", "폭풍"}, {"lightning", "번개"}, {"thunder", "천둥"} },
    
    // 4일차
    { {"water", "물"}, {"fire", "불"}, {"earth", "지구"}, {"air", "공기"}, {"stone", "돌"},
      {"sand", "모래"}, {"mud", "진흙"}, {"ocean", "바다"}, {"river", "강"}, {"lake", "호수"} },
    
    // 5일차
    { {"red", "빨간색"}, {"blue", "파란색"}, {"green", "초록색"}, {"yellow", "노란색"}, {"purple", "보라색"},
      {"orange", "주황색"}, {"pink", "분홍색"}, {"brown", "갈색"}, {"black", "검정색"}, {"white", "흰색"} },
    
    // 6일차
    { {"one", "하나"}, {"two", "둘"}, {"three", "셋"}, {"four", "넷"}, {"five", "다섯"},
      {"six", "여섯"}, {"seven", "일곱"}, {"eight", "여덟"}, {"nine", "아홉"}, {"ten", "열"} },
    
    // 7일차
    { {"carrot", "당근"}, {"potato", "감자"}, {"onion", "양파"}, {"cabbage", "양배추"}, {"lettuce", "상추"},
        {"spinach", "시금치"}, {"broccoli", "브로콜리"}, {"pepper", "고추"}, {"cucumber", "오이"}, {"tomato", "토마토"} }
};

// shuffle array
void shuffle(int *arr, int n) {
    srand(time(NULL));

    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    // for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
}

// get random integer from 0 to n
static inline int rand_int(int n) {
    srand(time(NULL));
    return rand() % (n+1);
}

static inline void print_wrong_indicator(int wrongCnt) {
    if (wrongCnt >= 1) printf("                        O \n");
    else putchar('\n');

    if (wrongCnt >= 4) printf("                      / | \\ \n");
    else if (wrongCnt >= 3) printf("                      / | \n");
    else if (wrongCnt >= 2) printf("                        | \n");
    else putchar('\n');

    if (wrongCnt >= 6) printf("                       / \\ \n");
    else if (wrongCnt >= 5) printf("                       / \n");
    else putchar('\n');

    putchar('\n');
}

_Bool check_char_in_target(const char *target, const int targetLength, const char c, char *const inputField, int *const revealed) {
    _Bool found = 0;

    for (int i = 0; i < targetLength; i++) {
        if (target[i] != c) continue;
        else if (inputField[i] == c) return 1; // if already revealed, return 1

        inputField[i] = c;
        found = 1;
        (*revealed)++;
    }

    return found;
}

void flash_card() {
    // shuffle order array
    int order[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle(order, 10);

    // input day and outputMethod
    int day, outputMethod;
    printf("학습 일차 : ");
    scanf("%d", &day);
    printf("출력 방식(수동모드 : 1, 자동모드 : 2) : ");
    scanf("%d", &outputMethod);
    getchar();
    sleep(1);
    system("clear");

    // start flash card
    for (int i = 0; i < 10; i++) {
        printf(">> 영어 단어 암기 프로그램 : 플래쉬카드 <<\n\n\n\n\n");
        printf("%s\n", word_dict[day-1][order[i]].eng);

        if (outputMethod == 1) getchar();       // if manual mode, wait for enter key
        else if (outputMethod == 2) sleep(1);   // if auto mode, wait for 1 second
        system("clear");

        printf(">> 영어 단어 암기 프로그램 : 플래쉬카드 <<\n\n\n\n\n");
        printf("%s\n", word_dict[day-1][order[i]].kor);

        if (outputMethod == 1) getchar();
        else if (outputMethod == 2) sleep(1);
        system("clear");
    }
}

void game() {
    // input day
    int day;
    printf("학습 일차 : ");
    scanf("%d", &day);
    getchar();

    // set random word to target
    const int targetIndex = rand_int(9);
    const char *hint = word_dict[day-1][targetIndex].kor;
    const char *targetWord = word_dict[day-1][targetIndex].eng;
    const int targetLength = strlen(targetWord);

    // set input field
    char *const inputField = (char *)malloc(targetLength+1);
    for (int i = 0; i < targetLength; i++) inputField[i] = '_';
    inputField[targetLength] = '\0';

    sleep(1);

    // start game
    int try, wrongCnt = 0, revealed = 0; // wrong count for indicator, revealed for checking if all characters are revealed
    for (try = 1; wrongCnt < 6 && revealed < targetLength; try++) {
        system("clear");
        printf(">> 영어 단어 암기 프로그램 : 게임 <<\n");
        printf("(힌트) %s\n\n", hint);

        printf("------------------------+\n");
        print_wrong_indicator(wrongCnt);

        for (int i = 0; i < targetLength; i++) printf("%c ", inputField[i]);
        printf("\n%d번째 시도 : ", try);

        char c;
        c = getchar();
        getchar();
        
        // reveal correct character and check if input is correct
        if (!check_char_in_target(targetWord, targetLength, c, inputField, &revealed)) wrongCnt++;
    }

    // game done
    // update message one more time
    system("clear");
    printf(">> 영어 단어 암기 프로그램 : 게임 <<\n");
    printf("(힌트) %s\n\n", hint);

    printf("------------------------+\n");
    print_wrong_indicator(wrongCnt);

    for (int i = 0; i < targetLength; i++) printf("%c ", inputField[i]);

    if (revealed == targetLength) printf("\n##########################\n### Congratulations!!! ###\n##########################\n");
    else printf("\n실패! 정답은 %s입니다.\n", targetWord);

    sleep(2);

    free(inputField);
}

int main() {
    int select;

    do {
        // show menu and wait for select
        system("clear");

        printf(">> 영어 단어 암기 프로그램 <<\n");
        printf("1. 플래쉬카드\t\t2. 게임\t\t3. 프로그램 종료\n\n");
        printf("번호를 선택하세요 : ");
        scanf("%d", &select);

        system("clear");

        // do selected stuff
        switch (select) {
            case FLASH_CARD:
                flash_card();
                break;
            case GAME:
                game();
                break;
            case EXIT:
                system("clear");
                break;
            default: // invalid input
                printf("입력이 올바르지 않습니다.\n");
                sleep(1);
                break;
        }
    } while (select != EXIT);

    return 0;
}