#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//TODO
//Implement the below function
//Simulate one particle moving n steps in random directions
//Use a random number generator to decide which way to go at every step
//When the particle stops at a final location, use the memory pointed to by grid to 
//record the number of particles that stop at this final location
//Feel free to declare, implement and use other functions when needed

void one_particle(int *grid, int n)
{
// Particle starts at (0, 0, 0) and set a counter for number of steps taken
int x = 0;
int y = 0;
int z = 0;
int steps = 0;
int final_part_loc = 0;

// Initialize grid
// for (int i = 0; i < (2*n+1)*(2*n+1)*(2*n+1); i++)
// {
// 	grid[i] = 0;
// }
	
// While the number of steps is less than or equal to the desired moves, do the following:
while (steps < n)
{
	// Find a random integer called r between 0 and 6
	int r = rand() % 6;
	// If r is 0 or 1, move left or right respectivly
	if (r == 0)
	{
		x -= 1;
	}
	else if (r == 1)
	{
		x += 1;
	}
	// If r is 2 or 3, move up or down y-axis respctivly
	else if (r == 2)
	{
		y += 1;
	}
	else if (r == 3)
	{
		y -= 1;
	}
	// If r is 4 or 5, move up or down z-axis respctivly
	else if (r == 4)
	{
		z += 1;
	}
	else if (r == 5)
	{
		z -= 1;
	}
// Increment steps
steps += 1;
}
// Find the final location of the particle and input into array
final_part_loc = (x+n) + (y+n)*(2*n+1) + (z+n)*(2*n+1)*(2*n+1);
grid[final_part_loc] += 1;
}

//TODO
//Implement the following function
//This function returns the fraction of particles that lie within the distance
//r*n from the origin (including particles exactly r*n away)
//The distance used here is Euclidean distance
//Note: you will not have access to math.h when submitting on Mimir
double density(int *grid, int n, double r)
{
// Total number of particles within the sphere
int count = 0;
// Total is the total number of spaces the particle has visited
int total = 0;

// Go through the grid and find the total number of places the particle has visited
for (int x = -n; x <= n; x++)
{
	for (int y = -n; y <= n; y++)
	{
		for (int z = -n; z <= n; z++)
		{
			// Add the value to the total number of particles it has visited
			total += grid[(x+n) + (y+n)*(2*n+1) + (z+n)*(2*n+1)*(2*n+1)];
		
			//If the particle is inside the sphere or just as the edge of the sphere
			if (x*x + y*y + z*z <= (n*r)*(n*r))
			{
				// Get the number of times a particle has been there 
				count += grid[(x+n) + (y+n)*(2*n+1) + (z+n)*(2*n+1)*(2*n+1)]; 
			} 	
		}
	}
}
// Return the fraction of particles that lie in the distance of sphere
return (double)count/total;  
}

//use this function to print results
void print_result(int *grid, int n)
{
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
    {
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
    }
}

//TODO
//Finish the following function
//See the assignment decription on Piazza for more details
void diffusion(int n, int m)
{
	//fill in a few line of code below
	int *grid;
	int z = (2*n +1);
	// Allocating memory for the grid to be (2n+1)^3 with size of integer
	// Do this using calloc since calloc gives the memory and also assigns everything to zero
	grid = (int*)calloc(z*z*z,sizeof(int));
	for(int i = 1; i<=m; i++){
		one_particle(grid, n);
	}; 
	print_result(grid, n);
	//fill in some code below
	free(grid);
}

int main(int argc, char *argv[])
{
	
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	assert(n >= 1 && n <=50);
	assert(m >= 1 && m <= 1000000);
	srand(12345);
	diffusion(n, m);
	return 0;
}