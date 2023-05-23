#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define OK 1
#define OVERFLOW -2
#define ARGC_CNT 3

typedef int Status;

typedef struct {
	int weight;
	char data;
	int parent;
	int lchild;
	int rchild;
}HTNode, * HuffmanTree;

//定义存储字符及其对应编码的结构体
typedef struct {
	char data;
	char code[50];
}Code, * HuffmanCode;

//求字符串S长度
int LengthOf(char S[])
{
	int len = 0;
	for (len = 0; S[len] != '\0'; len++);
	return len;
}//LengthOf

//统计字符串S中出现的互不相同的字符数
int GetNum(char S[])
{
	int i = 0, j = 1, num = 1, flag = 0;//flag=0表示没有新的字符出现
	for (j = 1; S[j] != '\0'; j++)
	{
		flag = 1;
		for (i = 0; i < j; i++)
		{
			if (S[j] == S[i])
			{
				flag = 0;
			}
		}
		if (flag == 1)
		{
			num++;
		}
	}
	return num;
}//GetNum

//统计权重，并用HT数组返回字符与权重
void GetWeight(HTNode HT[], char S[], int m)
{
	int i = 1, j = 1, t = 1, flag = 1;
	for (i = 1; i <= m; i++)
	{
		HT[i] = { 0,NULL,0,0,0 };//初始化HT
	}
	for (j = 0; S[j] != '\0'; j++)//遍历S串
	{
		flag = 1;
		for (i = 1; i <= m; i++)//依次查询前方有无出现过该字符                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
		{
			if (HT[i].data == S[j])//若出现过，令flag=0
			{
				HT[i].weight++;
				flag = 0;
			}
		}
		if (flag == 1)//若没出现过，则添加进去
		{
			HT[t].data = S[j];
			HT[t].weight++;
			t++;
		}
	}
	for (i = 1; i < (m + 1) / 2; i++)
	{//将树中字符按照ASCII表排序，避免产生不同种的树,小字符在左
		for (j = 1; j < (m + 1) / 2 - i + 1; j++)
		{
			if (HT[j].data > HT[j + 1].data)
			{
				HT[0] = HT[j];//借用HT[0]作为交换媒介
				HT[j] = HT[j + 1];
				HT[j + 1] = HT[0];
			}
		}
	}
	HT[0] = { 0,NULL,0,0,0 };
}//GetWeight

struct min {
	int weight;//标记最小数值
	int number;//记录最小数值的序号
};//min

//选择两个权值最小的字符
void Select(HTNode HT[], int range, int& s1, int& s2)
{
	int i = 0;
	struct min min1, min2;
	min1.weight = INT_MAX;
	min2.weight = 1000;
	for (i = 1; i <= range; i++)
	{
		if (HT[i].parent == 0)
		{
			if (HT[i].weight < min1.weight)
			{
				min2 = min1;
				min1.weight = HT[i].weight;
				min1.number = i;
			}
			else if (HT[i].weight < min2.weight)
			{
				min2.weight = HT[i].weight;
				min2.number = i;
			}
		}//if
	}//for
	s1 = min1.number;
	s2 = min2.number;
}

//由编码规则进行编码并生成哈夫曼树
void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int n)
{
	int i = 0, m = 0, s1 = 0, s2 = 0;
	m = 2 * n - 1;
	for (i = n + 1; i <= m; i++)
	{
		Select(HT, i - 1, s1, s2);//在HT[1...i-1]选择parent为0且weight最小的两个结点
		HT[s1].parent = i;
		HT[s2].parent = i;
		if (HT[s1].weight == HT[s2].weight)
		{
			if (HT[s1].data > HT[s2].data)
			{
				HT[i].lchild = s2;
				HT[i].rchild = s1;
			}
			else
			{
				HT[i].lchild = s1;
				HT[i].rchild = s2;
			}
		}
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	char cd[50] = { 0 };
	int start = 0, c = 0, f = 0;
	cd[49] = '\0';
	for (i = 1; i <= n; i++)
	{
		start = 49;
		for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)
		{
			if (HT[f].lchild == c)
			{
				start--;
				cd[start] = '0';
			}
			else
			{
				start--;
				cd[start] = '1';
			}
		}//for
		strcpy_s(HC[i].code, &cd[start]);
		HC[i].data = HT[i].data;
	}
}

//先序非递归遍历哈夫曼树
Status PreTraverse(HuffmanTree HT, int n)
{
	int i = 0, m = 2 * n - 1, f = 0, level = 1, degree = 0;
	int* symbol;
	symbol = (int*)malloc((m + 1) * sizeof(int));
	if (!symbol)
	{
		return 0;
	}
	for (i = 0; i <= m; i++)
	{
		symbol[i] = 0;//0为还未遍历,1为遍历过左子树，2为遍历过右子树
	}
	f = m;//以f作为游标从根节点出发开始遍历
	while (f)
	{
		degree = 0;
		if (HT[f].lchild)
		{
			degree++;
		}
		if (HT[f].rchild)
		{
			degree++;
		}
		if (symbol[f] == 0)//向左
		{
			if (HT[f].data)
			{
				printf("%d %d %d %c\n", HT[f].weight, degree, level, HT[f].data);
			}
			symbol[f] = 1;
			if (HT[f].lchild)
			{
				f = HT[f].lchild;
				level++;
			}
		}
		else if (symbol[f] == 1)//向右
		{
			symbol[f] = 2;
			if (HT[f].rchild)
			{
				f = HT[f].rchild;
				level++;
			}
		}
		else//左右均遍历过，则回溯
		{
			f = HT[f].parent;
			level--;
		}
	}
	free(symbol);
	return OK;
}//PreTraverse

//译码
Status Translate(char Word[], HuffmanCode HC, char Result[], int n)
{//Word为输入的密码，HC存放编码结果，Result存放译码结果，n为叶子节点数
	int i = 0, j = 0, t = 0, k = 0, mark = 0, flag = 0;
	while (Word[t + 1] != '\0')
	{
		for (flag = 0, i = 1; i <= n; i++)
		{
			t = mark;
			for (j = 0; HC[i].code[j] == Word[t]; j++, t++)
			{
				if (HC[i].code[j + 1] == '\0')
				{
					Result[k] = HC[i].data;
					mark = t + 1;
					k++;
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			break;
		}
	}
	Result[k] = '\0';
	if (flag == 1) return OK;//flag=1标志两者比较到末尾而结束，译码成功
	else return 0;//flag=0标志又进入了新的循环，则没有比较到末尾，译码失败
}//Translate

//打印编码结果
void PrintCodeResult(HuffmanCode HC, char S[], int n)
{
	int i = 0, j = 0;
	for (i = 0; S[i] != '\0'; i++)
	{
		for (j = 1; j <= n; j++)
		{
			if (HC[j].data == S[i])
			{
				printf("%s", HC[j].code);
			}
		}
	}
}//PrintCodeResult

int main(int argc, char* argv[])
{
	if (argc != ARGC_CNT)
	{
		printf("ERROR_01");
		return 0;
	}//若命令行参数输入错误，则结束运行并报错，错误类型ERROR_01
	if (LengthOf(argv[1]) < 20)
	{
		printf("ERROR_02");
		return 0;
	}//若输入字符串长度小于20，则结束运行并报错，错误类型ERROR_02
	int m = 0, n = 0;//m为节点数，n为叶子结点数
	HTNode* HT;//定义哈夫曼树
	HuffmanCode HC;//定义HC用于存放每个字符的编码值
	char result[20];//存放最终译码结果
	n = GetNum(argv[1]);//先算出有多少叶子节点，以便地址开辟
	if (n == 1)
	{
		printf("ERROR_02");
		return 0;
	}//若输入字符串仅有单一字符，则结束运行并报错，错误类型ERROR_02
	m = 2 * n - 1;
	HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
	if (!HT)
	{
		return OVERFLOW;
	}
	GetWeight(HT, argv[1], m);
	HC = (HuffmanCode)malloc((n + 1) * sizeof(Code));
	if (!HC)
	{
		return OVERFLOW;
	}
	HuffmanCoding(HT, HC, n);//编码内容到此结束
	//PreTraverse(HT, n);//先序遍历HT，并输出每个节点的角标，权重，层数，度
	if (!Translate(argv[2], HC, result, n))
	{
		printf("ERROR_03");
		return 0;
	}//若译码失败，则结束运行并报错，错误类型ERROR_03
	PrintCodeResult(HC, argv[1], n);//打印编码结果
	printf(" %s", result);//打印译码结果
	free(HT); 
	free(HC);//释放malloc分配的地址
	return 0;
}