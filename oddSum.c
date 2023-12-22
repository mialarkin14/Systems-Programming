#include <stdio.h>
#include <stdlib.h>

int oddSumHelp(int count, int bound, int value)
{
if (count == 0)		// Base case 
	{
		if (value != 0)
		{
			return 0; 	// This means there is no solution so we return 0
		}	

		if (value == 0)
		{
			return 1;		// There is a solution!
		}
			
    }

// For loop finds all of the odd numbers that are in bound
for(int j = bound; j >= 1; j -=2){
	// If the value is greater than the j (bound)
	if (value >= j){
		// If there is a solution stop 
		if (oddSumHelp(count -1, j -2, value -j))
		{
			printf("%d ", j);
			return 1;
		}
	}
}
return 0;	
}

//Do not change the code below
void oddSum(int count, int bound, int value)
{
    	if(value <= 0 || count <= 0 || bound <= 0) return;
    
    	if(bound % 2 == 0) bound -= 1;

    	if(!oddSumHelp(count, bound, value)) printf("No solutions.\n");
	else printf("\n");
}

int main(int argc, char *argv[])
{
	if(argc != 4) return -1;

	int count = atoi(argv[1]);
	int bound = atoi(argv[2]);
	int value = atoi(argv[3]);

	//oddSum(12,30,200);
	//oddSum(10,20,100);
	//oddSum(20,20,200);
	oddSum(count, bound, value);
	return 0;
}