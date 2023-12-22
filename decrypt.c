#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

//Using these two global variables can be justified:
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int word_count = 0;                    //number of words, initilized to 0

//Note the words in the dictionary file are sorted
//This fact could be useful
void read_file_to_array(char *filename)
{
    FILE *fp;

    //open the file for reading
    fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }
    //make sure when each word is saved in the array words,
    //it does not ends with a '\n'
    //see how this is done using fscanf 
    while(!feof(fp))
        fscanf(fp, "%s\n", words[word_count++]);
    fclose(fp);
}

//TODO
//Test wether a string word is in the dictionary
//Return 1 if word is in the dictionary
//Return 0 otherwise
//Be efficient in implementing this function
//Efficiency is needed to pass test cases in limited time
int in_dict(char *word)
{
// Use a binary search 
// We need to go through conducting a binary search in terms of 1st letter, 2nd letter, 3rd letter, etc.
int start = 0;
int end = 60000;

while (start <= end)
{
    int middle = (start + end)/2;
    // We found the word! Return 1
    if (strcmp(word, words[middle]) == 0)
    {
        return 1;
    }
    // If the word is in the right half, go to the right half
    if (strcmp(word, words[middle]) > 0)
    {
        start = middle + 1;
    }
    // If the word is in the left half, go the the left half
    else if (strcmp(word, words[middle]) < 0)
    {
        end = middle - 1;
    }
}
// We did not find the word. Return 0
return 0;
}

//TODO
//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted

void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted)
{
// We know the encrytped so we do the opposite of the encrypted 
/*
void encrypt(char *message, unsigned char key, unsigned char shift, int fd)
{
    int len = strlen(message);
	printf("len = %d\n", len);
	int msg[len];

	for(int i = 0; i<len; i++)
		msg[i] = (message[i] << shift) ^ key;
	write(fd, msg, len*sizeof(int));
	printf("%s\n", (char *)msg);
}
*/

// Decrypted is an array
// Go through each and xor it and shift right
for (int j = 0; j < len/4; j++)
{
    decrypted[j] = (char)((encrypted[j]^key) >> shift);
}
}

//TODO
//calculate a score for a message msg
//the score is used to determine whether msg is the original message
int message_score(const char *msg)
{
    int count = 0;
    // Need to find each word in the message, #of words = #of spaces + 1
    // If the word is in the dictionary then increase the counter
    
    // Create a buffer that is the length of the message
    char buffer[strlen(msg)]; 
    // Copy the message into the buffer
    strcpy(buffer, msg);
    // Find the first word 
    char *word = strtok(buffer, " ");       // This will create a pointer to the characters in the word and stop at the space
    // While the word is not null
    while (word != NULL)
    {
        if (in_dict(word) == 1)
            {
            // If the word is in the dictionary, increment the count
            count++;
            }
        // This jumps to the next word
        word = strtok(NULL, " ");
    }   
    // Return the count
    return count;
}

//search using all the (key, shift) combinations
//to find the original message
//result is saved in message
void search(const int *encrypted, int len, char *message)
{
	char decrypted[MAX];

    int max_score = 0;
    strcpy(message, "");
    for(unsigned char k = 0; k < 255; k++)
    {
        for(unsigned char shift = 0; shift <= 24; shift++)
        {
            decryption(k, shift, encrypted, len, decrypted);
            int score = message_score(decrypted);
			if(score > max_score)
			{	
				max_score = score;
				strcpy(message, decrypted);
			}
        }
    }
}

//TODO
//read the encrypted message from the file to encrypted
//return number of bytes read
int read_encrypted(char *filename, int *encrypted)
{
    // Open the encrypted message file for reading 
    int fd = open(filename, O_RDWR);
    
    if(fd == -1)
    {
        // If the file is empty then print an error
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }

    // Getting the number of bytes
    int bytes = read(fd, encrypted, 10240);

    //Close the file
    close(fd);

    // Return the number of bytes
    return bytes;
    
}

//Do not change the main() function
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s encrypted-message\n", argv[0]);
		return 0;
	}

	read_file_to_array("dict.txt");
    int encrypted[MAX];
	int len = read_encrypted(argv[1], encrypted);

    char message[MAX];
    strcpy(message, "");
	search(encrypted, len, message);
	printf("%s\n", message);
	return 0;
}
