#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <stdio.h>
#include "tree.h"


#define COMPRESS_VALUE(A) (((A)>=97 && (A)<=122)? (A-97):(A-39))
#define CHAR_TO_INT(C) ((C)-'0')
#define BITS_IN_BYTE 8

char* getWordFromFile(FILE* file);
short getFreqFromFile(FILE* file);
void getNegativeFreq(FILE* file, short* freq);
void getWordsFromTreeAndWriteToFile(WordTree tr, unsigned short num_of_words, FILE* file);
void writeCompressedWordTree(WordTree tr, char* fname);
void compressAndWriteWordToFile(char* word, unsigned short freq, FILE* file, unsigned char* buffer, int* bits_left);
void writeBitsToBuffer(FILE* file, unsigned char* buffer, unsigned char letter_to_write, int* bits_left, int bits_to_write);
void WriteCompressdFileFromTextFile(char* iname, char* oname);
void CompressAndWriteArrToFile(DListNode** word_lists_arr, FILE* file);