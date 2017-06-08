/*************************************************************************
	>    File Name: Type.c
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年06月08日 星期四 21时28分09秒
 ************************************************************************/

#include<stdio.h>
typedef struct {
    int id;     //演出厅id
    char name[30];  //演出厅名称
    int rowsCount;  //座位行数
    int colsCount;  //座位列数
} *studio_t;

