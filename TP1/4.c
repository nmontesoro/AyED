#include <stdio.h>
#include "../libs/myStrings.h"

/* Returns 1 if str is a strong password */
int is_strong_password(char const *str);

int main(int argc, char const *argv[])
{
    char *pwd = NULL;

    for (int i = 1; i < argc; i++)
    {
        pwd = argv[i];
        fprintf(stdout, "'%s' is%s a strong password.\n", pwd, is_strong_password(pwd) ? "" : " NOT");
    }

    return 0;
}

int is_strong_password(char const *str)
{
    return (str_length(str) > 8 &&
            has_lowercase_chars(str) &&
            has_uppercase_chars(str) &&
            has_numeric_chars(str) &&
            has_special_chars(str));
}
