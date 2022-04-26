typedef struct
{
    double real;
    double im;
} complex_t;

/* Initializes a complex number */
complex_t *complex_init(double real, double imaginary);

/* Returns the real part of a complex number */
double complex_real(complex_t *num);

/* Returns the imaginary part of a complex number */
double complex_imaginary(complex_t *num);

/* Returns the sum of two complex numbers */
complex_t *complex_sum(complex_t *a, complex_t *b);

/* Returns the result of subtracting two complex numbers */
complex_t *complex_subtract(complex_t *a, complex_t *b);

/* Returns the result of multiplying two complex numbers */
complex_t *complex_multiply(complex_t *a, complex_t *b);

/* Returns the result of dividing two complex numbers */
complex_t *complex_divide(complex_t *a, complex_t *b);

/* Compares two complex numbers */
int complex_compare(complex_t *a, complex_t *b);

/* Returns the absolute value of a complex number */
double complex_abs(complex_t *a);

/* Returns the angle of a complex number (in radians) */
double complex_angle(complex_t *a);

/* Returns the string representation of a complex number */
char *complex_to_string(complex_t *a);

/**
 * @brief Calls a function that prints the value of a complex number
 * @param cpx Number
 * @param printfunct Function to be called. Must take two arguments: 
 * real and imaginary part (both of type double)
 * @return (void)
 */
void complex_print(complex_t *cpx, void (*printfunct)(double, double));
