#include <stdio.h>
#include "mymalloc.h"

#define MEMORY_SIZE 4096
typedef unsigned short metaBlock;
static char myblock[4096];
short _firstMalloc = 0;

metaBlock getDataSize(metaBlock* block);
void* getDataPointer(metaBlock* block);
void setDataSize(metaBlock* block, unsigned short size);
unsigned short isFree(metaBlock* block);
void setFreeBit(metaBlock* block);
void eraseFreeBit(metaBlock* block);
metaBlock* getNextMetaBlock(metaBlock* block);
short isValidAddress(metaBlock* block);
void initMalloc();
void printBlockList();
void split(metaBlock* initialBlock, unsigned short size);
void* mymalloc(size_t allocationSize, char* filename, int line);
void myfree(void* p, char* filename, int line);
void merge(metaBlock* justFreed);

//mask out the free bit
unsigned short getDataSize(metaBlock* block){
    return *block & 0b0111111111111111;
}

void* getDataPointer(metaBlock* block){
    return (void*)((char*)block + sizeof(metaBlock));
}

void setDataSize(metaBlock* block, unsigned short size){
    *block = size | isFree(block);
}

//is the free bit set or not
unsigned short isFree(metaBlock* block){
    return *block & 0b1000000000000000;
}

void setFreeBit(metaBlock* block){
    *block |= 0b1000000000000000;
}

void eraseFreeBit(metaBlock* block){
    *block &= 0b0111111111111111;
}

metaBlock* getNextMetaBlock(metaBlock* block){
    unsigned short size = getDataSize(block);
    return (metaBlock*)((char*)getDataPointer(block) + size);
}

short isValidAddress(metaBlock* block){
    return ((char*)block >= (char*)myblock && (char*)block < (myblock + 4096));
};

void initMalloc(){
    metaBlock* firstBlock = (metaBlock*)myblock;
    setFreeBit(firstBlock);
    setDataSize(firstBlock, MEMORY_SIZE - sizeof(metaBlock));
    ++_firstMalloc;
}

void printBlockList(){
    metaBlock* m = (metaBlock*)myblock;
    while(isValidAddress(m)){
        printf("\t%x\t", m);
        printf("%hu\t", getDataSize(m));
        printf("%d\n", isFree(m) ? 1 : 0);
        m = getNextMetaBlock(m);
    }
}

void split(metaBlock* initialBlock, unsigned short size){
    metaBlock* newBlock = (metaBlock*)((char*)getDataPointer(initialBlock) + size);

    setDataSize(newBlock, getDataSize(initialBlock) - sizeof(metaBlock) - size);
    setFreeBit(newBlock);

    setDataSize(initialBlock, size);
    eraseFreeBit(initialBlock);
}

void* mymalloc(size_t allocationSize, char* filename, int line){
    if(_firstMalloc == 0){
        initMalloc();
    }
    if(allocationSize <= 0){
        printf("Error: provide positive size parameter for malloc()\tFile: %s, Line: %d\n", filename, line);
        return NULL;
    }
    metaBlock* current = (metaBlock*)myblock;
    //traverse the linked list of meta blocks until you find one that can hold allocationSize
    while((getDataSize(current) < allocationSize || isFree(current) == 0) && isValidAddress(getNextMetaBlock(current))){
        current = getNextMetaBlock(current);
    }
    //perfect size, no need to split any blocks
    if(getDataSize(current) == allocationSize){
        eraseFreeBit(current);
        //printf("Perfect size block allocated\n");
        return getDataPointer(current);
    } else if(getDataSize(current) > allocationSize){
        //block is bigger than what we need so we split it in two
        split(current, allocationSize);
        return getDataPointer(current);
    } else{
        printf("Error: not enough memory to malloc() requested size\tFile: %s, Line: %d\n", filename, line);
        return NULL;
    }
}

void merge(metaBlock* justFreed){
    metaBlock* current = (metaBlock*)myblock;
    metaBlock* next;
    metaBlock* previous = NULL;
    //find the block that was just freed
    while(current != justFreed){
        previous = current;
        current = getNextMetaBlock(current);
    }
    next = getNextMetaBlock(current);
    //inspect the metablock to the right and merge the two if possible
    if(isValidAddress(next) && isFree(next)){
        setDataSize(current, getDataSize(current) + sizeof(metaBlock) + getDataSize(next));
    }
    //inspect the metablock to the left and merge the two if possible
    if(isValidAddress(previous) && isFree(previous)){
        setDataSize(previous, getDataSize(previous) + sizeof(metaBlock) + getDataSize(current));
    }
}

void myfree(void* p, char* filename, int line){
    metaBlock* toBeFreed = (metaBlock*)((char*)p - sizeof(metaBlock));
    //check if toBeFreed is a valid meta block
    metaBlock* current = (metaBlock*)myblock;
    while(current != toBeFreed){
        if(!isValidAddress(current)){
            //reached the end of memory without finding toBeFreed, invalid address
            printf("Error invalid address to free\tFile:%s, Line:%d\n", filename, line);
            return;
        }
        current = getNextMetaBlock(current);
    }
    if(!isFree(toBeFreed)){
        setFreeBit(toBeFreed);
        //merge any adjacent free blocks
        merge(toBeFreed);
    } else {
        printf("Error variable has already been freed\tFile:%s, Line:%d\n", filename, line);
    }
}

int main(){
    char* r = (char*)malloc(2048);
    char* q = (char*)malloc(2044);
    printBlockList();
    free(r);
    free(q);
    printf("\nAfter Free\n");
    printBlockList();
}   