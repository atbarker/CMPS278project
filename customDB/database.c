//contains the main database functionality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//hash table implementation

hash_t *tables;

int init_db(char *newDB){
    tables = hash_new();

    return 0;
}

int create_table(char *new){

    return 0;
}

int drop_table(Table *table){
    if(!table){
        printf("No such table\n");
        return -1;
    }
    return 0;
}

int add_column(Table *table, char *new_column){
    return 0;
}

int drop_column(Table *table, Column *column){
    if(!column){
        return -1;
    }
    return 0;
}

int drop_row(Table *table, Row *row){
   return 0;
}

Column *find_column(){

}

int insert(){
    return 0;
}

int select(){
    return 0;
}

int list_columns(Table, *table){
   return 0;
}

int list_tables(){
   return 0;
}

int where(char *table_name, char *column_name, char *value){
   return 0;
}
