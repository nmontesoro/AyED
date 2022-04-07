#include <stdio.h>
#include <string.h>

int check_username(char const *username);

int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        char *pwd = argv[i];
        fprintf(stdout, "Password '%s' is %s.\n", pwd, check_username(pwd) ? "valid" : "invalid");
    }

    return 0;
}

int check_username(char const *username)
{
    int has_upper = 0,
        has_lower = 0,
        has_numbers = 0,
        has_spc_char = 0;

    for (int i = 0; i < strlen(username); i++)
    {
        int char_code = (int) username[i];
        
        if (char_code > 47 && char_code < 58) {has_numbers = 1;}
        if (char_code > 64 && char_code < 91) {has_upper = 1;}
        if (char_code > 96 && char_code < 123) {has_lower = 1;}
        if (char_code == 95 || char_code == 45) {has_spc_char = 1;}
    }

    return (has_upper && has_lower && has_numbers && has_spc_char);
}
