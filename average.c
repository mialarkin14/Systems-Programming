#include <stdio.h>

int main()
{
    // Initializing variables
    double x;
    double total;
    double average;
    double counter;

    // While there is no input:
    while (scanf("%lf", &x) == 1)
        {
            // Add the double to the total
            total += x;
            // Increase counter for number of inputted numbers
            counter += 1;
            // Calculate the average
            average = total/counter;
            // Print out the total and average after loop is exited
            printf("Total=%f Average=%f\n", total, average);
            
        }

   

}