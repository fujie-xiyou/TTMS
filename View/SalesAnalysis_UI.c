/*
 * SalesAnalysis_UI.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */
#include "SalesAnalysis_UI.h"
#include "../Common/List.h"
#include "../Common/Common.h"
#include "../Service/SalesAnalysis.h"
//#include <conio.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//外部变量gl_CurUser 用于存储登录用户信息
extern account_t gl_CurUser;

//定义分页机制中页面大小为5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//剧院销售排行榜函数,降序显示截止目前剧院电影票房排行榜
void SalesAnalysis_UI_BoxOffice() {
	salesanalysis_list_t head,pos;
	Pagination_t paging;
	int i;
	char choice;

	List_Init(head,salesanalysis_node_t);
	paging.pageSize=SALESANALYSIS_PAGE_SIZE;
	paging.totalRecords=Salesanalysis_Srv_StaticSale(head);
	Paging_Locate_FirstPage(head,paging);
	do{
		printf("\n==================================================================\n");
		printf("********************** 票房排行榜 **********************\n");
		printf("--------------------------------------------------------\n");
		printf("%2s %10s %4s %4s %4s %4s","编号","名称","区域","销量","票价","票房");
		for(i=0,pos=(salesanalysis_list_t)paging.curPos;pos!=head && i<paging.pageSize;i++){
			printf("%2d %10s %4s %4ld %4d %4ld",pos->data.play_id,pos->data.name,pos->data.area,pos->data.totaltickets
					,pos->data.price,pos->data.sales);
			pos=pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]上一页|[N]下一页|[R]返回");
		printf("\n==================================================================\n");
		printf("功能选择:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch(choice){
		case 'P':
		case 'p':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
			}
			break;
		}
	}while(choice != 'r' && choice != 'R');
	List_Destroy(head,salesanalysis_node_t);
}

//显示售票员在给定日期区间的售票情况
void SalesAanalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){
	account_t user;
	int amount=0;
	amount=Salesanalysis_Srv_CompSaleVal(usrID,stDate,endDate);
	Account_Srv_FetchByID(usrID,&user);
	printf("售票员:%s 在 %4d/%2d/%2d 到 %4d/%2d/%2d 间销售额为%d\n",user.username,stDate.year
			,stDate.month,stDate.day,endDate.year,endDate.month,endDate.day,amount);
	printf("-------------------------------------------------------------\n");
}

//销售分析入口函数，显示菜单，菜单包含"降序显示截止目前剧院电影票房排行榜"，“显示或查询当日售票员售票情况”，
//“查询给定日期区间某售票员售票情况”
void SalesAanalysis_UI_MgtEntry() {
	char choice;
	user_date_t stDate,endDate;
	account_t user;
	do{
	printf("[B]浏览排行榜|[D]查询售票员的销售情况|[S]判断当前用户是否不是经理|[R]结束\n");
	printf("输入你的选择:");
	scanf("%c",&choice);
	switch(choice)
	{
		case 'b':
		case 'B':
			if(gl_CurUser.type==USR_MANG){
				SalesAnalysis_UI_BoxOffice();
			}else{
				printf("您不是经理,无权查看此界面\n");
			}
		    break;
		case 'd':
		case 'D':
			printf("\n==============================================================\n");
			printf("*******************查询售票员当天时间销量********************\n");
			stDate=endDate=DateNow();
			if(gl_CurUser.type==USR_CLERK){
				SalesAanalysis_UI_StatSale(gl_CurUser.id,stDate,endDate);
			}else{
				fgets(user.username,30,stdin);
				if(Account_Srv_FetchByName(user.username,&user)){
					SalesAanalysis_UI_StatSale(user.id,stDate,endDate);
				}
			}
			break;
		case 's':
		case 'S':
			if(gl_CurUser.type!=USR_MANG){
				printf("您不是经理,无权查看此页面!\n");
				break;
			}
			printf("\n==============================================================\n");
			printf("*******************查询售票员在给定时间销量********************\n");
			printf("请输入售票员名字:");
			fgets(user.username,30,stdin);
			if(Account_Srv_FetchByName(user.username,&user)){
				printf("请输入开始时间:(yyyy/mm/dd)\n");
				scanf("%d/%d/%d",&stDate.year,&stDate.month,&stDate.day);
				printf("请输入结束时间:(yyyy/mm/dd)\n");
				scanf("%d/%d/%d",&endDate.year,&endDate.month,&endDate.day);
				SalesAanalysis_UI_StatSale(user.id,stDate,endDate);
			}else{
				printf("输入的用户名不存在");
			}
			break;
		}
	}while('r'==choice  && 'R'==choice);
}



