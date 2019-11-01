%option noyywrap
%{
	#include<stdio.h>
	#include "tp5.tab.h"
%}

%%
algoritmo	{return ALGORITMO;}
finAlgoritmo	{return FINALGORITMO;}
variable	{return VARIABLE;}
escribir	{return ESCRIBIR;}
leer		{return LEER;}
si		{return SI;}
finSi		{return FINSI;}
[a-zA-Z]+	{return ID;}
[0-9]+		{return NUM;}
[+-]		{return OPERADOR;}
[$/>\[\]=]	{return yytext[0];}
.		;
%%