#!/usr/bin/env python3

# Script used for counting the number of asm lines for the bootloader

from os import listdir
from os.path import isfile, join

PATH = "boot/"

def countLines(filename: str):
    count = 0
    with open(filename, "r") as file:
        for line in file:
            line = line.lstrip()
            if line == "" or line == "\n" or line.startswith(";"):
                continue
            count += 1
    
    return count


currentCount = countLines(PATH + "stage1.asm")
files = [f for f in listdir(PATH + "stage2/") if isfile(join(PATH + "stage2/", f))]

for file in files:
    currentCount += countLines(PATH + "stage2/" + file)

print(f"{currentCount} Lines of Assembly")
