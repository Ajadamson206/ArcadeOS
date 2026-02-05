#include "../../boot/stage2/include/stdint.h"
#include "../../boot/stage2/include/kernel_parse.h"

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

    // The section table is at the end of the file
    u32 numheaders = buffer->e_shnum;
    u32 current = 0;
    rewind(elf);
    fseek(elf, buffer->e_shoff, SEEK_SET);

    while(current < numheaders) {

        Elf32_Shdr *shead = malloc(sizeof(*shead));

        out = fread(shead, sizeof(Elf32_Shdr), 1, elf);
        if(out == 0) {
            fprintf(stderr, "Unable to read the section header\n");
            return 1;
        }
   
        printf("\nCurrent Header: [%u]\n", current++);

        Elf32_Word sh_name = shead->sh_name;
        printf("sh_name: 0x%X\n", sh_name);

        Elf32_Word sh_type = shead->sh_type;
        printf("sh_type: 0x%X\n", sh_type);

        Elf32_Word sh_flags = shead->sh_flags;
        printf("sh_flags: 0x%X\n", sh_flags);

        Elf32_Addr sh_addr = shead->sh_addr;
        printf("sh_addr: 0x%X\n", sh_addr);

        Elf32_Off sh_offset = shead->sh_offset;
        printf("sh_offset: %u\n", sh_offset);

        Elf32_Word sh_size = shead->sh_size;
        printf("sh_size: %u\n", sh_size);

        Elf32_Word sh_link = shead->sh_link;
        printf("sh_link: %u\n", sh_link);

        Elf32_Word sh_info = shead->sh_info;
        printf("sh_info: %u\n", sh_info);

        Elf32_Word sh_addralign = shead->sh_addralign;
        printf("sh_addralign: %u\n", sh_addralign);

        Elf32_Word sh_entsize = shead->sh_entsize;
        printf("sh_entsize: %u\n", sh_entsize);
    }
    return 0;
}