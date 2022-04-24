/* Fraction data type */
typedef struct
{
    int numerator;
    int denominator;
} fraction_t;

/* Initializes a fraction type */
fraction_t *fraction_init(int numerator, int denominator);

/* Returns the denominator from a fraction data type */
int fraction_get_denominator(fraction_t *frac);

/* Returns the numerator from a fraction data type */
int fraction_get_numerator(fraction_t *frac);

/* Returns a simplified fraction */
fraction_t *fraction_simplify(fraction_t *frac);

/* Returns the result of adding two fractions */
fraction_t *fraction_add(fraction_t *frac1, fraction_t *frac2);

/* Returns the result of subtracting two fractions */
fraction_t *fraction_subtract(fraction_t *frac1, fraction_t *frac2);

/* Returns the result of multiplying two fractions */
fraction_t *fraction_multiply(fraction_t *frac1, fraction_t *frac2);

/* Returns the result of dividing two fractions */
fraction_t *fraction_divide(fraction_t *frac1, fraction_t *frac2);

/* Returns the string representation of a fraction */
char *fraction_to_string(fraction_t *frac);

/* Returns the floating point representation of a fraction */
float fraction_to_float(fraction_t *frac);

/* Returns true if two fractions are equal */
int fraction_compare(fraction_t *frac1, fraction_t *frac2);