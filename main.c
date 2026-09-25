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

void update_sonsors(void){
    if(carriage_pos == 0){
        bit_on(FLAG_CARRIAGE_AT_LEFT);
    }
    else{
        bit_off(FLAG_CARRIAGE_AT_LEFT);
    }
    if(carriage_pos == CARRIAGE_POS_MAX){
        bit_on(FLAG_CARRIAGE_AT_RIGHT);
    }
    else{
        bit_off(FLAG_CARRIAGE_AT_RIGHT);
    }
}

void handle_errors(void){
    if(bit_check(ERROR_MASK)){
        bit_off(FLAG_STOP_REQUEST | FLAG_START_REQUEST);
        current_state = ST_HALTED;
    }
}

int main(void){
    const char *state_names[] = {"ST_IDLE", "ST_CARRIAGE_SCAN", "ST_MEDIA_FEED", "ST_HALTED", "ST_CARRIAGE_RETURN"};
    int job_size = 3;
    int tick = 0;

    while(1){
        update_sonsors();
        handle_errors();
        printf("[tick %3d] %-18s pos=%2d feed=%d lines=%d/%d  flags=", tick++, state_names[current_state], carriage_pos, feed_steps, printed_lines, total_lines);
        print_flags();
    
        switch (current_state)
        {
        case ST_IDLE:
            if((flags & START_READY_MASK) == START_READY_MASK){
                total_lines = job_size;
                printed_lines = 0;
                feed_steps = 0;
                bit_off(FLAG_START_REQUEST);
                current_state = ST_MEDIA_FEED;
            }
            else if((flags & START_READY_MASK) == FLAG_START_REQUEST){
                total_lines = job_size;
                printed_lines = 0;
                bit_off(FLAG_START_REQUEST);
                current_state = ST_CARRIAGE_RETURN;
            }
            break;

        case ST_CARRIAGE_SCAN:
            if(bit_check(FLAG_CARRIAGE_AT_RIGHT)){
                printed_lines += 1;
                current_state = ST_CARRIAGE_RETURN;
            }
            else{
                carriage_pos += 1;
            }
            break;

        case ST_HALTED:
            if(!bit_check(ERROR_MASK)){
                current_state = ST_IDLE;
            }
            break;

        case ST_CARRIAGE_RETURN:
            if(bit_check(FLAG_CARRIAGE_AT_LEFT) && total_lines > printed_lines){
                feed_steps = 0;
                current_state = ST_MEDIA_FEED;
            }
            else if(bit_check(FLAG_CARRIAGE_AT_LEFT) && total_lines <= printed_lines){
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
        
        int c = getchar();
        if (c != '\n') {
            while(getchar() != '\n'){ }
            switch(c)
            {
            case 's':
                bit_on(FLAG_START_REQUEST);
                break;
            case 'c':
                if(bit_check(FLAG_COVER_OPEN)){
                    bit_off(FLAG_COVER_OPEN);
                }
                else{
                    bit_on(FLAG_COVER_OPEN);
                }
                break;
            case 'x':
                bit_on(FLAG_STOP_REQUEST);
                break;
            case 'q':
                return 0;
            
            default:
                break;
            }
        }
    }
}
