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

//gets the data size stored in a metablock
unsigned short getDataSize(metaBlock* block){
    return *block & 0b0111111111111111;
}

//gets a pointer to the data belonging to a metablock
void* getDataPointer(metaBlock* block){
    return (void*)((char*)block + sizeof(metaBlock));
}

//sets the data size of a metablock
void setDataSize(metaBlock* block, unsigned short size){
    *block = size | isFree(block);
}

//determines if a meta block is free or not indicated by the highest bit
//if the metablock is free: a positive unsigned short is returned, otherwise: 0
unsigned short isFree(metaBlock* block){
    return *block & 0b1000000000000000;
}

//sets a metablock to be free
void setFreeBit(metaBlock* block){
    *block |= 0b1000000000000000;
}

//sets a metablock to be not free
void eraseFreeBit(metaBlock* block){
    *block &= 0b0111111111111111;
}

//gets the address of the next metablock in memory
metaBlock* getNextMetaBlock(metaBlock* block){
    unsigned short size = getDataSize(block);
    return (metaBlock*)((char*)getDataPointer(block) + size);
}

//checks if an address lies within the bounds of memory
short isValidAddress(metaBlock* block){
    return ((char*)block >= (char*)myblock && (char*)block < (myblock + 4096));
};

//initializes the first metablock of memory
void initMalloc(){
    metaBlock* firstBlock = (metaBlock*)myblock;
    setFreeBit(firstBlock);
    setDataSize(firstBlock, MEMORY_SIZE - sizeof(metaBlock));
    ++_firstMalloc;
}

//prints out information about each metablock in memory (debugging)
void printBlockList(){
    metaBlock* m = (metaBlock*)myblock;
    while(isValidAddress(m)){
        printf("\t%d\t", m);
        printf("%hu\t", getDataSize(m));
        printf("%d\n", isFree(m) ? 1 : 0);
        m = getNextMetaBlock(m);
    }
}


//splits one metablock in two so that a metablock only contains the requested allocation size
void split(metaBlock* initialBlock, unsigned short size){
    metaBlock* newBlock = (metaBlock*)((char*)getDataPointer(initialBlock) + size);

    setDataSize(newBlock, getDataSize(initialBlock) - sizeof(metaBlock) - size);
    setFreeBit(newBlock);

    setDataSize(initialBlock, size);
    eraseFreeBit(initialBlock);
}

//allocates allocationSize bytes in memory and returns a pointer to the beginning of those bytes
//if parameter allocationSize is < 1: "provide positive size parameter for malloc()" is printed
// and NULL is returned
//if there is not enough memory to allocate allocationSize: "not enough memory to malloc() requested size"
// is printed and NULL is returned
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
    if(getDataSize(current) == allocationSize){
        //found a block of perfect size, no need to split any blocks
        eraseFreeBit(current);
        return getDataPointer(current);
    } else if(getDataSize(current) > allocationSize){
        //block found is larger than allocationSize so it gets split in two
        split(current, allocationSize);
        return getDataPointer(current);
    } else{
        printf("Error: not enough memory to malloc() requested size\tFile: %s, Line: %d\n", filename, line);
        return NULL;
    }
}

//combines adjacent free metablocks into a singular free metablock
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

//free bytes allocated in memory so that they can be reallocated
//if parameter p is not a valid memory address: "invalid address to free" is printed and the function returns
//if parameter p has already been freed: "variable has already been freed" is printed and the function returns
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

// int main(){
//     int i = 0;
//     while(i++ < 120){
//         char* r = (char*)malloc(1);
//         free(r);
//     }
//     i = 0;
//     char* arr[120];
//     while(i++ < 120){
//         arr[i] = (char*)malloc(1);
//     }
//     i = 0;
//     while(i++ < 120){
//         free(arr[i]);
//     }
//     printBlockList();
// }   