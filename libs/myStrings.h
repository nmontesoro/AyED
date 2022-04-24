/* Returns 1 if the specified string contains any uppercase 
characters */
int has_uppercase_chars(const char* str);

/* Returns 1 if the specified string contains any lowercase 
characters */
int has_lowercase_chars(const char *str);

/* Returns 1 if the specified string contains any numeric 
characters */
int has_numeric_chars(const char *str);

/* Returns 1 if the specified string contains characters between two 
ASCII codes */
int has_chars_between(const char *str, const int lower, const int upper);

/* Returns 1 if the specified string contains any special characters 
(ver ejercicio 4, guia 1) */
int has_special_chars(const char *str);

/* Returns the length of the specified string */
int str_length(const char *str);

/* Returns the number of words in str, separated by sep */
int count_words(const char *str, const char *sep);