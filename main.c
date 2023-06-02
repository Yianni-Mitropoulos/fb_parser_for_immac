#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define case_whitespace \
    case ' ': \
    case '\t':

#define case_special_char \
    case '(': \
    case ')': \
    case '[': \
    case ']': \
    case '{': \
    case '}': 

#define get_next_character(i, c) \
    i++; \
    c = str[i];

int line_number = 0;

void syntax_error(char* error_msg) {
    printf("Syntax error at line %d: %s", line_number, error_msg);
    exit(1);
}

void printTokens(const char *str) {
    int i = -1;
    char c;
    get_next_character(i, c)
    while (1) {
        switch (c) {
            case '\0':
                goto printTokensFinalize;
            case_whitespace
                get_next_character(i, c)
                break;
            case_special_char
                printf("[Token: %c]", c);
                get_next_character(i, c)
                break;
            case ':':
                printf("[Token: %c", c);
                get_next_character(i, c)
                if (c == ':' || c == '=' || c == ' ') {
                    printf("%c]", c);
                } else {
                    syntax_error("After a colon, there needs to be one of the following characters: {':','=',' ',}");
                }
                get_next_character(i, c)
                break;
            default:
                printf("[Token: ");
                int flag = 0;
                while (1) {
                    switch (c) {
                        case '\0':
                        case_whitespace
                        case_special_char
                            flag = 1;
                            break;
                        default:
                            printf("%c", c);
                    }
                    if (flag) {
                        break;
                    } else {
                        get_next_character(i, c)
                    }
                }
                printf("]");
        }
    }
    printTokensFinalize:
    printf("\n");
}

void handle_text() {
    printf("Text\n");
}

void handle_import() {
    printf("Import\n");
}

enum Mode {Undefined, Text, Import, Code};
enum Mode mode = Undefined;

void process_line(char* line) {

    // Increment the line number
    line_number++;

    // Check if the line starts with "#"
    if (line[0] == '#') {

        // Check if the line contains the word "Text"
        if (strstr(line, "Text") != NULL) {
            mode = Text;
            return;
        }

        // Check if the line contains the word "Import"
        if (strstr(line, "Import") != NULL) {
            mode = Import;
            return;
        }

        // Check if the line contains the word "Text"
        if (strstr(line, "Code") != NULL) {
            mode = Code;
            return;
        }

        // Throw a syntax error if no such keyword exists
        syntax_error("Unrecognized keyword");
    }

    // Based on the current mode, choose an appropriate action
    switch (mode) {

        // No mode has been defined
        case Undefined:
            syntax_error("Non-empty document must begin with a #");

        // Text mode
        case Text:
            printf("Text: %s\n", line);
            break;

        // Import mode
        case Import:
            printf("Import: %s\n", line);
            break;

        // Code mode
        case Code:
            printTokens(line);
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    char line[257];
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    printf("Processing file: %s\n", argv[1]);

    while (fgets(line, sizeof(line), file)) {

        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Process the line
        process_line(line);

    }

    fclose(file);

    return 0;
}