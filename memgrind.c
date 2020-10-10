#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int free_counter= 0;
    int malloc_counter =0; 
    int counter = 0;
    int big_count = 0;
    while(big_count<50){
        while (counter<120){
            //function pointer to malloc calling malloc 1 byte
            char (*_malloc)(void);
            //function pointer to free to release byte
            char (*_free)(void);
            counter++;
        }
        counter = 0;
        while (counter<120){
            //function pointer to malloc calling malloc 1 byte
            char (*_malloc)(void);
            counter++;
        }
        counter = 0;
        while (counter<120){
            //function pointer to free to release byte
            char (*_free)(void);
            counter++;
        }
        counter=0;
        while (counter<240){
            int random= rand() % 2;
            if(random = 0){ 
                //function pointer to malloc calling malloc 1 byte
                char (*_malloc)(void);
                malloc_counter += 1;
            }
            else{
                //function pointer to free to release byte
                char (*_free)(void);
                free_counter +=1  ;
            }
            if(malloc_counter<120){
                while (malloc_counter<120)
                {
                //function pointer to malloc calling malloc 1 byte
                char (*_malloc)(void);
                }
            }
            if(free_counter<120){
                while(free_counter<120){
                //function pointer to free to release byte
                char (*_free)(void);
                }    
            }

        }
    big_count++;
    }
    
   
}