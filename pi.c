#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n, i;
    int x = 1;

	// Getting an input for n and saving it as variable n
    printf("n = ");
	scanf("%d", &n);

	double pi = 0.;
	// We will use a while loop to find pi based on the inputted nth value 
    for (int i = 0; i <= n; ++i)
    {

        
        int exponenet = i;
        // Use formula to calculate value
        double val = (4.00/(8*i + 1) - 2.00/(8*i + 4) - 1.00/(8*i + 5) - 1.00/(8*i + 6))* (1.00/(x));  
        // Add value to pi
        pi += val;
        // Calculate the power with i as the exponent
        x *= 16;
    };
    // Print PI 
	printf("PI = %.10f\n", pi);
	return 0;
}
