#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MALLOC_ERROR_CODE -1

typedef struct DListNode {
	char *str;
	unsigned short frequency;
	struct DListNode *next;
}DListNode;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

unsigned int InsertToArrayOfLists(char* word, char freq, DListNode ***word_lists_arr);
void checkWordInList(char* word, char freq, DListNode* lst, int* size, int index, DListNode*** word_lists_arr);
void deleteFromList(DListNode* cell_to_del, DListNode* prev, BOOL* list_is_empty, int index, DListNode** word_lists_arr);
void addToList(char* word, char freq, DListNode** lst, DListNode*** word_lists_arr, int index);
DListNode* allocateCell(char* word, unsigned short freq);
void allocationCheck(void *pointer);