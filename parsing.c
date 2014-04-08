#include <stdio.h>
#include <stdlib.h>
#include "libraries/mpc.h"

// For Windows
#ifdef _WIN32

#include <string.h>

static char buffer[2048];

char* readline(char* prompt) {
	fputs("mexlisp> ", stdout);
	fgets(buffer, 2048, stdin);
	char *cpy = malloc(strlen(buffer) + 1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy) - 1] = '\0';
	return cpy;
}

void add_history(char* unused) {}

#else

// Mac, Linux, etc.
#include "libraries/readline.h"

#endif

static char* version_number = "0.1";

// Use operator string to see which operation to perform
long eval_op(long x, char* op, long y) {
	if (strcmp(op, "+") == 0) { return x + y; }
	if (strcmp(op, "-") == 0) { return x - y; }
	if (strcmp(op, "*") == 0) { return x * y; }
	if (strcmp(op, "/") == 0) { return x / y; }
	return 0;
}

long eval(mpc_ast_t* t) {
	// if number, return it directly
	if (strstr(t->tag, "number")) {
		return atoi(t->contents);
	} else {
		// the operator is always in the second child
		char* operator = t->children[1]->contents;

		long x = eval(t->children[2]);

		int i = 3;
		// use the same operator for the remaining children
		while (strstr(t->children[i]->tag, "expr")) {
			x = eval_op(x, operator, eval(t->children[i]));
			i++;
		}

		return x;
	}
}

int main(int argc, char** argv) {

	printf("mexlisp version %s\n", version_number);
	puts("¡A huevo!");

	// Create some parsers
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Text_Operator = mpc_new("text_operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Mexlisp = mpc_new("mexlisp");

	mpca_lang(MPC_LANG_DEFAULT,
	"																				\
		number   	  : /-?[0-9]+(\\.[0-9][0-9]?)?/ ; 								\
		operator 	  : '+' | '-' | '*' | '/' | '\045';								\
		text_operator : \"sum\" | \"res\" | \"mul\" | \"div\" | \"mod\"; 			\
		expr     	  : <number> | '(' (<operator> | <text_operator>) <expr>+ ')' ; \
		mexlisp  	  : /^/ (<operator> | <text_operator>) <expr>+ /$/ ;	    	\
	",
	Number, Operator, Text_Operator, Expr, Mexlisp);

	while (1) {
		// Get input and output it using editline
		char* input = readline("mexlisp> ");
		add_history(input);
		
		// Attempt to parse the input
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Mexlisp, &r)) {
			// success
      		printf("%li\n", eval(r.output));
      		mpc_ast_delete(r.output);
		} else {
			// error
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}

		free(input);
	}

	// Undefine and delete the parsers
	mpc_cleanup(4, Number, Operator, Text_Operator, Expr, Mexlisp);

	return 0;
}