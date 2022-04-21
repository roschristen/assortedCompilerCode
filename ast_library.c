#include "ast_library.h"
#include <stdio.h>
#include <stdlib.h>
//This is incomplete code from ast_library.c, the part of our compiler that held the declarations for the different type creation functions to build the ast. 
//It is based on code and guidance from Introduction to Compilers and Language Design by Professor Douglas Thain. It is intentionally incomplete so that the
//solutions are not available to current students in cs230.

/* Create a declaration */
struct decl * decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next) {
  // Note the sizeof(*d) translates to sizeof(struct decl)  -- what d points to
  struct decl *d = malloc(sizeof(*d));
  if (!d) {
    yyerror("out of space: declaration");
    exit(0);
  }
  
//set all of the fields of the declaration struct to the corresponding parameter values
  d->name = name;
  d->type = type;
  d->value = value;
  d->code = code;
  d->next = next;
  return d;
}

/* Create a statement */
struct stmt *stmt_create(stmt_t kind, struct decl *decl,struct expr *init_expr,struct expr *expr, struct expr *next_expr, struct stmt *body,struct stmt *else_body, struct stmt *next){
  struct stmt *t = malloc(sizeof(*t));
  if (!t) {
    yyerror("out of space: type");
    exit(0);
  }
  //set all of the fields of the statement struct to the corresponding parameter values
  t->kind = kind;
  t->decl = decl;
  t->init_expr = init_expr;
  t->expr = expr;
  t->next_expr = next_expr;
  t->body = body;
  t->else_body = else_body;
  t->next = next;
  return t;
}

/* Function to create an integer literal */
struct expr *int_lit_create(int val) {
  struct expr *t = malloc(sizeof(*t));
  if (!t) {
    yyerror("out of space: type");
    exit(0);
  }
    //set all of the fields of the expression struct to the corresponding parameter values
    t->intVal = val;
    t->kind = EXPR_INTEGER_LITERAL;
  return t;
}
/* Function to create a Boolean literal */
struct expr *bool_lit_create(int val) {
  struct expr *t = malloc(sizeof(*t));
  if (!t) {
    yyerror("out of space: type");
    exit(0);
  }
    //set all of the fields of the expression struct to the corresponding parameter values
      t->intVal = val;
      t->kind = EXPR_BOOLEAN_LITERAL;
  return t;
}

//set all of the fields of the expression struct to the corresponding parameter values
   t->intVal = *(val+1);//put the character in intVal(val points to the first quote mark, so it needs to be val+1)
   t->kind = EXPR_CHARACTER_LITERAL;
  return t;
}

/* Function to create a string literal */
struct expr *str_lit_create(char* val) {
  struct expr *t = malloc(sizeof(*t));
  if (!t) {
    yyerror("out of space: type");
    exit(0);
  }
    //set all of the fields of the expression struct to the corresponding parameter values
    t->stringVal = val;
    t->kind = EXPR_STRING_LITERAL;
  return t;
}
