#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int isBracket(char c) {
    return (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}');
}

void printTokens(const char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        switch (str[i]) {
            case ' ':
            case '\t':
                continue;  // Ignore whitespace characters
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
                printf("[Token: %c]", str[i]);
                break;
            default:
                printf("[Token: ");
                while (str[i] != '\0' && !isBracket(str[i]) && str[i] != ' ') {
                    printf("%c", str[i]);
                    i++;
                }
                printf("]");
                i--;  // Decrement i to account for the character after the token
        }
    }
    printf("\n");
}

void handle_text() {
    printf("Text\n");
}

void handle_import() {
    printf("Import\n");
}

int line_number = 0;

void syntax_error(char* error_msg) {
    printf("Syntax error at line %d: %s", line_number, error_msg);
    exit(1);
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
        syntax_error("Unrecognized Keyword");
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

    char line[100];
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