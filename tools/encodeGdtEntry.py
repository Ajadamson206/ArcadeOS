#!/usr/bin/env python3

# Modify the Base/Limit/Access Byte/Flags Global Variables accordingly
# Either 1/True and 0/False can be used interchangably
# Will print the 64-bit long hex string

##################
# Variables to Modify
##################

BASE=0
LIMIT=0xFFFFF

# Access Byte
PRESENT=1
DPL=0           # Privilege Must be between Level 0-3
TYPE=1
EXECUTABLE=0
DC=0
RW=1
ACCESSED=0

# Flags Byte
GRANULATIY=1       
DB=1
LONG_MODE_FLAG=0

##################

def create_flags():
    global FLAGS
    FLAGS = 0

    if GRANULATIY == True or GRANULATIY == 1:
        FLAGS |= 1<<3

    if DB == True or DB == 1:
        FLAGS |= 1<<2

    if LONG_MODE_FLAG == True or LONG_MODE_FLAG == 1:
        FLAGS |= 1<<1
    
    # The 0th bit is reserved


def create_access_byte():
    global ACCESS_BYTE
    ACCESS_BYTE = 0

    if PRESENT == True or PRESENT == 1:
        ACCESS_BYTE |= 1<<7

    if 0 <= DPL <= 3:
        ACCESS_BYTE |= DPL<<5

    if TYPE == True or TYPE == 1:
        ACCESS_BYTE |= 1<<4

    if EXECUTABLE == True or EXECUTABLE == 1:
        ACCESS_BYTE |= 1<<3

    if DC == True or DC == 1:
        ACCESS_BYTE |= 1<<2

    if RW == True or RW == 1:
        ACCESS_BYTE |= 1<<1

    if ACCESSED == True or ACCESSED == 1:
        ACCESS_BYTE |= 1
    

def encode():
    entry = 0

    if LIMIT > 0xFFFFF:
        print("Limit cannot exceed 0xFFFFF")
        exit(1)

    # Ecnode the limit
    entry |= LIMIT & 0xFF
    entry |= ((LIMIT >> 8) & 0xFF) << 8
    entry |= ((LIMIT >> 16) & 0x0F) << 48
    
    # Encode the base
    entry |= (BASE & 0xFF) << 16
    entry |= ((BASE >> 8) & 0xFF) << 24
    entry |= ((BASE >> 16) & 0xFF) << 32
    entry |= ((BASE >> 24) & 0xFF) << 56
    
    # Encode the access byte
    entry |= ACCESS_BYTE << 40
    
    # Encode the flags
    entry |= (FLAGS << 4) << 48

    return entry

def main(): 
    create_flags()
    create_access_byte()

    hex_string = hex(encode())

    if len(hex_string) != 18:
        paddedZeros = "0" * (18 - len(hex_string))
        rs = hex_string.split('x')

        hex_string = "0x" + paddedZeros + rs[1].upper()

    print(hex_string)

if __name__ == "__main__":
    main()
