#!/usr/bin/env bash

grub-file --is-x86-multiboot2 build/kernel/arcade.elf

if [[ $? -eq 0 ]]; then
    echo "Valid MB2 Header"
else
    echo "Unable to find Valid MB2 Header"
fi