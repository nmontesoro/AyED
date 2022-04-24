#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include "myStrings.h"

int has_uppercase_chars(const char *str)
{
    return has_chars_between(str, 64, 91);
}

int has_lowercase_chars(const char *str)
{
    return has_chars_between(str, 96, 123);
}

int has_numeric_chars(const char *str)
{
    return has_chars_between(str, 47, 58);
}

int has_special_chars(const char *str)
{
    /* Los caracteres especiales según el ejercicio 4 son:
    !".$\%&/()=|@#~?'+-_^{}[]*,:;<>\
    Toman los valores 33-47, 58-64, 91-95 y 123-126.

    Faltaría el caracter ¬, no lo codeo porque no es parte del charset
    ASCII.
    */

    return (has_chars_between(str, 33, 47) ||
            has_chars_between(str, 58, 64) ||
            has_chars_between(str, 91, 95) ||
            has_chars_between(str, 123, 126));
}

int has_chars_between(const char *str, const int lower, const int upper)
{
    int match = 0,
        i = 0,
        char_code = 0;

    while (match == 0 && (char_code = (int)str[i++]) != '\0')
    {
        match = (char_code > lower && char_code < upper);
    }

    return match;
}

int str_length(const char *str)
{
    int length = 0;

    /* Checks for '\0' while adding 1 to length */
    while (str[length++]){}

    return length;
}

int count_words(const char *str, const char *sep)
{
    /* Asume que no hay un caracter separador al final del string.
       No tiene en cuenta qué ocurre si hay dos separadores juntos. */
    
    int words = 0,
        i = 0,
        j = 0;

    char current_char = ' ',
         current_sep = ' ';
    
    while ((current_char = str[i++]))
    {
        while ((current_sep = sep[j++]))
        {
            if (current_char == current_sep)
            {
                words++;
            }
        }
        j = 0;    
    }

    return words + 1; 
}