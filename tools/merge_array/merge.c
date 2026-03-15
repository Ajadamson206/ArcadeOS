#include <stdio.h>
#include <stdint.h>

const uint8_t num_font_len[] = {4  ,3  ,  4,  4,  4,  4,  4,  4,  4,4};
const uint8_t num_font_lenr[] ={134,139,143,148,153,158,163,168,173,178};

void print_hex(uint16_t x) {
    char buffer[] = "(u16)0x0000U";
    uint8_t buffer_index = 10;

    while(x) {
        char c = x % 16;
        x /= 16;
        
        if(c >= 10) {
            c -= 10;
            buffer[buffer_index] = c + 'A';
        } else {
            buffer[buffer_index] = c + '0';
        }

        buffer_index--;
    }

    printf("%s", buffer);
}

int main() {
    printf("{");
    for(int i = 0; i < 10; i++) {
        print_hex(((uint16_t)(num_font_len[i]<<8)) | num_font_lenr[i]);
        printf(",");
    }
    printf("}\n");
}