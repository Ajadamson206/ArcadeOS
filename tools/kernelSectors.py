#!/usr/bin/env python3

import os
from math import ceil


fileinfo = os.lstat("build/kernel/arcade.elf")
filesize = fileinfo.st_size
filesectors = ceil(filesize / 512)

print(f"Info: {fileinfo}\nFile Size: {filesize}\nSectors: {filesectors}")

