#include <calculator.h>
#include <stdlib.h>

#define OPERATORS 8
#define NULLTERMINATED 0
#define NUMBER 1



char operators[OPERATORS] = {'+', '-', '*', '/', '%', '^', '(', ')'};
char mat[OPERATORS - 1][OPERATORS] = {
    {1, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}};





char * error_msg[]={"Error de formato en la expresion: 2 signos de puntuacion en un mismo numero",
"Error de formato en la expresion: caracter invalido ingresado",
"Error de formato en la expresion: falta algun parentesis",
"Error de formato en la expresion: falta argumento",
"Error de formato en la expresion: falta operador",
"El limite de una division con denominador tendiendo a 0, es infinito"
};


void evaluator(char *in){
    token toks[100];
    token posfix[100];
    double result = 0;

    char error = tokenizer(in, toks);
    if(error != OK){
        puts(error_msg[error-1]);
        return;
    }
    error = infixToPosfix(toks, posfix);
    if(error != OK){
       puts(error_msg[error-1]);
        return;
    }
    error = posfixEvaluator(posfix, &result);
    if(error != OK){
        puts(error_msg[error-1]);
        return;
    }
    printf("%f",result);
}

char posfixEvaluator(token *expression, double *result){
    double stack[100];
    int idx = 0;
    int exprIdx = 0;
    while (expression[exprIdx].header != NULLTERMINATED)
    {
        if (expression[exprIdx].header == NUMBER)
        {
            stack[idx] = expression[exprIdx].value;
            idx++;
            exprIdx++;
        }
        else
        {
            if (idx == 0)
            {
                return BADFORMAT_MISSINGARG;
            }
            double aux1 = stack[--idx];
            if (idx == 0)
            {
                return BADFORMAT_MISSINGARG;
            }
            double aux2 = stack[--idx];
            char error = evaluateTok(aux2, aux1, expression[exprIdx].header,&stack[idx]);
            if(error!=OK) return error;
            idx++;
            exprIdx++;
        }
    }

    if (idx == 0)
    {
        return BADFORMAT_MISSINGARG;
    }
    *result = stack[--idx];
    if (idx != 0)
    {
        return BADFORMAT_MISSINGOP;
    }
    return OK;
}

char tokenizer(char *in, token *out){
    int signo = 1;
    int i = 0;
    int tokIdx = 0;
    int comma;
    while (in[i] != 0)
    {

        if (isOperator(in[i]) || isBracket(in[i]))
        {
            if(in[i]== '-' && (tokIdx == 0 || isOperator(out[tokIdx-1].header) || out[tokIdx-1].header == '(')){
                signo = -signo;
            }else{
                out[tokIdx].header = in[i];
                tokIdx++;
            }
            i++;
        }
        else if (isDigit(in[i]))
        {
            comma = 0;
            int j = i + 1;
            while (isDigit(in[j]) || in[j] == '.' || in[j] == ',')
            {
                if (in[j] == '.' || in[j] == ',')
                {
                    if (comma == 0)
                    {
                        comma = 1;
                    }
                    else
                    {
                        return BADFORMAT_2DOTS;
                    }
                }

                j++;
            }
            out[tokIdx].header = NUMBER;
            out[tokIdx].value = stringToDouble(in + i, in + j)*signo;
            signo = 1;
            i = j;
            tokIdx++;
        }
        else if (in[i] == ' ')
        {
            do
            {
                i++;
            } while (in[i] == ' ');
        }
        else
        {
            return BADFORMAT_UNEXPECTEDCHAR;
        }
    }
    out[tokIdx].header = NULLTERMINATED;
    return OK;
}

char infixToPosfix(token *infija, token *posfija){
    int i = 0;
    token tok;
    token stack[100];
    int idx = 0;
    int out = 0;
    while (infija[i].header != NULLTERMINATED)
    {
        tok = infija[i];
        if (isOperator(tok.header) || isBracket(tok.header))
        {
            if (idx == 0)
            {
                if (tok.header == ')')
                {
                    return BADFORMAT_UNBALANCEDPAR;
                }
                stack[idx] = tok;
                idx++;
            }
            else
            {
                while ((idx != 0) && getPrecedence(stack[idx - 1].header, tok.header))
                {
                    posfija[out] = stack[--idx];
                    out++;
                }
                if (tok.header == ')')
                {
                    if (stack[idx - 1].header == '(')
                    {
                        idx--;
                    }
                    else
                    {
                        return BADFORMAT_UNBALANCEDPAR;
                    }
                }
                else
                {
                    stack[idx++] = tok;
                }
            }
        }
        else
        {
            posfija[out++] = tok;
        }
        i++;
    }

    while (idx != 0)
    {
        if (isBracket(stack[idx - 1].header))
        {
            return BADFORMAT_UNBALANCEDPAR;
        }
        else
        {
            posfija[out++] = stack[--idx];
        }
    }
    posfija[out].header = NULLTERMINATED;
    return OK;
}

char getPrecedence(char first, char second){
    for (int i = 0; i < OPERATORS; i++)
    {
        if (first == operators[i])
        {
            first = i;
        }
        if (second == operators[i])
        {
            second = i;
        }
    }

    return mat[(unsigned char)first][(unsigned char)second];
}

char isBracket(char tok){
    return (tok == '(' || tok == ')');
}

char isOperator(char tok){
    for (int i = 0; i < OPERATORS - 2; i++)
    {
        if (tok == operators[i])
        {
            return 1;
        }
    }
    return 0;
}

char isDigit(char tok){
    return tok >= '0' && tok <= '9';
}

char evaluateTok(double first, double second, char op, double * result){
    switch (op)
    {
    case '+':
        *result = first + second;
        break;
    case '-':
        *result = first - second;
        break;
    case '*':
        *result = first * second;
        break;
    case '/':
        if(second==0) return DIVISION_BY_ZERO;
        *result = first / second;
        break;
    case '^':
        *result = 1;
        break;
    case '%':
        if(second==0) return DIVISION_BY_ZERO;
        *result = ((int)first)%((int)second);
        break;
    }
    return OK;
}
