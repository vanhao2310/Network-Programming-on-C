#include<stdio.h>
#include<time.h>

int numstep = 1000000000;

int main() {
    double x, sum = 0;
    double step = 1.0/numstep;

    struct timespec start, stop;
    for(int i = 0; i < numstep; i++) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x*x);
    }
    double pi = sum * step;
    printf("Pi = %.10f\n", pi);
    return 0;
}