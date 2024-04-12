#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 23

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order;
	struct RecordType* next;
};

// Fill out this structure
struct HashType
{
	struct RecordType* records[HASH_SIZE];
};

// Compute the hash function
int hash(int x)
{
	return (x % HASH_SIZE);
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}
		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

    for (i = 0; i < hashSz; ++i)
    {
        struct RecordType* currentNode = pHashArray->records[i];
		if(currentNode != NULL){
			printf("Index %d -> ", i);
			while (currentNode != NULL)
			{
				printf("%d, %c, %d -> ", currentNode->id, currentNode->name, currentNode->order);
				currentNode = currentNode->next;
			}
		printf("NULL\n");
		}
    }
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	// Your hash implementation
    struct HashType hashTable;

    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable.records[i] = NULL;
    }

    // Insert records into the hash table
	for (int i = 0; i < recordSz; i++) {
		int index = hash(pRecords[i].id);
		struct RecordType* newNode = (struct RecordType*)malloc(sizeof(struct RecordType));
		if (!newNode){
			return -1;
		}

		//Copy info from node
		*newNode = pRecords[i];
		// Point to the current head at the array
		newNode->next = hashTable.records[index];
		// Make newNode the new head
		hashTable.records[index] = newNode;
	}

    // Display records in the hash structure
    displayRecordsInHash(&hashTable, HASH_SIZE);

    // Free memory allocated for the hash table
    for(int i = 0; i < HASH_SIZE; ++i)
    {
        struct RecordType* current = hashTable.records[i];
        while (current != NULL)
        {
            struct RecordType* temp = current;
            current = current->next;
            free(temp);
        }
    }

    // Free memory allocated for records array
    free(pRecords);

	return 0;
}