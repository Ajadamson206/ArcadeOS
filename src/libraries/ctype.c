
// All of these functions are based on the graph from 
// https://cplusplus.com/reference/cctype/

int isalnum(int character) {
    return ('0' <= character && character <= '9') || ('A' <= character && character <= 'Z') || ('a' <= character && character <= 'z');
}

int isalpha(int character) {
    return ('a' <= character && character <= 'z') || ('A' <= character && character <= 'Z');
}

// Blank characters
int isblank(int character) {
    return character == 0x09 || character == 0x20;
}

// Control Codes
int iscntrl(int character) {
    return (0x00 <= character && character <= 0x1F) || character == 0x7F;
}

int isdigit(int character) {
    return '0' <= character && character <= '9';
}

int isgraph(int character) {
    return 0x21 <= character && character <= 0x7E;
}

int islower(int character) {
    return 'a' <= character && character <= 'z';
}

int isprint(int character) {
    return 0x20 <= character && character <= 0x7E;
}

int ispunct(int character) {
    return (0x21 <= character && character <= 0x2F) || (0x3A <= character && character <= 0x40) || (0x5B <= character && character <= 0x60) || (0x7B <= character && character <= 0x7E);
}

// Spaces
int isspace(int character) {
    return (0x09 <= character && character <= 0x0D) || character == 0x20;
}

int isupper(int character) {
    return 'A' <= character && character <= 'Z';
}


int isxdigit(int character) {
    return ('0' <= character && character <= '9') || ('A' <= character && character <= 'Z') || ('a' <= character && character <= 'z');
}