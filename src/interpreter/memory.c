#include "../../include/interpreter/memory.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Int_t* init_int(long long int value)
{
  Int_t* temp = malloc(sizeof(Int_t*));

  temp->value = value;

  return temp;
}

Int_t* loadInt(void* obj)
{
  return (Int_t*)obj;
}



Float_t* init_float(long double value)
{
  Float_t* temp = malloc(sizeof(Float_t*));

  temp->value = value;

  return temp;
}


Float_t* loadFloat(void* obj)
{
  return (Float_t*)obj;
}



String_t* init_str(char* value)
{
  String_t* temp = malloc(sizeof(String_t));

  temp->value = value;

  return temp;
}


String_t* loadStr(void* obj)
{
  return (String_t*)obj;
}




Variable_t* init_variable(char* name)
{
  Variable_t* temp = malloc(sizeof(Variable_t));
  temp->name = malloc(sizeof(char)*strlen(name));

  temp->name = name;

  return temp;
}


void assign_variable(Variable_t* var, void* obj, int type, size_t size)
{
  var->value = malloc(size);
  var->value = obj;
  var->type = type;
}


Variable_t* loadVariable(void* obj)
{
  return (Variable_t*)obj;
}



List_t* init_list(void* obj, int type, size_t size)
{
  List_t* temp = malloc(sizeof(List_t));
  temp->index  = 0;

  temp->type = type;

  temp->obj = malloc(size);

  temp->length = 1;

  return temp;
}

void append(List_t* list, void* obj, int type, size_t size)
{
  // get the head of the linked list
  int i = 1;

  list->length+=1;

  while(i < list->length-1)
  {
    list = list->next;
    i++;
  }

  list->next = init_list(obj, type, size);

}// push an item to the list

List_t* pop(List_t* list)
{
// get the head of the linked list
  int i = 1;
  list->length-=1;
  while(i < list->length+1)
  {
    list = list->next;
    i++;
  }

  return list;
}

List_t* loadList(void* obj)
{
  return (List_t*)obj;
}


Object_t* init_object(char* key, void* obj, int type, size_t size)
{
  Object_t* temp = malloc(sizeof(Object_t));

  temp->key = malloc(sizeof(char)*strlen(key));
  temp->key = key;

  temp->obj = malloc(size);
  temp->obj = obj;

  temp->type = type;
  temp->length = 1;
  temp->index = 0;

  return temp;
}


void push(Object_t* object, char* key, void* obj, int type, size_t size)
{
  Object_t* tmp = object->next;
  int i = 1;
  while(i < object->length)
  {
    i++;
    tmp = tmp->next;
  }

  tmp = init_object(key, obj, type, size);
}// push key value pair to this object


void removePair(Object_t* object, char* key)
{

}


void update(Object_t* object, char* key, void* obj, int type, size_t size)
{

}


Object_t* loadObject(void* obj)
{
  return (Object_t*)obj;
}




MemObject_t* initMemObj(void* obj, int type, size_t size)
{
  MemObject_t* temp = malloc(sizeof(MemObject_t));

  temp->type = type;

  temp->obj = malloc(size);

  temp->obj = obj;

  return temp;
}


Function_t* init_function(char* name)
{
  Function_t* temp = malloc(sizeof(Function_t));

  temp->name = malloc(sizeof(char)*strlen(name));

  temp->args_size = 1;
  temp->args = malloc(sizeof(Variable_t*));

  return temp;
}


void add_functionArg(Function_t* function, Variable_t* argument)
{
  function->args[function->args_size-1] = argument;
  function->args_size++;
  function->args = realloc(function->args, sizeof(Variable_t*)*function->args_size);
}// add an arg to the array of args


Function_t* loadFunction(void* obj)
{
  return (Function_t*)obj;
}


FunctionCall_t* init_func_call(char* name, List_t* args)
{
  FunctionCall_t* call = malloc(sizeof(FunctionCall_t));

  call->name = malloc(sizeof(char)*strlen(name));

  call->args = malloc(sizeof(List_t));
  call->args = args;

  return call;
}


FunctionCall_t* loadFunctionCall(void* obj)
{
  return (FunctionCall_t*)obj;
}


BinExp_t* init_bin_exp(void* left, int left_type, void* right, int right_type, int opcode)
{
  BinExp_t* obj = malloc(sizeof(BinExp_t));

  obj->left = left;
  obj->left_type = left_type;


  obj->right = right;
  obj->right_type = right_type;

  obj->opcode = opcode;

  return obj;
}


BinExp_t* loadBinExp(void* obj);



Memory_t* init_memory()
{
  Memory_t* temp = malloc(sizeof(Memory_t));

  temp->size = 1;
  temp->stack = malloc(sizeof(MemObject_t*));

  return temp;
}


void mem_push(Memory_t* mem, MemObject_t* obj)
{
  printf("%zu, type: %d\n", mem->size, obj->type);
  mem->stack[mem->size-1] = obj;

  mem->size+=1;
  mem->stack = realloc(mem->stack, sizeof(MemObject_t*)*mem->size);
}


MemObject_t* mem_pop(Memory_t* mem)
{
  if(mem->size>0)
    return mem->stack[--mem->size];

  return (MemObject_t*)NULL;
}


void mem_free(Memory_t* mem)
{
  for(int i = 0; i < mem->size; i++)
  {
    free(mem->stack[mem->size--]);
  }

  free(mem);
}



