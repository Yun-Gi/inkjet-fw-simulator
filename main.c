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
        printf("0. CARRIAGE_AT_RIGHT\n");
        printf("1. CARRIAGE_AT_LEFT\n");
        printf("2. START_REQUEST\n");
        printf("3. STOP_REQUEST\n");
        printf("4. COVER_OPEN\n");
        printf("5. INK_LOW\n");
        printf("6. MEDIA_EMPTY\n");
        printf("7. EXIT\n");

        int input;
        scanf("%d", &input);

        if(input < 0 || input > 7){
            printf("False Input\n");
            continue;
        }
        else if(input == 7){
            return 0;
        }

        if(bit_check(1u << input)){
            bit_off(1u << input);
        }
        else{
            bit_on(1u << input);
        }
        print_flags();
        if(bit_check(ERROR_MASK)){
            printf("ERROR\n");
        }
        else{
            printf("OK\n");
        }
    }
}
