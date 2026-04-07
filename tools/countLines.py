#!/usr/bin/env python3

# Script used for counting the number of asm lines for the bootloader

from os import listdir
from os.path import isfile, join
from queue import Queue
import re

PATHS = ["boot/", "src/"]

lineCount = {}
lineCount.update({"C_lines": 0})
lineCount.update({"ASM_lines": 0})

def countAsmLines(filename: str):
    count = 0
    with open(filename, "r") as file:
        for line in file:
            line = line.lstrip()
            if line == "" or line == "\n" or line.startswith(";"):
                continue
            count += 1
    
    count += lineCount["ASM_lines"]
    lineCount.update({"ASM_lines": count})

def countCLines(filename: str):
    count = 0
    with open(filename, "r") as file:
        for line in file:
            line = line.lstrip()
            if line == "" or line == "\n" or line.startswith("/") or line.startswith("*"):
                continue
            count += 1
    
    count += lineCount["C_lines"]
    lineCount.update({"C_lines": count})



allFiles = Queue(80)
for path in PATHS:
    allFiles.put(path)

while allFiles.qsize() != 0:
    file = allFiles.get()

    if isfile(file):
        if re.search(".*\\.(c|h)$", file) != None:
            countCLines(file)
        elif re.search(".*\\.asm$", file) != None:
            countAsmLines(file)
    else:
        if file == "src/DOOM-PORT/doomgeneric":
            continue
    
        for newFiles in listdir(file):
            allFiles.put(join(file, newFiles))

print(lineCount)
