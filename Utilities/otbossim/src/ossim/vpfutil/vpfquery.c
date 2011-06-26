/*************************************************************************
 *
 *N  Module VPFQUERY
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for querying a VPF table with a
 *     selection expression.  It has one main entry point - query_table().
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *    set_type query_table( char *expression, vpf_table_type table );
 *E
*************************************************************************/

#include <string.h>
#include <stdlib.h>
#if defined(__MSDOS__)
#include <alloc.h>
#include <mem.h>
#include <conio.h>
#else
#ifdef __APPLE__ 
#include <sys/types.h>
#include <sys/malloc.h>
#else
#ifndef __FreeBSD__
#include <malloc.h>
#include <string.h>
#endif
#endif
#endif


#ifndef __MSDOS__
#ifndef _WIN32
#include <unistd.h>
#endif
#endif
#include <stdio.h>
#include <ossim/vpfutil/linklist.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpftidx.h>
#include <ossim/vpfutil/set.h>


/* Delimiter tokens */
typedef enum { EQ, NE, LE, GE, LT, GT, AND, OR, QUOTE } delim_type;
/*static delim_type delim;*/

/* Valid delimeter strings */
char *delimstr[] = { "=", "<>", "<=", ">=", "<", ">", " AND ",
		     " OR ", "\"" };
int ndelim = 9;

char **fieldname;
int *fieldcol;
int nfields;

/* Token types */
#define DELIMETER  1
#define FIELD      2
#define VALUE      3
#define QUOTE      4
#define STRING     5
#define EOL        6
#define FINISHED   7
#define LOP        8   /* Logical Operator */
#define JOIN       9
#define ERROR     10

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif


typedef struct {
   int field;
   char op;
   char value[255];
   char join;
} expr_type;


static void *memalloc( ossim_uint32 size )
{
   void *ptr;

   ptr = malloc(size);
   if (!ptr) {
#ifdef __MSDOS__
      printf("Memory allocation error in VPFQUERY  (%d)(%d)\n",size,
	     farcoreleft());
#else
      printf("Memory allocation error in VPFQUERY  (%d)\n",(int)size);
#endif
      exit(1);
   }
   return ptr;
}

int is_white( char c )
{
   if (c==' ' || c=='\t') return 1;
   return 0;
}


/*************************************************************************
 *
 *N  return_token
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the first token string found in the
 *     expression string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expr   <input>==(char *) selection expression string.
 *    token <output>==(char *) first token in the string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
*************************************************************************/
static void return_token( char *expr, char *token )
{
   register int i,j,n,found=0,stopflag;

   n = 0;
   stopflag=0;
   while (expr[0] == ' ') {
      for (i=0;i<ndelim;i++)
	 if (ossim_strncasecmp(expr,delimstr[i],(unsigned int)strlen(delimstr[i])) == 0) {
	    stopflag=1;
	    break;
	 }
      if (stopflag) break;
      expr++;
   }
   strcpy(token,expr);
   for (i=0;(unsigned int)i<strlen(token);i++) {
      for (j=0;j<ndelim;j++) {
	 if (ossim_strncasecmp(expr,delimstr[j],(unsigned int)strlen(delimstr[j]))==0) {
	    if (n>0)
	       token[i] = '\0';
	    else
	       token[strlen(delimstr[j])] = '\0';
	    found = 1;
	    break;
	 }
      }
      if ((found) || (!is_white(*expr))) n++;
      if ((!found)&&(*expr)) expr++;
/*      if (!is_white(*expr)) n++;  */
/*      if ((found) || (!is_white(*expr))) n++; */
      if (found) break;
   }
}




/*************************************************************************
 *
 *N  get_token
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function gets the first token, token type, and token value of
 *     the expression string, and then advances the expression string
 *     past the token.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression   <input>==(char *) selection expression string.
 *    token       <output>==(char *) first token in the string.
 *    token_type  <output>==(int *) token type.
 *    token_value <output>==(int *) token_value.
 *    return      <output>==(char *) new selection expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    static void return_token( char *expr, char *token ) VPFQUERY.C
 *E
 *************************************************************************/
static char *get_token( char *expression,
		 char *token, int *token_type, int *token_value )
{
   register int i, stopflag;

   *token_type = 0;

   if (*expression == '\0') {
      *token_type = FINISHED;
      *token_value = 0;
      return expression;
   }

   if (*expression=='\r') {  /* crlf */
      ++expression; ++expression;
      *token = '\r';
      token[1] = '\n';
      token[2] = 0;
      *token_type = DELIMETER;
   }

   stopflag = 0;
   while ((expression[0] == '\"') || (expression[0] == ' ')) {
      for (i=0;i<ndelim;i++)
	 if (ossim_strncasecmp(expression,delimstr[i],(unsigned int)strlen(delimstr[i]))==0) {
	    stopflag=1;
	    break;
	 }
      if (stopflag) break;
      expression++;
   }

   return_token( expression, token );
   expression += strlen(token);

   if (*token == '\0') {
      *token_type = FINISHED;
      *expression = '\0';
      return expression;
   }

   leftjust(token);
   rightjust(token);

   if (ossim_strcasecmp(token,"AND")==0) {
      strupr(token);
      *token_type = JOIN;
      *token_value = AND;
      while ((expression[0] == '\"') || (expression[0] == ' ')) expression++;
      return expression;
   }

   if (ossim_strcasecmp(token,"OR")==0) {
      strupr(token);
      *token_type = JOIN;
      *token_value = OR;
      while ((expression[0] == '\"') || (expression[0] == ' ')) expression++;
      return expression;
   }

   if (token[0] == '"') {   /* quoted string */
      if (*expression) expression++;
      i = 0;
      while (*expression != '"') {
	 token[i] = *expression;
	 i++;
	 expression++;
	 if (*expression == '\0') {
	    *token_type = ERROR;
	    *token_value = ERROR;
	    return expression;
	 }
      }

      while ((expression[0] == '\"') || (expression[0] == ' '))
	 expression++;
      token[i] = '\0';
      *token_type = STRING;
      *token_value = (int)strlen(token);
      return expression;
   }

   for (i=0;i<ndelim;i++) {
      if (ossim_strcasecmp(token,delimstr[i])==0) {
	 *token_type = LOP;
	 *token_value = i;
	 return expression;
      }
   }

   for (i=0;i<nfields;i++) {
      if (ossim_strcasecmp(token,fieldname[i])==0) {
	 strupr(token);
	 *token_type = FIELD;
	 *token_value = i;
	 return expression;
      }
   }

   *token_type = VALUE;
   *token_value = 0;
   return expression;
}


/*************************************************************************
 *
 *N  parse_expression
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns a list of selection expression clause
 *     structures.  This list forms the internal structure of the query
 *     expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression   <input>==(char *) selection expression string.
 *    table        <input>==(vpf_table_type) VPF table structure.
 *    return      <output>==(linked_list_type) list of expression clauses.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    char *get_token( char *expression, char *token, int *token_type,
 *    int *token_value) VPFQUERY.C
 *    void display_message( char *input) USER DEFINED
 *    linked_list_type ll_init() LINKLIST.C
 *    void ll_reset( linked_list_type list ) LINKLIST.C
 *    void ll_insert( void *element, unsigned size,
 *    position_type position ) LINKLIST.C
 *E
 *************************************************************************/
static linked_list_type parse_expression( char *expression, vpf_table_type table )
{
   linked_list_type exprlist;
   position_type pos;
   expr_type expr;
   int i, token_type, token_value;
   char token[260];

   exprlist = ll_init();
   pos = exprlist;

   /* Set up static globals */

   nfields = table.nfields;

   fieldname = (char **)memalloc( (nfields+2) * sizeof(char *) );
   fieldcol = (int *)memalloc( (nfields+2) * sizeof(int) );

   for (i=0;i<table.nfields;i++) {
      fieldname[i] = (char *)memalloc(40*sizeof(char));
      strcpy(fieldname[i], table.header[i].name);
      fieldcol[i] = i;
   }

   /*****/

   expression = get_token( expression, token, &token_type, &token_value );
   while (token_type != FINISHED) {
      if (token_type != FIELD) {
	 display_message("Expression syntax error -- Invalid field name");
	 ll_reset(exprlist);
	 exprlist = NULL;
	 break;
      }
      expr.field = token_value;

      expression = get_token( expression, token, &token_type, &token_value );
      if (token_type != LOP) {
	 display_message("Expression syntax error");
	 ll_reset(exprlist);
	 exprlist = NULL;
	 break;
      }
      expr.op = (char)token_value;

      expression = get_token( expression, token, &token_type, &token_value );
      if (token_type == ERROR) {
	 display_message("Expression syntax error");
	 ll_reset(exprlist);
	 exprlist = NULL;
	 break;
      }
      strcpy(expr.value,token);

      expression = get_token( expression, token, &token_type, &token_value );
      if (token_type == JOIN) {
	 expr.join = (char)token_value;
	 ll_insert( &expr, sizeof(expr), pos );
	 pos = pos->next;
	 expression = get_token( expression, token, &token_type,
				 &token_value );
      } else if (token_type == FINISHED) {
	 expr.join = '\0';
	 ll_insert( &expr, sizeof(expr), pos );
      } else {
	 display_message("Expression syntax error");
	 ll_reset(exprlist);
	 exprlist = NULL;
	 break;
      }
   }

   for (i=0;i<nfields;i++) free(fieldname[i]);
   free(fieldname);
   free(fieldcol);

   return exprlist;
}



/*************************************************************************
 *
 *N  comp
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function is a general comparison function for comparing two
 *     buffers.  NOTE:  This function compares the bytes of the buffers
 *     as unsigned characters.  Numeric values > 255 should not be
 *     compared with this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(void *) first buffer to compare.
 *    val2    <input>==(void *) second buffer to compare.
 *    size    <input>==(int) number of bytes to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
static int comp( void *val1, void *val2, int size, char op )
{
   int result;

   result = memcmp(val1,val2,size);

   switch (op) {
      case EQ:
	 result = !result;
	 break;
      case NE:
	 break;
      case LT:
	 if (result < 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      case LE:
	 if (result <= 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      case GT:
	 if (result > 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      case GE:
	 if (result >= 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      default:
	 printf("Invalid logical operator (%d)\n",op);
	 result = FALSE;
	 break;
   }
   return result;
}


/*************************************************************************
 *
 *N  strcompare
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function compares two strings with the given logical operator.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(char *) first buffer to compare.
 *    val2    <input>==(char *) second buffer to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
static int strcompare( char *val1, char *val2, char op )
{
   int result;
   char str1[300], str2[300];

   strcpy(str1,val1);
   rightjust(str1);
   strcpy(str2,val2);
   rightjust(val2);

   result = ossim_strcasecmp(str1,str2);

   switch (op) {
      case EQ:
	 result = !result;
	 break;
      case NE:
	 break;
      case LT:
	 if (result < 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      case LE:
	 if (result <= 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      case GT:
	 if (result > 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      case GE:
	 if (result >= 0)
	    result = TRUE;
	 else
	    result = FALSE;
	 break;
      default:
	 printf("Invalid logical operator (%d)\n",op);
	 result = FALSE;
	 break;
   }
   return result;
}


/*************************************************************************
 *
 *N  icompare
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function compares two ossim_int32 integers with the given logical
 *     operator.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(ossim_int32) first buffer to compare.
 *    val2    <input>==(ossim_int32) second buffer to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
static int icompare( ossim_int32 val1, ossim_int32 val2, char op )
{
   int result;

   switch (op) {
      case EQ:
	 result = (val1 == val2);
	 break;
      case NE:
	 result = (val1 != val2);
	 break;
      case LT:
	 result = (val1 < val2);
	 break;
      case LE:
	 result = (val1 <= val2);
	 break;
      case GT:
	 result = (val1 > val2);
	 break;
      case GE:
	 result = (val1 >= val2);
	 break;
      default:
	 printf("Invalid logical operator (%d)\n",op);
	 result = FALSE;
	 break;
   }
   return result;
}


/*************************************************************************
 *
 *N  fcompare
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function compares two floating point numbers with the given
 *     logical operator.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(ossim_int32) first buffer to compare.
 *    val2    <input>==(ossim_int32) second buffer to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
static int fcompare( float val1, float val2, char op )
{
   int result;

   switch (op) {
      case EQ:
	 result = (val1 == val2);
	 break;
      case NE:
	 result = (val1 != val2);
	 break;
      case LT:
	 result = (val1 < val2);
	 break;
      case LE:
	 result = (val1 <= val2);
	 break;
      case GT:
	 result = (val1 > val2);
	 break;
      case GE:
	 result = (val1 >= val2);
	 break;
      default:
	 printf("Invalid logical operator (%d)\n",op);
	 result = FALSE;
	 break;
   }
   return result;
}


/*************************************************************************
 *
 *N  query_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the set of selected rows of a VPF table
 *     based upon the evaluation of the given selection expression string.
 *
 *     The expression is strictly evaluated left to right.  No nesting
 *     is supported, so parentheses are not allowed.  The expression
 *     must match the form:
 *        <field><log op><value> [ <join> <field><log op><value>]
 *     where,
 *        <field> is a valid field name of the table.
 *        <log op> is one of the following: =, <, >, <=, >=, <> (not equal).
 *        <value> is a valid value for the field.
 *        <join> is either " AND " or " OR ".
 *     Any number of clauses (<field><log op><value>) may be joined
 *     together with AND or OR to form the expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression <input>==(char *) selection expression string.
 *    table      <input>==(vpf_table_type) VPF table structure.
 *    return    <output>==(set_type) set of selected rows.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init( ossim_int32 n ) SET.C
 *    void set_insert( ossim_int32 element, set_type set ) SET.C
 *    linked_list_type parse_expression( char *expression,
 *                     vpf_table_type table ) VPFQUERY.C
 *    row_type read_next_row( vpf_table_type table ) VPFREAD.C
 *    position_type ll_first( linked_list_type list ) LINKLIST.C
 *    int ll_end( position_type position ) LINKLIST.C
 *    void ll_element( position_type position, void *element ) LINKLIST.C
 *    void *get_table_element( ossim_int32 field_number,
 * 			 row_type row,
 *			 vpf_table_type table,
 *			 void *value,
 *			 ossim_int32  *count ) VPFREAD.C
 *    void display_message( char *info ) USER DEFINED
 *    static int strcompare( char *val1, char *val2, char op ) VPFQUERY.C
 *    static int icompare( ossim_int32 val1, ossim_int32 val2, char op ) VPFQUERY.C
 *    static int fcompare( float val1, float val2, char op ) VPFQUERY.C
 *    void ll_reset( linked_list_type list ) LINKLIST.C
      void free_row( row_type row, vpf_table_type table) VPFREAD.C
 *E
 *************************************************************************/
set_type query_table( char *expression, vpf_table_type table )
{
   row_type row;
   position_type pos;
   expr_type expr;
   register ossim_int32 i;
   int boolval=FALSE, booltemp=0, join = OR;
   ossim_int32 lval, lval2, count;
   float fval, fval2;
   char tval, tval2, *tptr;
   linked_list_type exprlist;
   set_type select_set;

   select_set = set_init(table.nrows+1);

   if (strcmp(expression,"*")==0) {
      set_on(select_set);
      return select_set;
   }

   exprlist = parse_expression( expression, table );

   if (!exprlist) return select_set;

   if (table.storage == DISK)
      fseek( table.fp, index_pos(1,table), SEEK_SET );

   for (i=1;i<=table.nrows;i++) {

      if (table.storage == DISK)
	 row = read_next_row(table);
      else
	 row = get_row( i, table );

      pos = ll_first(exprlist);
      while (!ll_end(pos)) {
	 ll_element( pos, &expr );
	 switch (table.header[expr.field].type) {
	    case 'I':
	       if (table.header[expr.field].count == 1) {
		  get_table_element( expr.field, row, table, &lval, &count );
		  lval2 = atol(expr.value);
		  booltemp = icompare( lval, lval2, expr.op );
	       } else {
		  display_message(
		     "Selection may not be performed upon arrays");
		  i=table.nrows+1;
	       }
	       break;
	    case 'T':
	       if (table.header[expr.field].count == 1) {
		  get_table_element( expr.field, row, table, &tval, &count );
		  tval2 = expr.value[0];
		  booltemp = comp( &tval, &tval2, sizeof(tval), expr.op );
	       } else {
		  tptr = (char *)get_table_element( expr.field, row, table,
				   NULL, &count );
		  booltemp = strcompare( tptr, expr.value, expr.op );
		  free(tptr);
	       }
	       break;
	    case 'F':
	       if (table.header[expr.field].count == 1) {
		  get_table_element( expr.field, row, table, &fval, &count );
		  if (!is_vpf_null_float(fval)) {
		     fval2 = (float)atof(expr.value);
		     booltemp = fcompare( fval, fval2, expr.op );
		  } else booltemp = FALSE;
	       } else {
		  display_message(
		     "Selection may not be performed upon arrays");
		  i=table.nrows+3;
	       }
	       break;
	    default:
	       display_message("Field type not supported for query");
	       i=table.nrows+3;
	       break;
	 }

	 if (i>table.nrows) break;

	 if (join==OR)
	    boolval = boolval || booltemp;
	 else
	    boolval = boolval && booltemp;

	 join = expr.join;

	 pos = pos->next;
      }
      free_row( row, table );
      if (boolval) set_insert(i,select_set);
      boolval = FALSE;
      join = OR;

      if (i==table.nrows+3) break;

   }

   ll_reset(exprlist);

   return select_set;
}

