/*
  * Account_UI.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#include "Account_UI.h"//
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include<windows.h>
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>//

//登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
int SysLogin() 
{
	char pwd[16];
	char name[18];
	Account_Srv_InitSys();
	int i=0;
	printf("**********输入用户名和密码***********\n");
	do
	{
		
		printf("用户名:");
		scanf("%s",name);
		printf("密码:");
		scanf("%s",pwd);
		if(!(Account_Srv_Vertify(name,pwd)))
		{
			i++;
		}
		else
		{
			return 1;
		}
	}while(i<3);


	return 0;

}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry() {
	account_list_t head;
	account_node_t * pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		/*system("cls");*/
		printf("\n==================================================================\n");
		printf("********************** 用户信息 **********************\n");
		printf("------------------------------------------------------------------\n");
		printf("用户名\t密码\n");
		//显示数据
		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("用户名:%s\t密码:%s\n",);//不会
			pos = pos->next;
		}

		printf(
				"******************************************************************\n");
		printf(
				"[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]eat | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Account_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Account_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, account_node_t);

}


//添加一个用户账号信息，如果账号名存在，提示出错信息
int Account_UI_Add(account_list_t list ) {
	account_t rec;
	int newRecCount = 0;
	char choice; 

		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("****************  添加新用户  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("用户名:");
		scanf("%s",rec.username);
		printf("类型:");
		scanf()//不会
		printf("密码:");
		scanf("%s",rec.password);
		printf("Id:");
		scanf("%d",rec.id) 
		

		if (!(Account_Srv_FindByName(list,rec.name))) {
			if(Account_Srv_Add(&rec))
			{
				newRecCount += 1;
			}	
			
		} else
		{ 
			printf("已存在!\n");
			return 0;
		}
		 
	

	return newRecCount;

}

//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list ,char usrName[]) {
		account_t rec;
		if ((Account_Srv_FindByUsrName(list,usrName))) {
				rec=list->data;
			printf("输入新密码:");
			scanf("%s",rec.password);
			if(Account_Srv_Modify(&rec))
			{
				return 1;
			 } 
				
			
		}
		 else
		{
			printf("不存在!\n");
			return 0;
		}
		//e不会 
}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t rec; 
		if ((Account_Srv_FindByUsrName(list,usrName))) {
			rec=list->data;

			
			if(Account_Srv_DeleteByID(rec.id))//错误，参数应该是id 
			{
				return 1;
			 } 	
		}
		 else
		{
			printf("不存在!\n");
			return 0;
		}
//e不会 
}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t rec; 
		if ((Account_Srv_FindByUsrName(list,usrName))) {
	
			rec=list->date;
			if(Account_Srv_DeleteByID(rec.id))//错误，参数应该是id 
			{
				return 1;
			 } 	
		}
		 else
		{
			printf("不存在!\n");
			return 0;
		}
//e不会


	return 1;
}

