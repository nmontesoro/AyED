#include <stdio.h>
#include "../libs/myStrings.h"
#define SEP "\".,:;'?()[]{}\t "

int main(int argc, char const *argv[])
{
    char *sentence = NULL;

    for (int i = 1; i < argc; i++)
    {
        sentence = argv[i];
        fprintf(stdout, "'%s': %i words.\n", sentence, count_words(sentence, SEP));
    }
    
    return 0;
}
