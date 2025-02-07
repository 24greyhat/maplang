#include "../../include/interpreter/interpreter.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


Instruction_t* inst_i(int bytecode)
{
  Instruction_t* tmp = malloc(sizeof(Instruction_t*));
  tmp->bytecode = bytecode;
  return tmp;
}


Instruction_t* inst(int bytecode, MemObject_t* argument)
{
  Instruction_t* tmp = malloc(sizeof(Instruction_t*));
  tmp->bytecode = bytecode;
  tmp->argument = argument;
  return tmp;
}



MplBin_t* init_mplBin()
{
  MplBin_t* tmp = malloc(sizeof(MplBin_t));

  tmp->size = 1;
  tmp->instructions = malloc(sizeof(Instruction_t*));

  return tmp;
}


void bin_append(MplBin_t* bin, Instruction_t* instruction)
{
  bin->instructions[bin->size-1] = instruction;
  bin->size+=1;
  bin->instructions = realloc(bin->instructions, sizeof(Instruction_t*)*bin->size);
}


MplBin_t* loadMplBin(void* obj)
{
  return (MplBin_t*)obj;
}








// ----------- Interpreter -----------


Interpreter_t* init_interpreter(char* path)
{
  Lexer_t* lex = init_lexer(path);

  Parser_t* parser = init_parser(lex);

  Interpreter_t* i = malloc(sizeof(Interpreter_t));

  i->root = parse(parser);



  i->memory = init_memory();
  i->mem = init_memory();

  return i;
}


// implement this fully
void execute(Interpreter_t* vm, Instruction_t* inst, int functionMode)
{
  switch(inst->bytecode)
  {

    case push_i:
      mem_push(functionMode ? vm->mem : vm->memory, inst->argument);
      break;


    case getVar_i:
      if(functionMode)
      {
        for(int i = 0; i < vm->mem->size; i++)
        {
          switch(vm->mem->stack[i]->type)
          {
            case variable_type:
              break;
          }
        }
      }


    case add_i: // add the last 2 memory objects and push results = add()
    {
        puts("ADDING");
        MemObject_t* left = mem_pop(functionMode ? vm->mem : vm->memory);
        MemObject_t* right = mem_pop(functionMode ? vm->mem : vm->memory);

        puts("ADDING-");

        if(!left || !right)
        {
          // raiseStackUnderflow
          exit(1);
        }

        if(left->type!=right->type)
        {
          puts("NO\n");
          // raiseException
        }
        else{
        puts("ADDING---");
        puts("ADDING----");
        }

          switch(left->type)
          {
            case float_type:
            {
              puts("ADDING----FLOAT");
              Float_t* l = loadFloat(left->obj);
              Float_t* r = loadFloat(right->obj);
              Float_t* sum = init_float(l->value + r->value);

              if(functionMode)
                mem_push(vm->mem, initMemObj(sum, float_type, sizeof(Float_t)));

              else
                mem_push(vm->memory, initMemObj(sum, float_type, sizeof(Float_t)));
            };


            case int_type:
            {
              puts("ADDING----INT");
              Int_t* l = loadInt(left->obj);
              Int_t* r = loadInt(right->obj);
              Int_t* sum = init_int(l->value + r->value);

              printf("sum=%lld\n", sum->value);

              if(functionMode)
                mem_push(vm->mem, initMemObj(sum, int_type, sizeof(Int_t)));

              else
                mem_push(vm->memory, initMemObj(sum, int_type, sizeof(Int_t)));
            }
          }

     }
      break;


    default:
      puts("INST\n");
      break;
  }
}



void runProgram(Interpreter_t* vm, MplBin_t* bin)
{
  int i = 0;

  while(i+1 < bin->size)
  {
    execute(vm, bin->instructions[i++], 0);
  }
}









// --------- COMPILER --------------


size_t getMemObjSize(MemObject_t* obj)
{
  switch(obj->type)
  {
    case variable_type: return sizeof(Variable_t);

    case float_type: return sizeof(Float_t);

    case int_type: return sizeof(Int_t);

    case object_type: return sizeof(Object_t);

    case list_type: return sizeof(List_t);

    case function_type: return sizeof(Function_t);

    case string_type: return sizeof(String_t);

    case instruction_type: return sizeof(Instruction_t);

    default: return sizeof(NULL);

  }
}



MemObject_t* AstToMemObj(AstNode_t* node)
{
  switch(node->node_type)
  {
    case ast_var:
    {
        if(node->var_name)
        {
          Variable_t* var = init_variable(node->var_name);
          return initMemObj(var, variable_type, sizeof(Variable_t));
        }

        else if(node->var_def_name)
        {
          Variable_t* var = init_variable(node->var_def_name);

          MemObject_t* value = AstToMemObj(node->var_def_value);

          assign_variable(var, value->obj, value->type, getMemObjSize(value));
          
          return initMemObj(var, variable_type, sizeof(Variable_t));
        }
    }


    case ast_object:
    {
        int i = 0;

        MemObject_t* value = AstToMemObj(node->object_values[i]);

        Object_t* obj = init_object(node->object_keys[i++], value, value->type, getMemObjSize(value));

        while(i < node->object_size-1)
        {
          value = AstToMemObj(node->object_values[i++]);
          push(obj, node->object_keys[i], value, value->type, getMemObjSize(value));
        }

        return initMemObj(obj, object_type, sizeof(Object_t));
    }



    case ast_list:
    {
        int i = 0;

        MemObject_t* value = AstToMemObj(node->list_body[i]); // value at 0

        List_t* lst = init_list(value->obj, value->type, getMemObjSize(value));


        while(lst->length < node->list_size)
        {
          i+=1;
          value = AstToMemObj(node->list_body[i]);
          append(lst, value, value->type, getMemObjSize(value));
        }

        return initMemObj(lst, list_type, sizeof(List_t));
    }
    


    case ast_str:
    {
        String_t* value = init_str(node->str_value);

        return initMemObj(value, string_type, sizeof(String_t));
    }


    case ast_float:
    {
        Float_t* value = init_float(node->float_value);

        return initMemObj(value, float_type, sizeof(Float_t));
    }


    case ast_int:
    {
        Int_t* value = init_int(node->int_value);

        return initMemObj(value, int_type, sizeof(Int_t));
    }


    case ast_func_def:
    {
      int i = 0;

      Function_t* func = init_function(node->func_def_name);

      while(i < node->func_def_args_size)
      {
        MemObject_t* value = AstToMemObj(node->func_def_args[i]);
        i+=1;
        add_functionArg(func, loadVariable(value));
      }
        return initMemObj(func, function_type, sizeof(Function_t));
      }


    case ast_func_call:
    {
        int i = 0;

        MemObject_t* value = AstToMemObj(node->func_call_arguments[i++]);

        List_t* args = init_list(value, value->type, getMemObjSize(value));

        while(i < node->func_call_arg_size-1)
        {
          MemObject_t* value = AstToMemObj(node->func_call_arguments[i++]);
          append(args, value, value->type, getMemObjSize(value->obj));
        }

        FunctionCall_t* functionCall = init_func_call(node->func_call_name, args);

        return initMemObj(functionCall, function_call_type, sizeof(FunctionCall_t));
    }


    case ast_bin_exp:
    {
      MemObject_t* left = AstToMemObj(node->bexp_left);

      MemObject_t* right = AstToMemObj(node->bexp_right);

      int op = node->bexp_op;

      BinExp_t* bnxp = init_bin_exp(left->obj, left->type, right->obj, right->type, op);

      return initMemObj(bnxp, binary_expression_type, sizeof(BinExp_t));
    } 

    default:
      printf("%d\n", node->node_type);
      exit(1);
    // raise exception 

  }
}



MplBin_t* compile(AstNode_t* root)
{

  MplBin_t* bin = init_mplBin();

  AstNode_t* cn;

  int i = 0;

  while(i < root->node_size)
  {

    cn = root->node_value[i++]; // current node


    switch(cn->node_type)
    {
      case ast_var:
      {
          bin_append(bin, inst(push_i, AstToMemObj(cn)));
          break;
      }


      case ast_func_call:
        bin_append(bin, inst(callFunc_i, AstToMemObj(cn)));

        break;


      case ast_func_def:
        // use the AstToMemObj function to get the arg for the push instruction instead of having to repeat
        {
          Function_t* func = loadFunction(AstToMemObj(cn)->obj);

          MplBin_t* body = init_mplBin();

          int i = 0;

          while(i < cn->func_def_body_size-1)
          {
            MplBin_t* b = compile(cn->func_def_body[i++]);

            if(b->size==1)
              bin_append(body, b->instructions[b->size-1]);
          }


          func->body_binary = body;

          bin_append(bin, inst(push_i, initMemObj(func, function_type, sizeof(Function_t))));

          break;
        }


      
      case ast_bin_exp:
      {
          MemObject_t* left = AstToMemObj(cn->bexp_left);

          MemObject_t* right = AstToMemObj(cn->bexp_right);

          int op = cn->bexp_op;

          bin_append(bin, inst(push_i, left));
          bin_append(bin, inst(push_i, right));

          switch(op)
          {
            case plus_tok: bin_append(bin, inst_i(add_i));

            case div_tok: bin_append(bin, inst_i(div_i));

            case minus_tok: bin_append(bin, inst_i(sub_i));

            case mul_tok: bin_append(bin, inst_i(mul_i));

            case mod_tok: bin_append(bin, inst_i(modulo_i));
          }

          break;
      }


      default:
        puts("UNKNWONW");
        break;
    }

  }

  printf("size of bin: %zu\n", bin->size);

  return bin;
}

