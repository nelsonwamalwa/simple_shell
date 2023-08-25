#include "shell.h"

/**
 * _getline - Wrapper function for the getline function.
 * @lineptr: Pointer to the buffer that will store the line.
 * @n: Pointer to the size of the buffer.
 * @stream: Pointer to the file stream.
 *
 * Return: The number of characters read, or -1 on failure.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) {
        errno = EINVAL;
        return -1;
    }

    if (*lineptr == NULL || *n == 0) {
        *n = BUFFER_SIZE;
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }

    size_t pos = 0;
    int c;

    while (1) {
        c = fgetc(stream);
        
        if (c == EOF) {
            if (pos == 0) {
                return -1; // No more lines to read
            } else {
                (*lineptr)[pos] = '\0';
                return pos; // Return the length of the line
            }
        }

        if (pos + 1 >= *n) {
            *n *= 2;
            char *new_lineptr = (char *)realloc(*lineptr, *n);
            if (new_lineptr == NULL) {
                return -1; // Error
            }
            *lineptr = new_lineptr;
        }

        (*lineptr)[pos] = c;
        pos++;

        if (c == '\n') {
            (*lineptr)[pos] = '\0';
            return pos; // Return the length of the line
        }
    }
}