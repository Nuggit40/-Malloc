#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(){
   srand(time(0));
    int big_count = 0;
    while(big_count<50){
        int free_counter= 0;
        int malloc_counter =0; 
        int counter = 0;
        int counter1 = 0;
        int top =0; 
        int i=0;
        int mallocCount=0;
        int freeCount = 0;
        while (counter<120){
            //function pointer to malloc calling malloc 1 byte
            char* r = (char*)malloc(1);
            //function pointer to free to release byte
            free(r);
            counter+=1;
        }
        counter = 0;
        char* arr[120];
        while (counter<120){
            //function pointer to malloc calling malloc 1 byte
            arr[counter] = (char*)malloc(1);
            counter+=1;
        }
        counter = 0;
        while (counter<120){
            //function pointer to free to release byte
            free(arr[counter]);
            counter+=1;
        }
        counter=0;
        
        char* pointers[120];
    // int top, i, mallocCount, freeCount = 0;
    for(i; i < 240; ++i){
        int r = rand() % 2;
        if(mallocCount < 120 && r == 0){
            //space to malloc again
            pointers[top] = (char*)malloc(1);
            ++top;
            ++mallocCount;
            printf("malloced byte %d \n", mallocCount);
             printf("total opperations %d \n", mallocCount+freeCount);
        } else if(freeCount < 120 && r == 1 && top != 0){
            //pointers to free again
            free(pointers[top-1]);
            --top;
            ++freeCount;
            printf("freed byte %d \n", freeCount);
        } else if(r == 0){
            //no more mallocs needed, do a free
            free(pointers[top-1]);
            --top;
            ++freeCount;
            printf("freed byte %d \n", freeCount);
        } else if(r == 1){
            //no more pointers to free, do a malloc
            pointers[top] = (char*)malloc(1);
            ++top;
            ++mallocCount;
            printf("malloced byte %d \n", mallocCount);
        }
    }
    
    //     char* arr1[120];
    // while (counter1<240 && counter1 > -1){
            
    //         int random= rand() % 2;
    //         if(random == 0){ 
    //             //function pointer to malloc calling malloc 1 byte
    //             arr1[counter1] = (char*)malloc(1);
    //             malloc_counter += 1;
    //             counter1 +=1;
    //             //printf("malloced byte %d \n", counter1);
    //         }
    //         else{
    //             //function pointer to free to release byte
    //             free(arr1[counter1]);
    //             free_counter +=1  ;
    //             counter1 +=1;
    //             //printf("freed byte %d \n", counter1);
    //         }
            

    //     }
    //     if(malloc_counter<120 && malloc_counter>-1){
    //             while (malloc_counter<120 && malloc_counter>-1)
    //             {
    //             //function pointer to malloc calling malloc 1 byte
    //             arr1[counter1] = (char*)malloc(1);
    //             malloc_counter+=1;
    //            // printf("malloced byte %d to 120\n", counter1);
    //             }
    //         }
    //         if(free_counter<120 && free_counter>-1){
    //             while(free_counter<120 && free_counter>-1){
    //             //function pointer to free to release byte
    //             free(arr1[counter1]);
    //             free_counter+=1;
    //             //printf("freed byte %d to 120\n", counter1);
    //             }    
    //         }
    printf("thats a big count %d\n", big_count);
    big_count+=1;
    }
  

  return 0; 
}
