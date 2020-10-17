#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
void* mymalloc(size_t size, char* filename, int line);
void myfree(void* p, char* filename, int line);