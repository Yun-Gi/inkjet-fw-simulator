#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//flag Constant
#define FLAG_MEDIA_EMPTY (1u << 6)
#define FLAG_INK_LOW  (1u << 5)
#define FLAG_COVER_OPEN (1u << 4)
#define FLAG_STOP_REQUEST (1u << 3)
#define FLAG_START_REQUEST (1u << 2)
#define FLAG_CARRIAGE_AT_LEFT (1u << 1)
#define FLAG_CARRIAGE_AT_RIGHT (1u << 0)
#define CARRIAGE_POS_MAX 20
#define FEED_STEPS_PER_LINE 5

// mask
#define ERROR_MASK (FLAG_MEDIA_EMPTY | FLAG_INK_LOW | FLAG_COVER_OPEN | FLAG_STOP_REQUEST)
#define START_READY_MASK (FLAG_CARRIAGE_AT_LEFT | FLAG_START_REQUEST)

// flags Variable
uint8_t flags = 0;

// function
void bit_on(uint8_t bit){
    flags |= bit;
}

void bit_off(uint8_t bit){
    flags &= ~bit;
}

bool bit_check(uint8_t mask){
    if (flags & mask){
        return true;
    }
    else{
        return false;
    }
}

void print_flags(void){
    for(int i = 7; i >= 0; i--){
        printf("%d", bit_check(1u << i));
    }
    printf("\n");
}

int main(void){
    while(1){
        
    }
}

/* 헤더 두 개 (하나는 printf, 하나는 uint8_t) */

/* 1. 플래그 상수: 비트 자리 7개 */

/* 2. 마스크: ERROR_MASK, START_READY_MASK */

/* 3. 센서 상태를 담는 변수 flags 하나 (함수 밖에 선언) */

/* 4. 함수들
      - 비트 세우기
      - 비트 지우기
      - 비트 켜져 있는지 확인
      - flags를 2진수 8자리로 출력 */

/* 5. main
      반복:
        메뉴 보여주기
        번호 입력받기
        그 번호의 센서를 켜거나 끄기
        flags를 2진수로 출력
        에러가 있는지 없는지 출력
      특정 번호를 입력하면 종료 */