#ifndef __MEM_TYPES__
#define __MEM_TYPES__

// ----------- Memory Types ---------------


#include "../parser/ast.h"
#include <stddef.h>



enum ObjectTypes {
  int_type,
  float_type,
  string_type,
  variable_type,
  list_type,
  object_type,
  function_type,
  function_call_type,
  binary_expression_type,
  instruction_type // Instruction_t struct type which is used by function bodies
};
                    

struct {
  long long int value;
} typedef Int_t;

Int_t* init_int(long long int value);
Int_t* loadInt(void* obj);


struct {
  long double value;
} typedef Float_t;

Float_t* init_float(long double value);
Float_t* loadFloat(void* obj);


struct {
  char* value;
} typedef String_t;

String_t* init_str(char* value);
String_t* loadStr(void* obj);


struct {
  char* name;
  void* value; // object
  int type; // object type
} typedef Variable_t;

Variable_t* init_variable(char* name);
void assign_variable(Variable_t* var, void* obj, int type, size_t size);
Variable_t* loadVariable(void* obj);



struct List_s {
  void* obj;
  int index;
  int type;
  int length;
  struct List_s* next;
} typedef List_t;

List_t* init_list(void* obj, int type, size_t size);

void append(List_t* list, void* obj, int type, size_t size); // push an item to the list
List_t* pop(List_t* list); // pop and return the head of the list
List_t* loadList(void* obj);


struct Object_s {
  char* key;
  void* obj; // value
  int type; // value type
  int index;
  int length;
  struct Object_s* next;
} typedef Object_t;


Object_t* init_object(char* key, void* obj, int type, size_t size);

void push(Object_t* object, char* key, void* obj, int type, size_t size); // push key value pair to this object
void removePair(Object_t* object, char* key);
void update(Object_t* object, char* key, void* obj, int type, size_t size);
Object_t* loadObject(void* obj);


struct {
  char* name;
  Variable_t** args; // arg names
  int args_size; // args size
  void* body_binary; // MplBin* pointer
} typedef Function_t;

Function_t* init_function(char* name);
void add_functionArg(Function_t* function, Variable_t* argument); // add an arg to the array of args
// args each arg is a variable with no type or value assigned
// when a function is called it's args are populated with values from the functionCall Instruction_t and pushed to the function memory stack and then the function body is executed within the context of the function memory stack
// the function memory stack is freed and reset after every function call outside of the functionMode.


Function_t* loadFunction(void* obj);



struct {
  char* name; // function name
  List_t* args; // function args
} typedef FunctionCall_t;

FunctionCall_t* init_func_call(char* name, List_t* args);

FunctionCall_t* loadFunctionCall(void* obj);


struct {
  void* left;
  int left_type;
  void* right;
  int right_type;
  int opcode; // div_i, plus_i, etc...
} typedef BinExp_t;

BinExp_t* init_bin_exp(void* left, int left_type, void* right, int right_type, int opcode);

BinExp_t* loadBinExp(void* obj);


struct {
  int type; // object type
  void* obj; // the struct pointer
} typedef MemObject_t;

MemObject_t* initMemObj(void* obj, int type, size_t size);



struct {
  MemObject_t** stack;
  size_t size; // stack size
} typedef Memory_t;


Memory_t* init_memory();

void mem_push(Memory_t* mem, MemObject_t* obj);

MemObject_t* mem_pop(Memory_t* mem);


void mem_free(Memory_t* mem);


#endif
