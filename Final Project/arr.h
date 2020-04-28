#pragma once
#include "list.h"

void addToArr(char* word, short freq, DListNode*** word_lists_arr, int* size);
void swapArrCells(DListNode** word1, DListNode** word2);
DListNode* checkWordInArr(char ch, DListNode** word_lists_arr, unsigned int* size, unsigned int* index);
void deleteFromArr(int index, unsigned int* size, DListNode*** word_lists_arr);
void freeWordListsArr(DListNode** arr);
void getNumOfWordsFromArr(DListNode** word_lists_arr, unsigned short* num_of_words);
