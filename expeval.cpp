#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#define STACK_INIT_SIZE 100//初始分配量
#define STACKINCREMENT 10//存储空间分配量

//建立栈
typedef struct Opter {
	char* top = NULL;
	char* base = NULL;
	int stacksize;
}Stack_Op;

int InitStack_Op(Stack_Op& Stack) {
	Stack.base = (char*)malloc(STACK_INIT_SIZE * sizeof(char));
	if (!Stack.base) {
		return 0;
	}
	Stack.top = Stack.base;
	Stack.stacksize = STACK_INIT_SIZE;
	return 1;
}//InitStack

int Push_Op(Stack_Op& Stack, char& e) {
	if (Stack.top - Stack.base >= Stack.stacksize) {
		Stack.base = (char*)realloc(Stack.base, (Stack.stacksize + STACKINCREMENT) * sizeof(char));
		if (!Stack.base) return 0;
		Stack.top = Stack.base + Stack.stacksize;
		Stack.stacksize += STACKINCREMENT;
	}
	*Stack.top++ = e;
	return 1;
}//push

int Pop_Op(Stack_Op& Stack, char& e) {
	if (Stack.top == Stack.base) return 0;
	e = *--Stack.top;
	return 1;
}//pop

char GetTop_Op(Stack_Op& Stack) {
	if (Stack.top == Stack.base) {
		return 0;
	}
	return(*(Stack.top - 1));
}//get top
//符号栈

typedef struct Data {
	double* top = NULL;
	double* base = NULL;
	int stacksize;
}Stack_Da;

int InitStack_Da(Stack_Da& Stack) {
	Stack.base = (double*)malloc(STACK_INIT_SIZE * sizeof(double));
	if (!Stack.base) {
		return 0;
	}
	Stack.top = Stack.base;
	Stack.stacksize = STACK_INIT_SIZE;
	return 1;
}//InitStack

int Push_Da(Stack_Da& Stack, double& e) {
	if (Stack.top - Stack.base >= Stack.stacksize) {
		Stack.base = (double*)realloc(Stack.base, (Stack.stacksize + STACKINCREMENT) * sizeof(double));
		if (!Stack.base) return 0;
		Stack.top = Stack.base + Stack.stacksize;
		Stack.stacksize += STACKINCREMENT;
	}
	*Stack.top++ = e;
	return 1;
}//push

int Pop_Da(Stack_Da& Stack, double& e) {
	if (Stack.top == Stack.base) return 0;
	e = *--Stack.top;
	return 1;
}//pop

double GetTop_Da(Stack_Da& Stack) {
	if (Stack.top == Stack.base) {
		return 0;
	}
	return(*(Stack.top - 1));
}//get top

//数字栈
//函数声明
char OP[] = { '+','-','*','/','(',')','#','[',']','^' };//所有运算符
int Form(char* str);//表达式的格式是否正确
void Evalue(char argv[1]);//表达式求值
char Procede(char frm, char lat);//运算符优先级
int In(char c, char* str);//c是否为表达式
double Opterater(double op1, char theta, double op2);
double change(char* tmp);
int getIndex(char tmp);
//
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("ERROR_01");
		return 1;
	}
	if (Form(argv[1])) {
		printf("ERROR_02");
		return 1;
	}
	Evalue(argv[1]);
	return 0;
}

void Evalue(char* str) {//按照书上伪代码操作
	Stack_Op OPTR;
	Stack_Da OPND;
	char theta = NULL, * str1 = NULL, * cur = NULL, state = '#';
	double op1 = 0, op2 = 0, flo = 0, sum = 0;
	char num[10];//
	int i = 0, j = 0;
	InitStack_Op(OPTR);
	InitStack_Da(OPND);
	str1 = (char*)malloc((strlen(str) + 1) * sizeof(char));
	for (i = 0; i < strlen(str); i++) {
		//将原表达式复制到新的字符串中并在末尾加上#符号
		str1[i] = str[i];
	}
	str1[strlen(str)] = '#';//表达式以#开始以#结束
	Push_Op(OPTR, state);
	cur = str1;
	while (*cur != '#' || GetTop_Op(OPTR) != '#') {
		if (!In(*cur, OP)) {   //判断当前字符是否为数字{
			j = 0;
			do {
				num[j++] = *cur;
				cur++;
			} while (*cur >= '0' && *cur <= '9' || *cur == '.');  //可存入一个或多个数字字符    
			num[j] = '\0';                  //将输入的连续多个数字字符拼成了字符串  

			flo = change(num);								//将char转为double   
			Push_Da(OPND, flo);
		}//将转换后的数压入操作数栈中    
		else {
			switch (Procede(GetTop_Op(OPTR), *cur)) {
			case'<':
				Push_Op(OPTR, *cur);
				cur++;
				break;
			case'=':
				Pop_Op(OPTR, theta);
				cur++;
				break;
			case'>':
				Pop_Op(OPTR, theta);
				Pop_Da(OPND, op2);
				Pop_Da(OPND, op1);
				sum = Opterater(op1, theta, op2);
				Push_Da(OPND, sum);
				break;
			}
		}
	}
	printf("%g", GetTop_Da(OPND));
}

double Opterater(double op1, char theta, double op2) {
	switch (theta) {
	case '+': return op1 + op2;
	case '-': return op1 - op2;
	case '*': return op1 * op2;
	case '/': {
		if (!op2) {
			printf("ERROR_03");
			exit(1);
		}
		else { return op1 / op2; }
	}
	case '^': return  pow(op1, op2);
	}
}

int getIndex(char tmp) {
	switch (tmp) {
	case'+':return 0;
	case'-':return 1;
	case'*':return 2;
	case'/':return 3;
	case'^':return 4;
	case'(':return 5;
	case')':return 6;
	case'[':return 8;
	case']':return 9;
	case'#':return 7;
	}
}

char Procede(char frm, char lat) {
	//优先级矩阵
	char Priority[10][10] =
	{  // 第二个
	   // +   -   *   /   ^   (   )  #    [    ]  第一个
		{'>','>','<','<','<','<','>','>','<','>'},//+
		{'>','>','>','<','<','<','>','>','<','>'},//-

		{'>','>','>','>','<','<','>','>','<','>'},//*
		{'>','>','>','>','<','<','>','>','<','>'},///

		{'>','>','>','>','<','<','>','>','<','>'},//^

		{'<','<','<','<','<','<','=','<','<','='},//(
		{'>','>','>','>','>','>','>','>','>','>'},//)

		{'<','<','<','<','<','<','<','=','<','<'},//#

		{'<','<','<','<','<','=','<','<','<','='},//[
		{'>','>','>','>','>','>','>','>','>','>'},//]
	};
	int frm_Idx = getIndex(frm);
	int lat_Idx = getIndex(lat);
	return Priority[frm_Idx][lat_Idx];
}

double change(char* tmp) {
	//将num字符串变为double
	double sum = 0;
	int len = 0, loa = 0;//标记长度和小数点的位置
	len = strlen(tmp); loa = len;
	char* p = tmp;
	for (p; p < tmp + strlen(tmp); p++) {
		if (*p == '.') {
			loa = p - tmp + 1;
		}
		else { sum = sum * 10 + *p - '0'; }
	}
	sum = sum / pow(10, (len - loa));
	return sum;
}

int In(char c, char* str) {
	int i = 0;
	for (i = 0; i < strlen(str); i++) {
		if (c == str[i]) {
			return 1;
		}
	}
	return 0;
}
int Form(char* str) {
	/*表达式常见错误
		1++ ([  1+a
		*/
	int i = 0;
	int flag = 1;
	Stack_Op form;
	int isle = 0;
	InitStack_Op(form);
	char ifFoEo = '\0';//存储要检查的符号
	for (i = 0; i < strlen(str); i++) {
		//非数字非运算符非小数点
		isle = 0;
		if (str[i] >= '0' && str[i] <= '9' || str[i] == '.' || In(str[i], OP))
			isle = 1;
		if (!isle) return 1;
	}
	for (i = 0; i < strlen(str); i++) {//括号匹配
		if (str[i] == '(' || str[i] == '[') {
			Push_Op(form, str[i]);
		}
		else if (str[i] == ')' || str[i] == ']') {
			if (Pop_Op(form, ifFoEo) == 0 ||
				str[i] == ')' && ifFoEo != '(' ||
				str[i] == ']' && ifFoEo != '[') {
				return 1;
			}
		}
	}
	if (form.top != form.base) return 1;//如果前括号比后括号多
	i = 0;
	//括号匹配后
	while (i < strlen(str)) {//运算符不能连排
		flag = 0;
		if (In(str[i], OP) && In(str[i + 1], OP)) {
			//两个相连字符为运算符，必有且仅有一个为括号
			if (str[i] == ')' || str[i] == ']') {
				//如果第一个为括号，则一定为右括号且第二个一定不是括号
				if (str[i + 1] != '(' && str[i + 1] != '[') {
					i++; flag = 1;
				}
				else { return 1; }
			}
			else if (str[i] != '(' && str[i] != '[') {
				//如果第一个不是括号，第二个一定是括号，且是左括号
				if (str[i + 1] == '(' || str[i + 1] == '[') {
					i++; flag = 1;
				}
				else return 1;
			}
			else return 1;
		}
		if (!flag) i++;
	}//可去标记
	return 0;
}// 1表示有误，0表示无误