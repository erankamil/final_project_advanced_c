#pragma once
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

static DListNode* findPlaceToInsertInList(char* word, DListNode* lst);
static void addToHeadOfList(DListNode* cell_to_add, DListNode** lst);
static void addToEndOfList(DListNode* cell_to_add, DListNode* prev);
static void addToInerPlaceOfList(DListNode* cell_to_add, DListNode* prev);

void checkWordInList(char* word, char freq, DListNode* lst, int* size, int index, DListNode*** word_lists_arr)
{ // the function checks if the word is exist in the list 
	BOOL list_is_empty = FALSE, isExist = FALSE;
	DListNode* prev = NULL, *curr = lst;
	while (curr && list_is_empty != TRUE && isExist != TRUE)
	{
		if (strcmp(word, curr->str) == 0) // we found the same word
		{
			isExist = TRUE;
			if (curr->frequency + freq <= 0) // we need to delete this cell in the list
			{
				deleteFromList(curr, prev, &list_is_empty, index, *word_lists_arr);
				if (list_is_empty) // the list is empty - we need to delete this cell in the array
					deleteFromArr(index, size, word_lists_arr);
			}
			else // we add the freq we have got to the current freq
				curr->frequency += freq;
		}
		prev = curr;
		curr = curr->next;
	}
	if (isExist == FALSE) // the word is not in the list
	{
		if (freq > 0) // add to the current list
			addToList(word, freq, &lst,word_lists_arr,index);
		else //  word does not exsit in the list and freq <=0 - error 
			fprintf(stderr, "ERROR:word does not exist and freq <= 0 ! \n");
	}
}
void deleteFromList(DListNode* cell_to_del, DListNode* prev, BOOL* list_is_empty, int index, DListNode** word_lists_arr)
{ // the function deletes cell from the list
	if (prev == NULL) // delete from head of list
	{
		prev = cell_to_del->next; // save the next cell of the cell we want to delete
		free(cell_to_del->str);
		free(cell_to_del);
		if (prev == NULL) // list is empty
			*list_is_empty = TRUE; // update that the list is now empty
		else // list is not empty need to update the array with the new head
			word_lists_arr[index] = prev; // update the array with the new head list
	}
	else // the list has  two cells at least
	{
		prev->next = cell_to_del->next;
		free(cell_to_del->str);
		free(cell_to_del);
		*list_is_empty = FALSE;
	}
}
void addToList(char* word, char freq, DListNode** lst,DListNode*** word_lists_arr,int index)
{ // the function adds a new cell to the list by lexicographic order
	DListNode* cell_to_add = allocateCell(word, freq);
	if (lst == NULL) // list is empty
		lst = cell_to_add;
	else
	{
		DListNode *p = findPlaceToInsertInList(word, *lst);
		if (p == NULL) // insert to the beggining of the list
		{
			addToHeadOfList(cell_to_add, lst);
			*word_lists_arr[index] = *lst; // update the array with the changed list ( since we change the head of the list)
		}
		else if (p->next == NULL) // adds to the end of the list
			addToEndOfList(cell_to_add, p);
		else // inner place in the list
			addToInerPlaceOfList(cell_to_add, p);
	}
}
static void addToHeadOfList(DListNode* cell_to_add, DListNode** lst)
{ // the function adds a new cell to the head of the list
	DListNode* prev_head = *lst;
	cell_to_add->next = prev_head;
	*lst = cell_to_add; // update the new head of the list
}
static void addToEndOfList(DListNode* cell_to_add, DListNode* prev)
{ // the function adds a new cell to the end of the list
	prev->next = cell_to_add;
	cell_to_add->next = NULL;
}
static void addToInerPlaceOfList(DListNode* cell_to_add, DListNode* prev)
{ // the function adds a new cell to inner place of the list
	cell_to_add->next = prev->next;
	prev->next = cell_to_add;
}
DListNode* allocateCell(char* word, unsigned short freq)
{ // the function allocates new cell 
	DListNode* cell_to_add = (DListNode*)malloc(sizeof(DListNode));
	allocationCheck(cell_to_add);
	cell_to_add->frequency = freq;
	cell_to_add->str = _strdup(word); // using strdup to allocate the cell_to_add->str pointer and copy the word
	allocationCheck(cell_to_add->str);
	cell_to_add->next = NULL;
	return cell_to_add;
}
static DListNode* findPlaceToInsertInList(char* word, DListNode* lst)
{ // the function returns a cell in the list that after him we want to enter our new cell
	DListNode* curr = lst, *prev = NULL;
	while (curr != NULL && (strcmp(curr->str, word) < 0))
	{
		prev = curr;
		curr = curr->next;
	}
	return prev;
}
void allocationCheck(void *pointer)
{ // the function check allocation for any pointer
	if (!pointer)
	{
		fprintf(stderr, "Allocation falid!\n");
		exit(MALLOC_ERROR_CODE);
	}
}