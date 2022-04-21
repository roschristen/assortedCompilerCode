// /* See chapter 7 -
//   Resolve and type checking code */
#include <stdio.h>
#include <stdlib.h>
#include "ast_library.h"
#include "type_check.h"
#include "symbol.h"
#include "scope.h"
#include "ast_printer.h"

//Incomplete code from the typechecking step of our compiler. It recursively traverses the AST performing name resolution, and checks for type errors 
//and computes the type of expressions. It is based on code and guidance from Introduction to Compilers and Language Design by Professor Douglas Thain. 
//It is incomplete to prevent current students of cs230 from using it for solutions. It originally contained resolve and typecheck functions for declarations, 
//statements, expressions, and param_lists, that would each call each other recursively.

int _counter = 0;
struct type *lastFunct = 0;

int typecheck_error_count(){
  return _counter;
}
//Declaration resolve function. The original code also contained an expr_resolve, stmt_resolve and param_list_resolve
void decl_resolve(struct decl *d){
    if(!d) return;
    //check if d has already been declared in current scope
    if(scope_lookup_current(d->name)){
      _counter+=1;
      printf("ERROR: variable %s declared twice in same scope\n", d->name);
    }
    else{
      //set the symbol kind depending on scope level
      symbol_t kind = scope_level() >1?
          SYMBOL_LOCAL:SYMBOL_GLOBAL;
    //store the symbol kind in d->symbol
    d->symbol = symbol_create(kind, d->type, d->name);
    //resolve the value of the declaration
    expr_resolve(d->value);
    //bind the value to the symbol table
    printf("binding %s to symbol table\n", d->name);
    scope_bind(d->name, d->symbol);
  }
    //if d is a function, run the appropriate resolves
    if(d->code){
      scope_enter();
      param_list_resolve(d->type->params);
      stmt_resolve(d->code);
      scope_exit();
    }
    //resolve the next declaration
    decl_resolve(d->next);
}

struct type * expr_typecheck(struct expr *e){
  if(!e) return 0;
//  printf("EXPR: running expr typecheck on expression of type %s\n", getExprName(e->kind));
  //call expr_typecheck on left and right sides
  struct type *lt = expr_typecheck(e->left);
  struct type *rt = expr_typecheck(e->right);
  //initialize return val and symbol struct for error checking
  struct symbol *s;
  struct type * result=0;
  switch(e->kind){
    case EXPR_NAME:
  //check if the variable has been declared
    if (e->symbol) {
      result = type_copy(e->symbol->type);
      }
    else {
      //otherwise print an error
      _counter +=1;
          printf("ERROR: Undefined variable %s\n", e->name);
          result = type_create(TYPE_VOID, 0, 0);
        }
        break;

    case EXPR_INTEGER_LITERAL:
    //create integer type and exit
        result = type_create(TYPE_INTEGER, 0 ,0);
        break;

    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_SUB:
    case EXPR_ADD:
    case EXPR_MODULUS:
    case EXPR_EXPONENTIATION:
    //if either the left side or the right side are functions, set their types to be their return types
    if(lt->kind == TYPE_FUNCTION){
      lt = lt->subtype;
    }
    if(rt->kind ==TYPE_FUNCTION){
      rt = rt->subtype;
    }
    //check if either of the values in an arithmetic expression are not integers
        if(lt->kind!=TYPE_INTEGER ||rt->kind!=TYPE_INTEGER){
          _counter +=1;
          printf("ERROR:cannot perform an arithmetic operation %s on values of type %s and %s\n", getExprName(e->kind),getTypeName(lt->kind),getTypeName(rt->kind));
        }
        //create a valid type(integer) and exit
        result = type_create(TYPE_INTEGER,0,0);
        break;
    case EXPR_INCREMENT:
    case EXPR_DECREMENT:
        //check if the variable being incremented/decremented is an integer
            if(lt->kind!=TYPE_INTEGER){
              _counter +=1;
              printf("ERROR:cannot perform an increment or decrement on non-integer value of type %s\n", getTypeName(lt->kind));
            }
            //create a valid type(integer) and exit
            result = type_create(TYPE_INTEGER,0,0);
            break;
            
    case EXPR_GREATER_THAN:
    case EXPR_LESS_THAN:
    case EXPR_GREATER_EQUAL:
    case EXPR_LESS_EQUAL:
    //if the left or right sides are functions, set their values to be their return types
    if(lt->kind ==TYPE_FUNCTION){
      lt=lt->subtype;
    }
    if(rt->kind == TYPE_FUNCTION){
      rt = rt->subtype;
    }
    //check if the operators are being used on non-integer values
        if(!lt->kind == TYPE_INTEGER || !rt->kind ==TYPE_INTEGER ){
          _counter +=1;
          printf("ERROR: comparison operators cannot compare non-integer values\n");
        }
        //create a valid type and exit
        result = type_create(TYPE_BOOLEAN, 0, 0);
        break;
      default:
      //debugging check
           printf("COMPILER BUG: Unhandled expression type %s\n", getExprName(e->kind));

            result = type_copy(lt);
            break;

  }
  //delete left and right sides
  type_delete(lt);
  type_delete(rt);
  //debugging check
  if (!result) {
  printf("COMPILER BUG: expression typecheck result is null\n");
}
else{
  //debugging print
  // printf("    expr_typecheck return result %s for expr type %s\n", getTypeName(result->kind), getExprName(e->kind));
}
  return result;
}
