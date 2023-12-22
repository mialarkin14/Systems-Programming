#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{
	// Calculating bounds of square
	int z = (2*n+1); 
	// An array that holds all possible coordinates
	int track_coords[z][z]; 
	// Coordinates 
	int x = 0;
	int y = 0;
	// Variables to calculate new system
	int new_x = 0;
	int new_y = 0;
	int sum_unique = 0;

	// Iniltializing the array
	for (int i = 0; i < z; i++)
		{
			for (int a = 0; a < z; a++)
                {
                track_coords[i][a] = 0;
                };
		};

	// While we at not at an edge do: 
	while (x != -n && x != n && y != -n && y != n)
	{
        // Count the initial square 
        // Defining the square where the top left corner is 0,0 
        new_x = x + n;		// Converting coordinates to new square definition 
		new_y = y + n;
		track_coords[new_x][new_y] = 1;	// Changing any square hit, array value to 1
		//r = rand() % 4;
		//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.
        int r = rand()%4;
		if (r == 0)
			y += 1; 	// Go up
		if (r == 1)
			x += 1;		// Go right
		if (r == 2)
			y -= 1;		// Go down
		if (r == 3)
			x -= 1;		// Go left
	} 

	// Go through array and find the number of 0s changed to 1s
	for (int j = 0; j <=2*n; j++)
		{
			for (int f = 0; f <=2*n; f++)
            {
                if (track_coords[j][f] == 1)
				    sum_unique += 1;
            };
            
		}; 
	// Return fraction of the visited by total number of squares, not including the initial square 
	return (double)sum_unique/((2*n-1)*(2*n-1)); 
}

//Do not change the code below
int main(int argc, char* argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}
