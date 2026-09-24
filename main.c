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
#define CARRIAGE_POS_MAX 5
#define FEED_STEPS_PER_LINE 5

// enum
typedef enum { ST_IDLE, ST_CARRIAGE_SCAN, ST_MEDIA_FEED, ST_HALTED, ST_CARRIAGE_RETURN } State; 

// variable
int carriage_pos;
int feed_steps;
int printed_lines;
int total_lines;
State current_state = ST_IDLE;

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
    /*while(1){
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
    }*/

    const char *state_names[] = {"ST_IDLE", "ST_CARRIAGE_SCAN", "ST_MEDIA_FEED", "ST_HALTED", "ST_CARRIAGE_RETURN"};
    int job_size = 3;
    int tick = 0;
    while(1){
        printf("[tick %3d] %-18s pos=%2d feed=%d lines=%d/%d  flags=", tick++, state_names[current_state], carriage_pos, feed_steps, printed_lines, total_lines);
        print_flags();
        switch (current_state)
        {
        case ST_IDLE:
            if(total_lines <= printed_lines && total_lines != 0){
                return 0;
            }
            else if(carriage_pos == 0){
                total_lines = job_size;
                printed_lines = 0;
                feed_steps = 0;
                current_state = ST_MEDIA_FEED;
            }
            else{
                total_lines = job_size;
                printed_lines = 0;
                current_state = ST_CARRIAGE_RETURN;
            }
            break;

        case ST_CARRIAGE_SCAN:
            if(carriage_pos == CARRIAGE_POS_MAX){
                printed_lines += 1;
                current_state = ST_CARRIAGE_RETURN;
            }
            else{
                carriage_pos += 1;
            }
            break;

        case ST_HALTED:
            current_state = ST_IDLE;
            break;

        case ST_CARRIAGE_RETURN:
            if(carriage_pos == 0 && total_lines > printed_lines){
                feed_steps = 0;
                current_state = ST_MEDIA_FEED;
            }
            else if(carriage_pos == 0 && total_lines <= printed_lines){
                printf("Done\n");
                current_state = ST_IDLE;
            }
            else{
                carriage_pos -= 1;
            }
            break;

        case ST_MEDIA_FEED:
            if(feed_steps == FEED_STEPS_PER_LINE){
                current_state = ST_CARRIAGE_SCAN;
            }
            else{
                feed_steps += 1;
            }
            break;

        default:
            break;
        }
        
        getchar();
    }
}
