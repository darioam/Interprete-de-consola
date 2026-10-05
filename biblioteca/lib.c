#include <stdio.h>

// Factorial (iterativo)
double factorial(void *no) {
    int n = (int)(*(double*)no);
    if (n < 0) return -1; // Error para números negativos
    long resultado = 1;
    for (int i = 2; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

// Fibonacci (iterativo)
double fibonacci(void *no) {
    int n = (int)(*(double*)no);
    if (n < 0) return -1; // Error para números negativos
    if (n == 0) return 0;
    if (n == 1) return 1;

    long a = 0, b = 1, temp;
    for (int i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}