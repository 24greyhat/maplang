#include "../../include/stdlib/io.h"
#include <stdio.h>
#include <stdlib.h>


IoFile_t* readFile(char* path)
{
  IoFile_t* io = malloc(sizeof(IoFile_t));

  FILE* fp = fopen(path, "r");

  if(fp==NULL)
  {
    printf("\n[-] Failed to open file \"%s\"\n", path);
    exit(1);
  }


  io->data = malloc(sizeof(char));
  io->size = 1;

  int i = 0;

  char c;

  while((c = fgetc(fp)) && c != EOF) {
    io->data = realloc(io->data, io->size++);
    io->data[i++] = c;
  }

  io->data[i] = '\0';
  
  return io;
}


void writeFile(char* path, char* data);


void appendFile(char* path, char* data);


void writeStruct(char* path, void* src_obj);


void readStruct(char* path, void* dest_obj);


