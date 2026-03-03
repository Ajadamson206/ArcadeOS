#include <stdint.h>
#include <bootinfo.h>
#include <graphics.h>
#include <serial.h>
#include <rawio.h>

__attribute__((noreturn))
void kernel_main(u32 magic, void *mb_info) {
    if(magic != 0x36D76289) {
        goto error;
    }

    load_pointers(mb_info);

    set_framebuffer(get_framebuffer_info());

    main_menu_background();

    // Print to the serial out

    serial_print(COM1, "RDTSC: ");

    u64 time = rdtsc();
    u32 time_upper = (u32)(time >> 32);

    serial_print_hex(COM1, time_upper);
    serial_print(COM1, " ");
    serial_print_hex(COM1, (u32)time);
    serial_print(COM1, "\n");


    io_wait();

    time = rdtsc();
    time_upper = (u32)(time >> 32);

    serial_print_hex(COM1, time_upper);
    serial_print(COM1, " ");
    serial_print_hex(COM1, (u32)time);



    //fill_screen(RED);

error:
    for(;;) { __asm__ volatile ("hlt"); }
}