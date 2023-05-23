// PathSearching.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define ARGC_CNT 3
#define BuildingNum 13
#define DC 100000//DC：不连接，用远超路径长度的值表示两点之间不连通

int ShortestPath(int so, int de)
{
    int i = 0, j = 0, v = 0, w = 0;
    int Distance[BuildingNum][BuildingNum] =
    {
        0,18,DC,DC,DC,DC,DC,DC,DC,DC,DC,22,DC,//北门
        18,0,19,DC,DC,DC,DC,DC,DC,27,DC,DC,DC,//饮水思源
        DC,19,0,23,DC,DC,DC,DC,DC,DC,DC,4,32,//腾飞塔
        DC,DC,23,0,15,DC,DC,DC,DC,4,4,DC,DC,//图书馆
        DC,DC,DC,15,0,8,21,DC,DC,DC,DC,DC,DC,//教学主楼
        DC,DC,DC,DC,8,0,DC,DC,14,DC,DC,DC,4,//宪梓堂
        DC,DC,DC,DC,21,DC,0,DC,30,DC,DC,DC,DC,//活动中心
        DC,DC,DC,DC,DC,DC,DC,0,20,DC,43,DC,DC,//西迁馆
        DC,DC,DC,DC,DC,14,30,20,0,DC,21,DC,DC,//南门
        DC,27,DC,4,DC,DC,DC,DC,DC,DC,DC,DC,DC,//传送门1
        DC,DC,DC,4,DC,DC,DC,43,21,DC,DC,DC,DC,//传送门2
        22,DC,4,DC,DC,DC,DC,DC,DC,DC,DC,DC,DC,//传送门3
        DC,DC,32,DC,DC,4,DC,DC,DC,DC,DC,DC,DC,//传送门4
    };//初始化直接可达的建筑之间的路径长度矩阵（带权邻接矩阵）
    int Len[BuildingNum] = { 0 };//存储从sourse出发到各点的最短路径长度
    //int Path[BuildingNum][BuildingNum] = { 0 };//若Path[v][w]为1，说明w是当前求得的so到v的最短路径上的点，若为0则不在
    int final[BuildingNum] = { 0 };//final=0表示未找到到该点的最短路径，为a表示是第a个找到最短路径的点
    int a = 1;
    for (v = 0; v < BuildingNum; v++)
    {
        Len[v] = Distance[so][v];
        if (Distance[so][v] < DC)
        {
            //Path[v][so] = 1;
            //Path[v][v] = 1;
        }
    }
    final[so] = a; a++;
    for (i = 0; i < BuildingNum; i++)
    {
        int min = DC;
        for (w = 0; w < BuildingNum; w++)
        {
            if (final[w] == 0)
            {
                if (Len[w] < min)
                {
                    v = w;
                    min = Len[w];
                }
            }
        }
        final[v] = a; a++;
        for (w = 0; w < BuildingNum; w++)
        {
            if (final[w] == 0 && (min + Distance[v][w] < Len[w]))
            {
                Len[w] = min + Distance[v][w];
                /*for (j = 0; j < BuildingNum; j++) { Path[w][j] = Path[v][j]; }
                Path[w][w] = 1;*/
            }
        }
    }
    int length = Len[de];
    //按顺序输出最短路径经过的建筑
    /*int k=1;
    const char* Building[BuildingNum] = {  "北门","饮水思源","腾飞塔","图书馆","教学主楼","宪梓堂","活动中心","西迁馆","南门","传送门1","传送门2","传送门3","传送门4"};
    for (k = 1; k <= BuildingNum+1; k++)
    {
        for (j = 0; j < BuildingNum; j++)
        {
            if (Path[de][j] == 1 && final[j] == k)
            {
                printf(Building[j]);
                printf(" ");
            }
        }
    }*/
    //按顺序输出最短路径经过的建筑
    return length;
}

int main(int argc, char* argv[])
{
    if (argc != ARGC_CNT)
    {
        printf("ERROR_01");
        return 0;
    }//根据程序需要判断参数数量是否正确;
    int i = 0, j = 0;
    int sourse = -1;
    int dest = -1;
    int Path[BuildingNum][BuildingNum] = { 0 };
    const char* Building[BuildingNum] = { "北门","饮水思源","腾飞塔","图书馆","教学主楼","宪梓堂","活动中心","西迁馆","南门","传送门1","传送门2","传送门3","传送门4",};
    for (i = 0; i < BuildingNum; i++)
    {
        if (strcmp(argv[1], Building[i]) == 0)
            sourse = i;
        if (strcmp(argv[2], Building[i]) == 0)
            dest = i;
    }
    if (sourse == -1 || dest == -1)
    {
        printf("ERROR_02");
        return 0;
    }//源点或终点不是地图上的建筑物时，输出错误提示ERROR_02
    else if (sourse == dest)
    {
        printf("%d", 0);
        return 0;
    }//源点与终点都是地图上的建筑物且相同时，直接输出路径长度0
    else
    {
        int len = ShortestPath(sourse, dest);
        printf("%d", len);
    }//源点与终点都是地图上的建筑物且不同时，求出最短路径长度并输出
    return 0;
}//PathSearching