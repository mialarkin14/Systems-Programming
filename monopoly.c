#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//There will be m players in an array
typedef struct Player 
{
	int id;		// This is the ID of the player that ranges from 0 to m-1 
	int loc;	// This is the location of the player that ranges from 0 to n-1
	long balance;	// This is the money balance of the player 
} TPlayer;

//There will be n properties in an array
typedef struct Property
{
	int id;		// This is the id of the property that ranges 0 to n-1
	int owner_id;	// This is the id of the player who owns this property 
	int rent;		 /* This is the cost to rent this property which increase linearly
						Property 0 is $1, Property 1 is $2, Property n-1 is $n */
} TProperty;


//TODO
//Implement the following function
//The player p1 needs to pay the player p2 'amount' of dollars
//If p1 has enough balance, the transaction will be successful
//Otherwise, p1 will pay all of their balance to p2, and the transaction
//is not successful 
//return 1 if the transaction is successful
//Otherwise, return 0
int transaction(TPlayer *p1, TPlayer *p2, int amount)
{
	// If player p1 has enough money to pay player p2 then...
	if (p1->balance >= amount){
		p1->balance = p1->balance - amount;		// Player p1's balance gets subtracted the amount
		p2->balance = p2->balance + amount;		// PLayer p2's balance gets added the amount
		return 1;			// The desired transaction has been successful 
	} 
	// If player p1 does not have enough money 
	else
	{
		p2->balance = p2->balance + p1->balance;		// Player p2 gets all of player p1's balance
		p1->balance = 0;		// Player p1 has no balance anymore
		return 0;			// The desired transaction has not been successful 
	}

}

//TODO
//Finish the following function
//If one player cannot pay rent to another player, this function should return 0. 
//The rest of the players will not get chances to play when this happens 

int one_round(int m, int n, TPlayer p[], TProperty prop[])
{
	// Essentially, we play the game and check try to do a transaction and it fails, end the game
	// i is the id of the player so for each player we are doing the following
	for(int i = 0; i < m; i++)
	{
		int steps = rand() % 6 + 1 + rand() % 6 + 1;	// Rolling the dice
		
		// If the player's current location is at the end of the board or close to the end of the board
		// go to the beginning of the board and give money 
		int locat = (p[i].loc + steps) % n;
		if (p[i].loc + steps >= n){
			// This means that we go back to zero to add money to player
			p[i].balance += n;
		}
		p[i].loc = locat;
	
		
		// if ((p[i].loc + steps) >= (n-1))
		// {
		// 	// Steps left till the end is x
		// 	int x = (n-1) - p[i].loc;  
		// 	// Steps added from initial 
		// 	int y = steps - x;
		// 	p[i].loc = y; 
		// 	p[i].balance += n;
		// }
		// // Otherwise we can simply add the steps to the player's location to put get them to a new location
		// else
		// {
		// 	p[i].loc = p[i].loc + steps;
		// }
		
		//Get the property at the player's location and the person it belongs to
		int owner = prop[p[i].loc].owner_id;
		
		// Check if it belongs to the player. If it does we're fine!
		// If the property belongs to no one, the player now owns it
		if (owner <0)
		{
			prop[p[i].loc].owner_id = p[i].id; 
		}
		// If the property is not owned by the person time to pay rent
		else if (owner != p[i].id)
		{
			// If the transaction is not successfull end the game and don't let other people play
			if (transaction(&p[i], &p[owner], prop[p[i].loc].rent) == 0)
				{
					return 0;
					break; 
				} 
		}

	}
	return 1;
}

//used for printing out results
void print_result(int m, TPlayer p[])
{
	printf("      id    balance\n");
	for(int i = 0; i < m; i++)
	{
		printf("%8d %8ld\n", p[i].id, p[i].balance);
	}
	long sum = 0;
	long max = 0;
	for(int i = 0; i < m; i++)
	{
		sum += p[i].balance;
		if(p[i].balance > max) max = p[i].balance;
	}
	printf("average: %f max: %ld, max/average = %lf\n", (double)sum/m, max, (double)max*m/sum); 
}

//max_rounds is needed because the game may never finish
void monopoly(int m, int n, TPlayer p[], TProperty prop[], int max_rounds)
{
	srand(12345);
	int rounds = 1;
	while(one_round(m, n, p, prop) && rounds < max_rounds)
	{
        rounds ++;
	}

	print_result(m, p);
	printf("after %d rounds\n", rounds);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Usage: %s m n rounds\n", argv[0]);
		return -1;
	}
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int rounds = atoi(argv[3]);
	assert(n >= 13);
	assert(m >= 1);
	assert(rounds >= 1);
	
	TPlayer p[m];
	TProperty prop[n];

	for(int i = 0; i < n; i++)
	{
		prop[i].id = i;
		prop[i].owner_id = -1;
		prop[i].rent = i + 1;
	}

	for(int j = 0; j < m; j++)
	{
		p[j].id = j;
		p[j].loc = 0;
		p[j].balance = n;
	}
	monopoly(m, n, p, prop, rounds);
	return 0;	
}