#include <iostream>
#include "hello.h"
#include "console.h"
#include "tokenscanner.h"
#include "stack.h"
#include <math.h>
#include "simpio.h"
#include <cctype>


using namespace std;



/*
 * извлекаем 2 операнда из стека
 * извлекаем оператор из стека
 * выполняем операцию соответственно оператору
 * результат операции помещаем в стек операндов
 */
void simpeOperation(Stack <double> & operandStack,Stack <char> & operatorStack){
    double result;
    double rightOperand = operandStack.pop();
    double leftOperand = operandStack.pop();
    char operatorFromStack = operatorStack.pop();
    switch(operatorFromStack){
        case '+' : result = leftOperand + rightOperand; break;
        case '-' : result = leftOperand - rightOperand; break;
        case '*' : result = leftOperand * rightOperand; break;
        case '/' :
        case ':' : result = leftOperand / rightOperand; break;
        //case '%' : result = leftOperand % rightOperand; break;
        case '^' : result = pow( leftOperand, rightOperand); break;
    }
    cout << leftOperand << operatorFromStack <<rightOperand << "="<< result <<endl;
    operandStack.push( result);
}

/**
 * This function определяет приоритет выполнения
 переданного в нее оператора.
 Здесь чем выше приоритет исследуемого оператора тем больше возвращаемое значение. Если символ переданный в качестве параметра не является математическим оператором функция вернет -1
 * -----------------------------------------------------
 * @brief priorityOperator
 * @param ch
 * @return
 */
int priorityOperator(char ch){
    string operators = "-+*/:^";
    if( operators.find(ch) ==  string::npos ) return -1;
    if( ch == '-' || ch == '+') return 1;
    if( ch == '*' || ch == '/' || ch == ':') return 2;
    if( ch == '^') return 3;
    return 4;
}

void executeSmallerOper(Stack <char> & operatorStack, Stack <double> & operandStack, int priority){
    //Если стек операторов пустой выйти из подпрограммы
    if (operatorStack.isEmpty()) return;

    if (priorityOperator(operatorStack.peek()) < priority) return;
    simpeOperation(operandStack,operatorStack);
    executeSmallerOper(operatorStack, operandStack, priority);


}

void toLower(string & str) {
   for (int i = 0; i < str.length(); i++) {
      str[i] = tolower(str[i]);
   }
}



void getFunction(string f, double & operand){

    toLower(f);

        if (f == "sin") operand = sin(operand);else
        if (f == "cos") operand = cos(operand);else
        if (f == "tg")  operand = tan(operand);else
        if (f == "ctg") operand = atan(operand);else
        if (f == "lg") operand = log10(operand);else
        if (f == "ln") operand = log(operand);else
        if (f == "sqrt") operand = sqrt(operand);
        else
        cout << "Funcrion "<< f << " incorrect." << endl;








}


double evaluating(TokenScanner & scanner, Stack <double> & operandStack,Stack <char> & operatorStack ){



     //для каждого токена
     while(scanner.hasMoreTokens()){

         //Определяем его значение и тип
         string carrentToken = scanner.nextToken();
         TokenType carrentTokenType = scanner.getTokenType(carrentToken);
         cout << carrentToken << " " << carrentTokenType << endl;//+++del

         if (carrentTokenType == TokenType::NUMBER){

             operandStack.push(atof(carrentToken.c_str()));
         }

         //если это оператор принадлежащий классу  TokenScanner
         if (carrentTokenType == TokenType::OPERATOR){
             char carrentOperator =  carrentToken[0];
             int priority = priorityOperator ( carrentOperator);

             //Если это допустимый в простейшем арифметическом выражении оператор "+-/*"
             if (priority >= 0){

                 //если стек операторов не пустой
                 if (!operatorStack.isEmpty()){

                     if(carrentOperator == '^'){
                         if(operatorStack.peek() == '^'){
                             priority++;
                         }
                     }

                     /*Выполнить простую последовательность операторов,
                     приоритет которых ниже текущего оператора*/
                     executeSmallerOper(operatorStack, operandStack, priority);

                 }
                 //если стек операндов пустой (отслеживаем + или - в начале выражения)
                 if(operandStack.isEmpty()){
                     if(carrentOperator == '+') continue;
                     if(carrentOperator == '-') operandStack.push(0);
                 }



                 operatorStack.push(carrentToken[0]);
             }
             else{
                 if(carrentOperator == '('){
                     Stack <double> nOperandStack;
                     Stack <char> nOperatorStack;
                     //operatorStack.push(carrentOperator);
                     double result = evaluating(scanner, nOperandStack, nOperatorStack );
                     operandStack.push(result);
                 }
                 if(carrentOperator == ')') {
                     if (!operatorStack.isEmpty()){
                         break;
                     }
                     /*if(operatorStack.peek()=='('){
                         operatorStack.pop();

                     }else{
                         cout << "Error" << endl;
                     }*/
                     break;
                 }
             }



         }
         /*выполнение функции*/
         if (carrentTokenType == TokenType::WORD){
             string currentFunction = carrentToken;
             Stack <double> nOperandStack;
             Stack <char> nOperatorStack;
             //operatorStack.push(carrentOperator);
             double result = evaluating(scanner, nOperandStack, nOperatorStack );
             getFunction(currentFunction, result);
             operandStack.push(result);
             cout << "Function"<<endl;
         }
     }
     /*
      * заведомо наибольший приоритет.
      * Любой возможный оператор имеет приоритет меньше,
      * чем BIGGER_PRIORITY
      */
     int BIGGER_PRIORITY = 100;
     if (!operatorStack.isEmpty())
     executeSmallerOper(operatorStack, operandStack, -1); // функция будет выполняться пока не израсходует все операторы оставшиеся в стеке

     double result = operandStack.pop();
     cout << "result " << result << endl;
     //cout << operatorStack.pop() << endl;
     return result;

}

bool errBrackets(string expression){
    int counterbrackets = 0;
    for ( string::iterator symbol=expression.begin(); symbol!=expression.end(); ++symbol){

        if(*symbol == '(') counterbrackets++;
        if(*symbol == ')') counterbrackets--;
        if (counterbrackets < 0) break;//первая ), а не (
    }
    cout << "counterbrackets = " << counterbrackets <<endl;
    return counterbrackets;
}


int main() {
    Stack <double> operand_Stack;
    Stack <char> operator_Stack;
    Stack <char>  parenthesis_Stack;

    string line = getLine("> ");
    cout << line << endl;

    if(errBrackets(line)){
        cout << "errBrackets" << endl;
        return 1;
    }
    TokenScanner expression(line);
    expression.scanNumbers();
    evaluating(expression, operand_Stack, operator_Stack );




    return 0;
}

