%{
#include <ossim/base/ossimEquTokenDefines.h>

int yywrap()
{
   return 1;
}

%}

string  \"[^\n"]+\"

ws      [ \t]+

alpha   [A-Za-z]
dig     [0-9]
num1    {dig}+\.?([eE][-+]?{dig}+)?
num2    {dig}*\.{dig}+([eE][-+]?{dig}+)?
number  {num1}|{num2}



%%

{ws}    /* skip blanks and tabs */

{number}   {
               return OSSIM_EQU_TOKEN_CONSTANT;
           }
","        {
               return OSSIM_EQU_TOKEN_COMMA;
           }
"/"        {
               return OSSIM_EQU_TOKEN_DIV;
           }

"*"        {
               return OSSIM_EQU_TOKEN_MULT;
           }

"|"        { 
               return OSSIM_EQU_TOKEN_OR_BAR;
           }

"&"        { 
               return OSSIM_EQU_TOKEN_AMPERSAND;
           }
"~"        { 
               return OSSIM_EQU_TOKEN_TILDE;
           }

"+"        {
               return OSSIM_EQU_TOKEN_PLUS;
           }
"-"        {
               return OSSIM_EQU_TOKEN_MINUS;
           }
"%"        {
               return OSSIM_EQU_TOKEN_MOD;
           }
"["        {
               return OSSIM_EQU_TOKEN_LEFT_ARRAY_BRACKET;
           }
"]"        {
               return OSSIM_EQU_TOKEN_RIGHT_ARRAY_BRACKET;
           }
"("     {
               return OSSIM_EQU_TOKEN_LEFT_PAREN;
        }
")"        {
               return OSSIM_EQU_TOKEN_RIGHT_PAREN;
           }
"\^"        {
               return OSSIM_EQU_TOKEN_POWER;
           }
"xor"   {
               return OSSIM_EQU_TOKEN_XOR;
        }

"in"    {
             return OSSIM_EQU_TOKEN_IMAGE_VARIABLE;
        }
"PI"       {
               return OSSIM_EQU_TOKEN_PI;
           }
"log"      {
               return OSSIM_EQU_TOKEN_LOG;
           }
"log10"      {
               return OSSIM_EQU_TOKEN_LOG10;
           }
"exp"      {
               return OSSIM_EQU_TOKEN_EXP;
           } 
"sin"      {
               return OSSIM_EQU_TOKEN_SIN;
           }
"sind"     {
               return OSSIM_EQU_TOKEN_SIND;
           }
"asin"      {
               return OSSIM_EQU_TOKEN_ASIN;
           }
"asind"     {
               return OSSIM_EQU_TOKEN_ASIND;
           }
"cos"      {
               return OSSIM_EQU_TOKEN_COS;
           }
"cosd"     {
               return OSSIM_EQU_TOKEN_COSD;
           }
"acos"      {
               return OSSIM_EQU_TOKEN_ACOS;
           }
"acosd"     {
               return OSSIM_EQU_TOKEN_ACOSD;
           }
"tan"     {
               return OSSIM_EQU_TOKEN_TAN;
           }
"tand"     {
               return OSSIM_EQU_TOKEN_TAND;
           }
"atan"     {
               return OSSIM_EQU_TOKEN_ATAN;
           }
"atand"     {
               return OSSIM_EQU_TOKEN_ATAND;
           }
"abs"      {
               return OSSIM_EQU_TOKEN_ABS;
           }
"sqrt"      {
               return OSSIM_EQU_TOKEN_SQRT;
           }
"min"      {
               return OSSIM_EQU_TOKEN_MIN;
           }
"max"      {
               return OSSIM_EQU_TOKEN_MAX;
           }
"conv"     {
               return OSSIM_EQU_TOKEN_CONV;
           }
"shift"    {
               return OSSIM_EQU_TOKEN_SHIFT;
           }
"blurr"    {
               return OSSIM_EQU_TOKEN_BLURR;
           }
"band"     {
               return OSSIM_EQU_TOKEN_BAND;
           }
"assign_band" {
               return OSSIM_EQU_TOKEN_ASSIGN_BAND;
              }
"clip" {
               return OSSIM_EQU_TOKEN_CLIP;
        }
"clamp" {
               return OSSIM_EQU_TOKEN_CLAMP;
        }
"==" {
               return OSSIM_EQU_TOKEN_BEQUAL;
}
"\>" {
               return OSSIM_EQU_TOKEN_BGREATER;
}
"\>=" {
               return OSSIM_EQU_TOKEN_BGREATEROREQUAL;
}
"\<" {
               return OSSIM_EQU_TOKEN_BLESS;
}
"\<=" {
               return OSSIM_EQU_TOKEN_BLESSOREQUAL;
}
"\<\>" {
               return OSSIM_EQU_TOKEN_BDIFFERENT;
}

%%
