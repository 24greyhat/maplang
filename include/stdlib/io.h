#ifndef __IO__
#define __IO__

#include <stddef.h>



struct {
  char* data;
  size_t size;
} typedef IoFile_t;


IoFile_t* readFile(char* path);


void writeFile(char* path, char* data);

void appendFile(char* path, char* data);


void writeStruct(char* path, void* src_obj);


void readStruct(char* path, void* dest_obj);



#endif
