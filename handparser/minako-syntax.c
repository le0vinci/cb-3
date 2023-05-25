#include <stdio.h>
#include <stdlib.h>
#include "minako.h"

/* macro for the error message */
#define ERROR_MESSAGE "ERROR: Syntaxfehler in Zeile "

/* functions declaration */
void program();
void functioncall();
void statementlist();
void block();
void statement();
void callOrAssignment();
void ifstatement();
void returnstatement();
void returnAssigment();
void fnkPrintf();
void type();
void assignment();
void exprOrAssignment();
void expr();
void simpexprExtension();
void simpexpr();
void termExtension();
void term();
void factorExtension();
void factor();
void functionOrNot();


int currentToken;		//global var to store current token
int nextToken;			//global var to store next token

int getNextToken(){
    int token = yylex(); 				  //to get the next token by yylex
    return (token != EOF) ? token : -1;   //if next token is End_OF_File --> -1, otherwise --> next token
}

void advance(){				// to set the next token by getNextToken
    nextToken = getNextToken();
}

void eat(int token){ 		// consume a token if matches the next token
    if(token == nextToken){
        advance();
    }
}

void eatAndCall(int token, void (*func)()){   //helper for eat() to consume token and call function
    eat(token);
    func();
}

void program(){
    switch (nextToken)
    {    		   /*  consume next token of type boolean, float, int or void
					for a function definition and call statement list and program functions recursively */
        case KW_BOOLEAN:
        case KW_FLOAT:
        case KW_INT:
        case KW_VOID:
            type(); 		//Calls the type function
            eat(ID);		//Consumes token (ID)
            eat('('); 		//Consumes token (open parenthesis)
            eat(')');		//Consumes token (close parenthesis)
            eat('{');		//Consumes token (open brace)
            statementlist();
            eat('}');		//Consumes token (close brace)
            program();		// Calls program function recursively
            break;
        case EOF:			//If next token end of file --> break switch
            break;
        default: 			//For not defined token values --> terminate with error message
            fprintf(stderr, ERROR_MESSAGE "97 \n");
            exit(-1);
    }
}


void statementlist(){
	switch (nextToken)
	{
	case '{':
	case KW_IF:
	case KW_RETURN:
	case KW_PRINTF:
	case ID:
		block();
		statementlist();
        break;
	default:
		break;
	}
}

void block(){
	switch (nextToken)
	{
	case '{':
		eat('{');
		statementlist();
		eat('}');
		break;
	case KW_IF:
	case KW_RETURN:
	case KW_PRINTF:
	case ID:
		statement();
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "134 \n");
      	exit(-1);
	}
}

void statement(){
	switch (nextToken)
	{
	case KW_IF:
		ifstatement();
		break;
	case KW_RETURN:
		returnstatement();
		eat(';');
		break;
	case KW_PRINTF:
		fnkPrintf();
		eat(';');
		break;
	case ID:
		eat(ID);
		callOrAssignment();
		eat(';');
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "160 \n");
      	exit(-1);
	}
}

void callOrAssignment(){
	switch (nextToken)
	{
	case '(':
		eat('(');
		eat(')');
		break;
	case '=':
		eat('=');
		assignment();
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "178 \n");
      	exit(-1);
	}
}

void ifstatement(){
	switch (nextToken)
	{
	case KW_IF:
		eat(KW_IF);
		eat('(');
		assignment();
		eat(')');
		block();
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "195 \n");
      	exit(-1);
	}
}

void returnstatement(){
	switch (nextToken)
	{
	case KW_RETURN:
		eat(KW_RETURN);
		returnAssigment();
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "209 \n");
      	exit(-1);
	}
}

void fnkPrintf(){
	switch (nextToken)
	{
	case KW_PRINTF:
		eat(KW_PRINTF);
		eat('(');
		assignment();
		eat(')');
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "225 \n");
      	exit(-1);
	}
}

void type(){
	switch (nextToken)
	{
	case KW_BOOLEAN:
	case KW_FLOAT:
	case KW_INT:
	case KW_VOID:
		eat(nextToken);
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "241 \n");
      	exit(-1);
	}
}

void returnAssigment(){
	switch (nextToken)
	{
	case ID:
	case '-':
	case CONST_INT:
	case CONST_FLOAT:
	case CONST_BOOLEAN:
	case '(':
		assignment();
		break;
	default:
		break;
	}
}

void assignment(){
	switch (nextToken)
	{
	case ID:
        eat(ID);
		exprOrAssignment();
		break;
	case '-':
	case CONST_INT:
	case CONST_FLOAT:
	case CONST_BOOLEAN:
	case '(':
		expr();
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "278 \n");
      	exit(-1);
	}
}

void exprOrAssignment(){
    switch (nextToken)
	{
	case '=':
        eat('=');
		assignment();
		break;
	case '(':
        eat('(');
        eat(')');
		factorExtension();
        termExtension();
        simpexprExtension();
		break;
    case '*':
    case '/':
    case AND:
        factorExtension();
        termExtension();
        simpexprExtension();
        break;
    case '+':
    case '-':
    case OR:
        termExtension();
        simpexprExtension();
        break;
    case EQ:
    case NEQ:
    case GRT:
    case LSS:
    case LEQ:
    case GEQ:
        simpexprExtension();
        break;
	default:
		break;
	}
}

void expr(){
	switch (nextToken)
	{
	case '-':
	case CONST_INT:
	case CONST_FLOAT:
	case CONST_BOOLEAN:
	case '(':
		simpexpr();
		simpexprExtension();
		break;
	default:
		fprintf(stderr, ERROR_MESSAGE "336 \n");
      	exit(-1);
	}
}

void simpexprExtension(){
    switch (nextToken)
	{
	case EQ:
	case NEQ:
	case LEQ:
	case GEQ:
	case LSS:
	case GRT:
		eat(nextToken);
		simpexpr();
		break;
	default:
		break;
	}
}

void simpexpr(){
	switch (nextToken)
	{
	case '-':
		eat('-');
		term();
		termExtension();
		break;
	case CONST_INT:
	case CONST_FLOAT:
	case CONST_BOOLEAN:
	case ID:
	case '(':
		term();
		termExtension();
        break;
	default:
		fprintf(stderr, ERROR_MESSAGE "376 \n");
      	exit(-1);
	}
}

void termExtension(){
	switch (nextToken)
	{
	case '+':
	case '-':
	case OR:
		eat(nextToken);
		term();
		termExtension();
		break;
	default:
		break;
	}
}

void term(){
	switch (nextToken)
	{
	case CONST_INT:
	case CONST_FLOAT:
	case CONST_BOOLEAN:
	case ID:
	case '(':
		factor();
		factorExtension();
		break;
	default:
		break;
	}
}

void factorExtension(){
	switch (nextToken)
	{
	case '*':
	case '/':
	case AND:
		eat(nextToken);
		factor();
		factorExtension();
		break;
	default:
		break;
	}
}


void factor(){
    switch (nextToken)
    {
        case CONST_INT:
            eatAndCall(CONST_INT, functionOrNot);
            break;
        case CONST_FLOAT:
            eatAndCall(CONST_FLOAT, functionOrNot);
            break;
        case CONST_BOOLEAN:
            eatAndCall(CONST_BOOLEAN, functionOrNot);
            break;
        case ID:
            eatAndCall(ID, functionOrNot);
            break;
        case '(':
            eat('(');
            assignment();
            eat(')');
            break;
        default:
            fprintf(stderr, ERROR_MESSAGE "450 \n");
            exit(-1);
    }
}


void functionOrNot(){
    switch (nextToken)
    {
    case '(':
        eat('(');
        eat(')');
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[]){
    if (argc != 2) {
    yyin = stdin;
    } else {
        yyin = fopen(argv[1], "r");
        if (yyin == 0) {
            fprintf(
                stderr,
                "Fehler: Konnte Datei %s nicht zum lesen oeffnen.\n",
                argv[1]
            );
            exit(-1);
        }
    }

    nextToken = getNextToken();
    program();

    return 0;
}
