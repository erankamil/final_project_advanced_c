#pragma once
#include "tree.h"
#include "list.h"
#include <stdio.h>


static void getNumOfWordsRec(WordTreeNode* node, unsigned short* num_of_words);


//This function gets word and insert the word to the tree
unsigned int InsertToWordTree(char *word, unsigned char freq, WordTree *tree)
{ //Q2
	WordTreeNode *curr = tree->root; //pointer to the tree root
	int index_str = 0, index_array = 0, size = 0;
	unsigned int res = 0;
	char ch;
	int length = strlen(word); //get the word length
	while (word[index_str] != '\0') // runs on the string, and check every letter
	{
		ch = word[index_str]; //current char
		res = checkIfCharExist(ch, curr->children, &curr, &index_array, &size); //check if the char exist in the children array
		if (res == 0) //char isn't exist in the array
		{
			AddWordToTree((word + index_str), freq,&curr, length, index_array, size);
			curr = curr->children[index_array]; //update curr to the current node that just added
			index_array = 0; //for the next letters, node doesn't have children
			size = 0;

			res = freq; //return new word frequency
		}
		else if ((res == 1) && (length == 1)) //the word already exist, update frequency
		{
			curr->frequency += freq;
			res = curr->frequency; //update exist word frequency
		}
		index_str++; //increment string
		length--;
	}
	return res;
}
void ConvertWordListArrToWordTree(DListNode **Word_list_arr, WordTree *tree)
{ //Q3
	tree->root = createTreeNode(' ', 0); //create tree root
	DListNode *curr;
	int index_arr = 0;
	char *str;
	unsigned char freq;

	while (Word_list_arr[index_arr]) //runs on the word list array
	{
		curr = Word_list_arr[index_arr]; //pointer to the head of the current list in the array
		while (curr) //runs on the current list
		{
			str = curr->str;   //current word in list
			freq = (unsigned char)curr->frequency; //current frequency
			InsertToWordTree(str, freq, tree); //insert word to tree
			curr = curr->next; //next word in list
		}
		index_arr++; //next pointer in the array
	}
}
//This function checks if the given char is already exist in the array
unsigned int checkIfCharExist(char ch, WordTreeNode **children_array, WordTreeNode **nextNode, int *index, int *size)
{
	unsigned int i = 0, res = 0, index1 = 0;
	while ((children_array[i] != NULL) && (res == 0) && (children_array[i]->ch <= ch))//runs on the children array, array is sorted
	{
		if (ch == children_array[i]->ch)  //checks if same char
		{
			res = 1; //found same char
			*nextNode = children_array[i]; //return the relevant node
		}
		else
			index1++;
		i++; //keep running on the children array any way
	}
	*index = index1; //return the relevant index to insert after
	while ((children_array[i] != NULL) && (res == 0)) //continue loop to to get the array size //only in case char isn't exist
	{
		i++;
	}
	*size = i;
	return res;
}
WordTreeNode *createTreeNode(char ch, unsigned short freq) 
{
	WordTreeNode *newNode = (WordTreeNode *)malloc(sizeof(WordTreeNode)); //allocate node
	allocationCheck(newNode); //check allocation successfully
	newNode->children = (WordTreeNode **)malloc(sizeof(WordTreeNode *));
	allocationCheck(newNode->children); //check allocation successfully
	newNode->children[0] = NULL;
	newNode->ch = ch;
	newNode->frequency = freq;
	return newNode;
}
//this function gets a word and add the word to the tree
void AddWordToTree(char *word, unsigned char freq, WordTreeNode **curr, int length, int index, int size)
{
	int i = 0;
	WordTreeNode *helper = *curr;
	helper->children = (WordTreeNode*)realloc(helper->children, sizeof(WordTreeNode) *(size + 1)); //increase helper with bigger size by 1
	allocationCheck(helper->children); //check allocation successfully
	size++; //increase size by one
	WordTreeNode *newNode = createTreeNode(word[i], 0); //create new node, frequency is zero
	if (length == 1)
		newNode->frequency = freq; //for the last char in the word, update to the given frequency
	helper->children[size] = newNode; //add the new node to the end
	for (i = size; i > index; i--) //swap in accordance to the given index
		swapTreeNodes(&helper->children[i - 1], &helper->children[i]);
	*curr = helper;
}
void swapTreeNodes(WordTreeNode **word1, WordTreeNode **word2)
{ 
	WordTreeNode *temp;
	temp = *word1;
	*word1 = *word2;
	*word2 = temp;
}
unsigned short getNumOfWords(WordTree tr)
{ // the function gets the number of words in the word tree
	unsigned short res = 0;
	if (tr.root)
	{
		int i = 0;
		while (tr.root->children[i]) // send each subtree(root child) to the recursive function
		{
			getNumOfWordsRec(tr.root->children[i], &res);
			i++;
		}
	}
	return res;
}
static void getNumOfWordsRec(WordTreeNode* node, unsigned short* num_of_words)
{ // the function gets the number of words in the current subtree of the root child
	if (node->children[0] == NULL) // a leaf
	{
		*num_of_words += 1;
		return;
	}
	else
	{
		if (node->frequency != 0) // end of word
			*num_of_words += 1;
		for (int i = 0; node->children[i] != NULL; i++) // recursive calls for the children of the current node
			getNumOfWordsRec(node->children[i], num_of_words);
	}
}