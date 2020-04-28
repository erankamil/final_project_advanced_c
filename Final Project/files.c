#include "files.h"
#include "arr.h"
#include "tree.h"

static void expandString(char** word, int* phy);
static void getWordsFromRootChildAndWriteToFile(WordTreeNode* node, char* word, unsigned int log_str, unsigned int* phy_str, unsigned char* buffer, int* bits_left, FILE* file);


void writeCompressedWordTree(WordTree tr, char* fname)
{ //Q5
	if (tr.root)
	{
		FILE* file = fopen(fname, "wb");
		allocationCheck(file);
		unsigned short num_of_words = getNumOfWords(tr);  // get the number of word in the tree
		getWordsFromTreeAndWriteToFile(tr, num_of_words, file); // gets the words from the tree and their freq,compress them and write to file
		fclose(file);
	}
	else // tree is empty
		printf("Error : tree is empty! \n");
}
void getWordsFromTreeAndWriteToFile(WordTree tr, unsigned short num_of_words, FILE* file)
{ // the function running all over the tree and write to file each word and freq afer compression
	unsigned char buffer = 0x00; // using buffer for the bits who left in the compression
	int bits_left = 8;
	fwrite(&num_of_words, sizeof(unsigned short), 1, file); // write the number of words to the file
	char* word = malloc(sizeof(char));
	unsigned int i = 0, log_str = 0, phy_str = 1;
	WordTreeNode* root = tr.root;
	while (root->children[i]) // calling  the recursive function for all of the root children
	{
		getWordsFromRootChildAndWriteToFile(root->children[i], word, log_str, &phy_str, &buffer, &bits_left, file);
		i++;
	}
	free(word); //free the allocated string 
}
static void getWordsFromRootChildAndWriteToFile(WordTreeNode* node, char* word, unsigned int log_str, unsigned int* phy_str, unsigned char* buffer, int* bits_left, FILE* file)
{ // the function runs over the subtree and and send to compression the words from the tree and their frequency
	if (*phy_str == log_str) // need to reallocate the string 
		expandString(&word, phy_str);
	if (node->children[0] == NULL) // the node has no children  , its a leaf
	{
		word[log_str] = node->ch, word[log_str + 1] = '\0'; // add the letter of the node to the word // we need to add the word to the array, adding '\0'
		compressAndWriteWordToFile(word, node->frequency, file, buffer, bits_left); // send the current word and freq to compression and write to file
		return;
	}
	else // the node has children 
	{
		if (node->frequency == 0)// its not the end of the string, we need to add the letter to our string
		{
			word[log_str] = node->ch; // add the letter of the node to the word
			log_str += 1; // plus one the index of the string - there is more letters
		}
		else // freq is not zero , we need to add the word to the array 
		{
			word[log_str] = node->ch, word[log_str + 1] = '\0';
			compressAndWriteWordToFile(word, node->frequency, file, buffer, bits_left);  // send the current word and freq to compression and write to file
			log_str += 1;
		}
		for (int i = 0; node->children[i] != NULL; i++) // recursive calls for the children of the current node
			getWordsFromRootChildAndWriteToFile(node->children[i], word, log_str, phy_str, buffer, bits_left, file);
		log_str -= 1; // minus the index_arr for the turning back recursive to reduce the string word 
	}
}
void compressAndWriteWordToFile(char* word, unsigned short freq, FILE* file, unsigned char* buffer, int* bits_left)
{ // the function gets a file compress the WordData array and write it to the file
	unsigned char word_length;
	unsigned char freq_helper = 0x00;
	word_length = strlen(word);
	writeBitsToBuffer(file, buffer, word_length, bits_left, 5); // write the compressed word length
	for (int i = 0; i < (int)word_length; i++) //sending each letter to the compress function
		writeBitsToBuffer(file, buffer, COMPRESS_VALUE(word[i]), bits_left, 6); // write the compressed word letters
	freq_helper |= (freq >> 8); // adding the first byte of the freq(short) the the freq helper
	writeBitsToBuffer(file, buffer, freq_helper, bits_left, BITS_IN_BYTE);// write the compressed word frequency
	freq_helper = 0x00;
	freq_helper |= freq;  // adding the second byte of the freq(short) the the freq helper
	writeBitsToBuffer(file, buffer, freq_helper, bits_left, BITS_IN_BYTE); // write the compressed word frequency
	freq_helper = 0x00;
}
void writeBitsToBuffer(FILE* file, unsigned char* buffer, unsigned char letter_to_write, int* bits_left, int bits_to_write)
{  // the function compress the letter_to_write according to bits_to_write and adds its to the buffer,
	// if the all of the bits is been used in the byte, we write the buffer to the file 
	if (*bits_left - bits_to_write <= 0) // there is not enough available bits in the byte
	{
		*buffer |= (letter_to_write >> (bits_to_write - *bits_left)); // shift right the letter to gets just the number of bits left in the buffer
		fwrite(buffer, sizeof(unsigned char), 1, file); // buffer in now full,write it to the file
		*buffer = 0x00; // nullify the buffer 
		letter_to_write <<= (BITS_IN_BYTE - (bits_to_write - *bits_left)); // shift left the letter to get the left bits we didnt added to the buffer
		*buffer |= letter_to_write; // add the bits to the buffer
		*bits_left = BITS_IN_BYTE - (bits_to_write - *bits_left); // updating the number of bits left to add
	}
	else // there is enough bits in the byte for the compressed letter/number
	{
		letter_to_write <<= (*bits_left - bits_to_write); // shift left the letter to do BITWISE OR with the available bits in buffer
		*buffer |= letter_to_write; //  BITWISE OR the letter and the buffer
		*bits_left -= bits_to_write; // // updating the number of bits left to add
	}
}
void WriteCompressdFileFromTextFile(char* iname, char* oname)
{ //Q6, the function creates array of words and freq like Q4 and write them after compression to binary file
	FILE* file_to_write = fopen(oname, "wb");
	allocationCheck(file_to_write);
	DListNode** word_lists_arr = (DListNode**)malloc(sizeof(DListNode*));
	unsigned short num_of_words = 0;
	allocationCheck(word_lists_arr);
	word_lists_arr[0] = NULL;
	ReadFileToWordLists(iname, &word_lists_arr); // using the Q4 functions to create an DListNode array from the text file
	getNumOfWordsFromArr(word_lists_arr,&num_of_words); // get the number of words in the array
	fwrite(&num_of_words, sizeof(unsigned short), 1, file_to_write); // first write the number of words in the tree to the file
	CompressAndWriteArrToFile(word_lists_arr, file_to_write); // compress each word and freq of the array and write them to the binary file
	freeWordListsArr(word_lists_arr); // free the allocated strings from the array
	fclose(file_to_write);
}
void CompressAndWriteArrToFile(DListNode** word_lists_arr, FILE* file)
{ // the function gets word_lists_arr and compress each word and freq and write them to the binary file
	unsigned char buffer = 0x00, word_length, freq = 0x00;
	int bits_left = BITS_IN_BYTE;
	DListNode* curr;
	for (int i = 0; word_lists_arr[i]; i++) // runs over the array untill the NULL cell
	{
		curr = word_lists_arr[i];
		while (curr) // runs over the current list in the array
		{
			word_length = strlen(curr->str);
			writeBitsToBuffer(file, &buffer, word_length, &bits_left, 5); // write the compressed word length
			for (int j = 0; j < (int)word_length; j++) //sending each letter to the compress function
				writeBitsToBuffer(file, &buffer, COMPRESS_VALUE(curr->str[j]), &bits_left, 6); // write the compressed word letters
			freq |= ((curr->frequency) >> 8); // adding the first byte of the freq(short) the the freq helper
			writeBitsToBuffer(file, &buffer, freq, &bits_left, BITS_IN_BYTE);// write the compressed word frequency
			freq = 0x00;
			freq |= (curr->frequency);  // adding the  byte of the freq(short) the the freq helper
			writeBitsToBuffer(file, &buffer, freq, &bits_left, BITS_IN_BYTE); // write the compressed word frequency
			freq = 0x00;
			curr = curr->next;
		}
	}
}
char* getWordFromFile(FILE* file)
{ // the function reads letters from the file and returns a string
	char* word = (char*)malloc(sizeof(char));
	allocationCheck(word);
	int log = 0, phy = 1;
	char ch;
	fscanf(file, "%c", &ch);
	while (ch != ' ') // read the letters of the word until we get space button
	{
		if (log == phy)
			expandString(&word, &phy);
		word[log] = ch;
		log++;
		fscanf(file, "%c", &ch);
	}
	word = (char*)realloc(word, sizeof(char)*log + 1);
	allocationCheck(word);
	word[log] = '\0';
	return word;
}
static void expandString(char** word, int* phy)
{
	*phy *= 2;
	*word = (char*)realloc(*word, sizeof(char)*(*phy));
}
short getFreqFromFile(FILE* file)
{ // the function gets the frequency of the word from the file(can be negative)
	short freq = 0;
	char ch;
	fscanf(file, "%c", &ch);
	if (ch == '-') // the freq is negative
		getNegativeFreq(file, &freq);
	else
	{
		while (ch != ' ' && ch != '\n' && !feof(file)) // runs until we got to space or enter or end of file
		{
			freq = freq * 10 + CHAR_TO_INT(ch); //change the freq to a numer representation
			fscanf(file, "%c", &ch);
		}
	}
	return freq;
}
void getNegativeFreq(FILE* file, short* freq)
{ // the function reads  a negative frequency from the file
	char ch;
	fscanf(file, "%c", &ch); 
	while (ch != ' ' && ch!='\n' && !feof(file)) // runs until we got to space or enter or end of file
	{
		*freq = *freq * 10 + CHAR_TO_INT(ch); //change the freq to a numer representation
		fscanf(file, "%c", &ch);
	}
	*freq *= -1; // multiply  by -1 to get the negative number
}
