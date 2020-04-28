#include "arr.h"
#include "files.h"
#include "list.h"
#include <stdio.h>

static void addToInerPlaceOfArr(int place, DListNode** word_lists_arr, int size);
static int findPlaceToInsertInArr(char lastLetter, DListNode** word_lists_arr, int* size);
static void addTobeginningOfArr(DListNode** word_lists_arr, int size);
static void addToEndOfArr(DListNode** word_lists_arr, int size);
static void addToEmptyArr(DListNode** word_lists_arr, int* size);
static void expandArr(DListNode*** word_lists_arr, unsigned int* size);


unsigned int InsertToArrayOfLists(char* word,  char freq, DListNode ***word_lists_arr)
{ //Q1 
	char last_char_in_str = word[strlen(word) - 1]; //get the last char of the given word
	unsigned int index, size = 0;
	//checks if the last char of the given word had already a list in the array
	//if so, returns the relevant list
	DListNode* lst = checkWordInArr(last_char_in_str, *word_lists_arr, &size, &index);
	if (lst) //means-> the list is exist
		//check where to insert the word in the list and update the list (freq/delete if needed)
		checkWordInList(word, freq, lst, &size, index, word_lists_arr);
	else //means ->the word doesnt exist
	{
		if (freq > 0) //add a new list with the given word to the array 
			addToArr(word, freq, word_lists_arr, &size);
		else // word does not exist and freq is <= 0
			fprintf(stderr, "ERROR:word does not exist and freq <= 0 ! \n");
	}
	return size; //return the number of list in the array
}
void ReadFileToWordLists(char* fname, DListNode*** word_lists_arr)
{	//Q4 , the function reads the words and their frequency from the file and adds them to the array according to Q1 rules
	char* word;
	unsigned int log = 1, phy = 2;
	short freq;
	FILE* file = fopen(fname, "r");
	allocationCheck(file);
	while (!feof(file)) // runs all over the file
	{
		if (log == phy)
			expandArr(word_lists_arr, &phy);
		word = getWordFromFile(file); // get the word from the file
		freq = getFreqFromFile(file); // get the frequency from the file
		log = InsertToArrayOfLists(word, freq, word_lists_arr);  // creating array of lists with the current word and freq
	}
	*word_lists_arr = (DListNode**)realloc(*word_lists_arr, sizeof(DListNode*)*log);
	fclose(file);
}
void addToArr(char* word, short freq, DListNode*** word_lists_arr, int* size)
{ // the function adds a new cell to the array of lists
	*size = *size + 1; //update size
	DListNode *lst_to_add = allocateCell(word, (unsigned short)freq); //allocate new array cell
	*word_lists_arr = (DListNode**)realloc(*word_lists_arr, sizeof(DListNode*)*(*size)); //realloce the array with the new size
	allocationCheck(*word_lists_arr); //check allocation successfully
	DListNode** helper = *word_lists_arr; // using helper pointer 
	helper[*size - 1] = lst_to_add; //adds the new list cell to the end of the array
	if (helper[0] == NULL) // array is empty
		addToEmptyArr(helper, size);
	else
	{
		char last_let = word[strlen(word) - 1]; //last letter of word
		int p = findPlaceToInsertInArr(last_let, helper, size); //find place to insert to the array
		if (p == -1) // adds to beginning of array
			addTobeginningOfArr(helper, *size);
		else if (p == *size - 3) // adds to end of array
			addToEndOfArr(helper, *size);
		else // adds to inner place of the array
			addToInerPlaceOfArr(p, helper, *size);
	}
	*word_lists_arr = helper;
}
static void addToEmptyArr(DListNode** word_lists_arr, int* size)
{ // the function swaps the new cell we added with the first cell(the NULL cell)
	swapArrCells(&word_lists_arr[0], &word_lists_arr[1]);
}
static int findPlaceToInsertInArr(char lastLetter, DListNode** word_lists_arr, int* size)
{ // the function returns the index that after thay we want to add out new cell
	int prev = -1, index = 0, found = 0;
	while ((index < *size - 2) && (found == 0)) //runs on the array cell until the NULL cell
	{
		char* str = word_lists_arr[index]->str;
		char last_curr_let = str[strlen(str) - 1]; // get the last letter of the current list
		if (last_curr_let < lastLetter) //keep running on the array while current last letter is smaller than given last letter
			prev++;
		else
			found = 1; //stop the loop
		index++;
	}
	return prev;
}
static void addTobeginningOfArr(DListNode** word_lists_arr, int size)
{ // the function swaps the new list we entered to the end of the array to the the beginning of the array
	for (int i = size - 1; i > 0; i--)
		swapArrCells(&word_lists_arr[i], &word_lists_arr[i - 1]);
}
static void addToEndOfArr(DListNode** word_lists_arr, int size)
{ // the function swaps the new list we entered to the end of the array with the NULL cell
	swapArrCells(&word_lists_arr[size - 1], &word_lists_arr[size - 2]); //move Null to the end of the array
}
static void addToInerPlaceOfArr(int place, DListNode** word_lists_arr, int size)
{ // the function swaps the new list we entered to the end of the array ,after the index (place)
	for (int i = size - 1; i > place + 1; i--)
	{
		swapArrCells(&word_lists_arr[i], &word_lists_arr[i - 1]);
	}
}
DListNode* checkWordInArr(char ch, DListNode** word_lists_arr, unsigned int* size, unsigned int* index)
{ // the function returns the cell in arr that points to the list with the same ending letter in her cells
	unsigned int i = 0;
	DListNode *res = NULL;
	while (word_lists_arr[i]) //runs on the array until Null
	{
		if (ch == word_lists_arr[i]->str[strlen(word_lists_arr[i]->str) - 1]) //compare the last letters
		{
			res = word_lists_arr[i];
			*index = i; // change the value of *index to the index cell in the arr we found the same end letter
		}
		i++;
	}
	*size = i + 1; // returns the size of the arr //keep running on the array anyway to find out his size
	return res;
}
void deleteFromArr(int index, unsigned int* size, DListNode*** word_lists_arr)
{ // the function deletes a cell from the array by swapping to the end and reallocating
	DListNode** helper = *word_lists_arr; // usings helper pointer
	for (int i = index; i < *size - 1; i++)
	{
		swapArrCells(&(helper[i]), &(helper[i + 1])); //moves the cell we want to delete to the end
	}
	*word_lists_arr = helper;
	*word_lists_arr = (DListNode*)realloc(*word_lists_arr, sizeof(DListNode)*(*size - 1)); // reallocate to the new size
	allocationCheck(*word_lists_arr); //checks allocation successfully
	*size = *size - 1; //update size
}
void freeWordListsArr(DListNode** arr)
{ // the function get an arr (DListNode**) and frees each cell and his string in each list of the array
	DListNode* saver, *curr;
	int i;
	for (i = 0; arr[i]; i++)
	{
		curr = arr[i];
		while (curr)
		{
			saver = curr->next; // free each cell in the current list
			free(curr->str);
			free(curr);
			curr = saver;
		}
	}
	free(arr); // free the array
}
static void expandArr(DListNode*** word_lists_arr, unsigned int* size)
{ // the function multiply the size of the array and  reallocting the array 
	*size *= 2;
	*word_lists_arr = (DListNode**)realloc(*word_lists_arr, sizeof(DListNode*)*(*size));
	allocationCheck(*word_lists_arr);
}
void swapArrCells(DListNode** lst1, DListNode** lst2)
{
	DListNode* temp;
	temp = *lst1;
	*lst1 = *lst2;
	*lst2 = temp;
}
void getNumOfWordsFromArr(DListNode** word_lists_arr, unsigned short* num_of_words)
{ // the function gets an arr and returns the size of the arr and the number of words 
	int i = 0;
	while (word_lists_arr[i]) // runs over the arr cells until the NULL cell
	{
		DListNode* curr = word_lists_arr[i];
		while (curr) // runs over each list 
		{
			*num_of_words += 1;
			curr = curr->next;
		}
		i++;
	}
}