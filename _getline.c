#include "shell.h"

/**
 * _getline - Wrapper function for the getline function.
 * @line: Pointer to the buffer that will store the line.
 * @n: Pointer to the size of the buffer.
 * @stream: Pointer to the file stream.
 *
 * Return: The number of characters read, or -1 on failure.
 */

ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
    int c;
    ssize_t read_chars = 0;
    size_t buffer_size = *n;
    char *line = *lineptr;

    if (line == NULL || buffer_size == 0) {
        buffer_size = 128;
        line = (char *)malloc(buffer_size);
        if (line == NULL) {
            perror("Memory allocation error");
            return -1;
        }
    }

    while ((c = fgetc(stream)) != EOF) {
              char *temp;

        if (read_chars >= (ssize_t)(buffer_size - 1)) {
            buffer_size *= 2;
            temp = (char *)realloc(line, buffer_size);
            if (temp == NULL) {
                perror("Memory allocation error");
                free(line);
                return -1;
            }
            line = temp;
        }

        line[read_chars++] = c;

        if (c == '\n') {
            break;
        }
    }

    if (read_chars == 0) {
        free(line);
        return -1;
    }

    line[read_chars] = '\0';
    *lineptr = line;
    *n = buffer_size;

    return read_chars;
}