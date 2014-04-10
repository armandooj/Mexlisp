#include "brain.h"
#include <string.h>

// Use the operator's string to see which operation to perform
long eval_op(long x, char* op, long y) {
	if (strcmp(op, "+") == 0 	|| strcmp(op, "sum") == 0) { return x + y; }
	if (strcmp(op, "-") == 0 	|| strcmp(op, "res") == 0) { return x - y; }
	if (strcmp(op, "*") == 0 	|| strcmp(op, "mul") == 0) { return x * y; }
	if (strcmp(op, "/") == 0 	|| strcmp(op, "div") == 0) { return x / y; }
	if (strcmp(op, "\045") == 0 || strcmp(op, "mod") == 0) { return x % y; }
	if (strcmp(op, "^") == 0 	|| strcmp(op, "exp") == 0) { return x << y; }	
	if (strcmp(op, "min") == 0) { return x > y ? y : x; }
	if (strcmp(op, "max") == 0) { return x > y ? x : y; }
	return 0;
}