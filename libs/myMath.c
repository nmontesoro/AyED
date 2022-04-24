int myGCD(int a, int b)
{
    int dividend = 0,
        divisor = 0,
        remainder = 0,
        gcd = 0;

    if (a > b)
    {
        dividend = a;
        divisor = b;
    } else {
        dividend = b;
        divisor = a;
    }

    remainder = dividend % divisor;

    if (remainder != 0)
    {
        gcd = myGCD(divisor, remainder);
    } else {
        gcd = divisor;
    }

    return gcd;
}