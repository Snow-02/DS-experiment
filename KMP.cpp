#include <iostream>
#include <vector>
#include <algorithm>

#define ARGC_CNT 3
struct HString {
    std::vector<char> ch;
    size_t length;
};

int StrAssign(HString& T, const char* chars) {
    // Clear the existing string data, if any
    T.ch.clear();
    T.length = 0;

    // Calculate the length of the chars string
    const size_t len = strlen(chars);

    if (len == 0) {
        // Set T to the empty string
        return 0;
    }

    // Allocate memory for the string data and copy the chars string into it
    T.ch.resize(len);
    std::copy(chars, chars + len, T.ch.data());
    T.length = len;
    return 0;
}

int StrLength(const HString& S) {
    return S.length;
}

int StrCompare(const HString& S, const HString& T) {
    const size_t min_len = std::min(S.length, T.length);
    for (size_t i = 0; i < min_len; ++i) {
        if (S.ch[i] != T.ch[i])
            return S.ch[i] - T.ch[i];
    }
    return S.length - T.length;
}

int ClearString(HString& S) {
    S.ch.clear();
    S.length = 0;
    return 0;
}

void get_next(const HString& T, std::vector<int>& next) {
    next.resize(T.length);
    next[0] = 0;
    for (size_t i = 1, j = 0; i < T.length;) {
        if (j == 0 || T.ch[i - 1] == T.ch[j - 1]) {
            i++;
            j++;
            next[i] = j;
        }
        else {
            j = next[j];
        }
    }
}

int Index_KMP(HString S, HString T, int pos) {//求T在S的第pos个字符后的位置
    int i = pos, j = 1;
    int* next = NULL;//给next空指针
    int next_len = T.length;
    next = (int*)malloc(next_len * sizeof(int));//按照T的长度初始化next的空间
    if (!next) {
        exit(OVERFLOW);
    }
    get_next(T, next);
    while (i <= S.length && j <= T.length) {
        if (j == 0 || S.ch[i - 1] == T.ch[j - 1]) { i++; j++; }
        else j = next[j];
    }
    if (j > T.length) return i - T.length;//查找到模式串，返回模式串位置
    else return -1;//未查找到模式串，返回-1
}//Index_KMP



int main(int argc, char* argv[])
{
    if (argc != ARGC_CNT)
    {
        printf("ERROR_01");
        return 0;
    }//判断命令行参数是否正确
    HString S = { 0 }; HString T = { 0 };//初始化主串及模式串
    StrAssign(S, argv[1]); 
    StrAssign(T, argv[2]);
    int position = Index_KMP(S, T, 1);//查找S在主串T中第一个字符之后的位置
    printf("%d", position);//输出位置
    ClearString(S); 
    ClearString(T);//释放内存空间
    return 0;
}//KMP