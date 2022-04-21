/* See chapter 7
  Scope management code */
  #include "scope.h"
  #include "symbol.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include "hash_table.h"
  //top of stack declaration
//This is incomplete code from scope.c, the scope management code for our compiler in cs230. It is based on code and guidance from 
//Introduction to Compilers and Language Design by Professor Douglas Thain. 
//It is intentionally incomplete to avoid giving solutions to current cs230 students.
  struct stackNode *root=0;
  void scope_enter(){
    //create a new stacknode
    struct stackNode *top = malloc(sizeof(top));
    //put a new hashtable in the new stacknode(with one bucket and no function???)
    struct hash_table *new = hash_table_create(1,0);
    top->hashTable = new;
    //set the next value to point to the previous top of the stack(which is null if there were no stacknodes before)
    top->next = root;
    //set the top value(root value I guess) to the new top of the stack
    root = top;
  }
  void scope_exit(){
    //store the top stackNode in a temp variable
    struct stackNode *temp = root;
    //set top to be previous stackNode
    root = root->next;
    //delete the hashmap stored at the old top stacknode
    hash_table_delete(temp->hashTable);
    //free the stacknode memory
    free(temp);
    //set the pointer temp to 0(no dangling pointers)
    temp = 0;
  }

  struct symbol *scope_lookup(const char *name){
    //store root in temp stackNode
   struct stackNode *temp = root;
   struct symbol *p; //return val

//while temp is not null, check its hashtable for the name
   while (temp) {
     p = hash_table_lookup(temp->hashTable, name);
     if (p) {
       //if found, return the symbol struct
       return p;
     }
     //otherwise get next stacknode
     temp = temp->next;
   }
   //return null if nothing was found
   return 0;
  }
