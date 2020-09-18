#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_
#define OK 0
#define BADFORMAT_2DOTS 1
#define BADFORMAT_UNEXPECTEDCHAR 2
#define BADFORMAT_UNBALANCEDPAR 3
#define BADFORMAT_MISSINGARG 4
#define BADFORMAT_MISSINGOP 5
#define DIVISION_BY_ZERO 6

typedef struct token
{
    char header;
    double value;
} token;
int doubleToString(double value, char * buffer);
double stringToDouble(char * init, char * end);
void evaluator(char * in);
char infixToPosfix(token * infix, token * posfix);
char getPrecedence(char first, char second);
char tokenizer(char * in, token * out);
char isBracket(char tok);
char isOperator(char tok);
char isDigit(char tok);
char posfixEvaluator(token * expression, double * result);
char evaluateTok(double first, double sec, char op, double * result);

#endif

