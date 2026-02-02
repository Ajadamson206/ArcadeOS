#include "../../boot/stage2/include/stdint.h"
#include "../../boot/stage2/include/elf_parse.h"

#include <stdio.h>
#include <stdlib.h>

void printHex(u8 value) {
    char buffer[3] = {0, 0, 0};

    // First 4 bits
    buffer[1] = value & 16;
    if (buffer[1] >= 10) {
        buffer[1] -= 10;
        buffer[1] += 'A';
    } else {
        buffer[1] += '0';
    }
    
    // Last 4 bits
    buffer[0] = value & 16;
    if (buffer[0] >= 10) {
        buffer[0] -= 10;
        buffer[0] += 'A';
    } else {
        buffer[0] += '0';
    }

    printf("%s", buffer);
}

int main() {
    FILE* elf = fopen("test.o", "rb");
    if(elf == NULL) {
        fprintf(stderr, "Unable to open test.o\n");
        return 1;
    }

    Elf32_Ehdr *buffer = malloc(sizeof(Elf32_Ehdr));

    size_t out = fread(buffer, sizeof(Elf32_Ehdr), 1, elf);
    if(out == 0) {
        fprintf(stderr, "Unable to read the file\n");
        return 1;
    }

    unsigned char *e_ident = buffer->e_ident;
    printf("e_ident: ");
    for(size_t i = 0; i < 16; i++) {
        printHex(e_ident[i]);
        printf(" ");
    }    

    Elf32_Half e_type = buffer->e_type;
    printf("\ne_type: %hu\n", e_type);
    
    Elf32_Half e_machine = buffer->e_machine;
    printf("e_machine: %hu\n", e_machine);
    
    Elf32_Word e_version = buffer->e_version;
    printf("e_version: %u\n", e_version);
    
    Elf32_Addr e_entry = buffer->e_entry;
    printf("e_entry: 0x%X\n", e_entry);
    
    Elf32_Off e_phoff = buffer->e_phoff;
    printf("e_phoff: %u\n", e_phoff);
    
    Elf32_Off e_shoff = buffer->e_shoff;
    printf("e_shoff: %u\n", e_shoff);
    
    Elf32_Word e_flags = buffer->e_flags;
    printf("e_flags: 0x%X\n", e_flags);
    
    Elf32_Half e_ehsize = buffer->e_ehsize;
    printf("e_ehsize: %hu\n", e_ehsize);

    Elf32_Half e_phentsize = buffer->e_phentsize;
    printf("e_phentsize: %hu\n", e_phentsize);
    
    Elf32_Half e_phnum = buffer->e_phnum;
    printf("e_phnum: %hu\n", e_phnum);

    Elf32_Half e_shentsize = buffer->e_shentsize;
    printf("e_shentsize: %hu\n", e_shentsize);
    
    Elf32_Half e_shnum = buffer->e_shnum;
    printf("e_shnum: %hu\n", e_shnum);
    
    Elf32_Half e_shstrndx = buffer->e_shstrndx;
    printf("e_shstrndx: %hu\n", e_shentsize);

    return 0;
}