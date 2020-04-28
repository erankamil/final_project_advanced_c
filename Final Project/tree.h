#pragma once
#include "list.h"

typedef struct _WordTreeNode
{
	char ch;
	unsigned short frequency;
	struct _WordTreeNode **children;
} WordTreeNode;

typedef struct _WordTree
{
	WordTreeNode *root;
} WordTree;


WordTreeNode *createTreeNode(char ch, unsigned short freq);
unsigned int InsertToWordTree(char* word, unsigned char freq, WordTree* tree);
void ConvertWordListArrToWordTree(DListNode **Word_list_arr, WordTree *tree);
unsigned int checkIfCharExist(char ch, WordTreeNode** children_array, WordTreeNode** nextNode, int* index, int* size);
void AddWordToTree(char *word, unsigned char freq, WordTreeNode **curr, int length, int index, int size);
void swapTreeNodes(WordTreeNode **word1, WordTreeNode **word2);
unsigned short getNumOfWords(WordTree tr);

