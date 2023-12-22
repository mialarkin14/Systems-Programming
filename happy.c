#include <stdio.h>
#include <stdlib.h>

int main() {
int n;
    printf("n = ");
    scanf("%d", &n);
int m = n;
int rem;
int sum; 
	// While sum does not equal 1 or 4
	while (sum != 1 && sum != 4)
	{
		// Set sum initially to 0
		sum = 0;
		// While the number inputted is not zero
		while (n > 0)
		{
			// Find the remainder of the number
			rem = n%10;
			// Set the sum to be the square of the remainder plus prevoius sum 
			sum = sum + (rem*rem);
			// "Floor" divide the number 
			n = n/10;
		} 
	// Set number equal to the sum
	n = sum;
	printf ("%d\n", n);
	};

	// If the modified number equals 1, it is a happy number! 
    if(n==1) printf("%d is a happy number.\n", m);
    
	// Else, the number is not happy :(
	else printf("%d is NOT a happy number.\n", m);
    return 0;
}
