#include <stdio.h>

void exampleFunction(float result) {
    printf("The result is: %f\n", result);
}

int main() {
    unsigned int array[2] = {57000, 57000};
    unsigned int sum = array[0] + array[1];
    float average = sum / 2.0;  // Ensure to use 2.0 to perform floating-point division
    exampleFunction(average);
    return 0;
}
