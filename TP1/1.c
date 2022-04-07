#include <strings.h>
#include <stdio.h>

int count_chars(char const *sequence);

int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        fprintf(stdout, "'%s': %i punctuation character(s)\n", argv[i], count_chars(argv[i]));
    }

    return 0;
}

int count_chars(char const *sequence)
{
    int sequence_length = strlen(sequence), char_count = 0;

    for (int i = 0; i < sequence_length; i++)
    {
        char character = sequence[i];
        if (character == '.' || character == ',' || character == ';' || character == '?' || character == '!') {
            char_count++;
        }
    }

    return char_count;
}
