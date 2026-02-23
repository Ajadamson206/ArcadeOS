#!/usr/bin/env bash

qemu-system-i386 -drive format=raw,file=build/arcadeOS.img -serial stdio -no-reboot -S -gdb tcp::1234