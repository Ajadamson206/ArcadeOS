#include <stdint.h>

struct mb2_kernel_header {
    u32 magic;
    u32 architecture;
    u32 header_length;
    u32 checksum;
} __attribute__((packed));

#define MB2_KERNEL_TAG_START u16 type; \
                             u16 flags; \
                             u32 size

/**
 * Magic value stored in the kernel to show that it is a multiboot2
 * compliant kernel
 */
#define KERNEL_MAGIC 0xE85250D6

#define MB2_ARCH_I386     0u

struct mb2_kernel_tag_1 {
    MB2_KERNEL_TAG_START;

    u32 tag_types[8];
} __attribute__((packed));

struct mb2_kernel_tag_2 {
    MB2_KERNEL_TAG_START;

    u32 header_addr;
    u32 load_addr;
    u32 load_end_addr;
    u32 bss_end_addr;
} __attribute__((packed));

struct mb2_kernel_tag_3 {
    MB2_KERNEL_TAG_START;

    u32 entry_addr;
} __attribute__((packed));

__attribute__((section(".multiboot2"), used, aligned(8)))
static const struct {
    struct mb2_kernel_header fixed;
    struct mb2_kernel_tag_1 tag1;
    struct mb2_kernel_tag_2 tag2;
    struct mb2_kernel_tag_3 tag3;
    u64 end;
} mb2_header = {
    .fixed = {
        .magic         = KERNEL_MAGIC,
        .architecture  = MB2_ARCH_I386,
        .header_length = (u32)sizeof(mb2_header),
        // checksum makes (magic + arch + length + checksum) == 0 mod 2^32
        .checksum      = (u32)(0u - (KERNEL_MAGIC + MB2_ARCH_I386 + (u32)sizeof(mb2_header))),
    },
    .tag1 = {
        .type = 1,
        .flags = 0,
        .size = 36,
        .tag_types = { 1u, 2u, 4u, 5u, 6u, 7u, 8u, 9u }
    },
    .tag2 = {
        .type = 2,
        .flags = 0,
        .size = sizeof(mb2_header.tag2),
        .header_addr = 12,
        .load_addr = 23,
        .load_end_addr = 23,
        .bss_end_addr = 23
    },
    .tag3 = {
        .type = 3,
        .flags = 0,
        .size = sizeof(mb2_header.tag3),
        .entry_addr = 0x00200000u
    },
    .end = 8
};