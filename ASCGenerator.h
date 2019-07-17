/**
 * Annotated Source Code Generator Header
 *
 * Interface for the annotated source code generator
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "ASCGList.h"
#include "symbolTable.h"
#include <stdbool.h>

ASCGList* list;
ASCGList* getMainList();

void asc_init();
/**
 * Stores the scope information for a given line.
 *
 * @param scope_information - scope information for the given line
 * @param line_number - the relevant line number for the scope
 *
 * @return void
 */
void asc_store_scope_information(char *scope_information, size_t line_number,ASCGList *list);

/**
 * Stores an error message for a given line
 *
 * @param error_message - an error message for a given line
 * @param line_number - the given line
 *
 * @return void
 */
void asc_store_error_message(char *error_message, size_t line_number,ASCGList *list);


/**
 * Stores a line of source code
 *
 *This is method only be called by void asc_print(char *source_code_path) method

 * @param source_code - a line of source_code from source file.
 * @param line_number - the given line
 *
 * @return void
 */
void asc_store_source_code(char *source_code, size_t line_number,ASCGList *list);


/**
The purpose is to reduce same code;
*/
void prepare_node_and_add(char *message, size_t line_number, int attr,ASCGList *list);

/**
 * Prints the annotated source code for the given program source
 *
 * Format of the print should be the following:
 *
 * 1. Print the scope information
 * 2. Print the corresponding source code for the line.
 * The first character of the line should appear at the
 * same place as all other lines of source code. Multi-
 * line comments do not have to start their first character
 * of the line at the same place as all other lines.
 * 3. Print the corresponding error message for the line,
 * if one exists.
 *
 * For more information on the format see the project handout.
 *
 * @param source_code_path - path to the source code
 * @param i - condition to destination filename
 * @return void
 *
 */
void asc_print(char *source_code_path,char* output_path,int load_attr,ASCGList *list);


/*
* Print only error messages
*/
void asc_print_errors(ASCGList* list);
/**
  This will save all nodes in the list in the the file with print_file_path
*/
void print_list_to_file(char *print_file_path,int prefCondition,ASCGList *list);
/**
  This will read all source code, seperate them into lines, and load into list
*/
void load_source_code_to_list(char *source_code_path,ASCGList *list);
/**
  This is intended to read all characters in the source code file and store it into a char**
  The output should be capable of adding into the list line by line

  * @param source_code_path - path to the source code
  *
  * @return void
*/
char* read_all_char(char *source_code_path);
