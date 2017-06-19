/*
  * Account_UI.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#include "Account_UI.h"//
#include "../Common/List.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
//#include<windows.h>
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>

//登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
int SysLogin() {
    Account_Srv_InitSys();
    int i=0;
    printf("==========欢迎使用HLW剧院票务管理系统!==========\n\n\n\t\t请您先登录!");
    char usrName[30];
    char passwd[30];
    while(i<3){
        printf("请输入账号:");
        scanf("%s",usrName);
        printf("请输入密码:");
        scanf("%s",passwd);
        if(Account_Srv_Verify(usrName,passwd)){
            return 1;
        }
        printf("账号或密码错误!请重新输入\n");

    }
    printf("密码错误超过3次!\n");


	return 0;

}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry() {
    account_list_t head;
    account_list_t pos;
    Pagination_t paging;
    int i;
    char choice,name[30];
    List_Init(head,account_node_t);
    paging.offset=0;
    paging.pageSize=ACCOUNT_PAGE_SIZE;
    paging.totalRecords = Account_Srv_FetchAll(head);
    Paging_Locate_FirstPage(head,paging);
    do{
        //system("cls");
        printf("\n========================================================\n");
        printf("************************用户列表************************\n");
        printf("%5s  %10s  %10s  %10s\n","编号","用户名","密码","职位");
        printf("--------------------------------------------------------\n");
        for(i=0,pos=(account_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
            printf("%5d  %10s  %10s  %10d\n",pos->data.id,pos->data.username,pos->data.password,pos->data.type);
        }
	printf("--------------------------------------------------------\n");
        printf("\n[A]新增|[M]修改|[D]删除|[Q]查询|[P]上一页|[N]下一页|[R]返回\n");
        printf("请输入功能选项:");
        scanf("%c",&choice);
        switch(choice){
            case 'A':
            case 'a':
            Account_UI_Add(head);
                break;
            case 'M':
            case 'm':
                printf("请输入要修改密码的用户名:");
                fgets(name,30,stdin);
                Account_UI_Modify(head,name);
                break;
            case 'D':
            case 'd':
                printf("请输入要删除的用户名:");
                fgets(name,30,stdin);
                Account_UI_Delete(head,name);
                break;
            case 'Q':
            case 'q':
                printf("请输入要查询的用户名:");
                fgets(name,30,stdin);
                Account_UI_Query(head,name);
                break;
            case 'P':
            case 'p':
    			if (1 < Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(head,paging,-1,account_node_t);
                }
                break;
            case 'N':
            case 'n':
    			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(head,paging,1,account_node_t);
                }
                break;
        }


    }while(choice!='R' && choice!='r');
    List_Destroy(head,account_node_t);

}


//添加一个用户账号信息，如果账号名存在，提示出错信息
int Account_UI_Add(account_list_t list ) {
    account_t newUser;
    printf("\n===================添加新用户=====================\n");
    printf("---------------------------------------------------\n");
    printf("请输入要添加的用户名:");
    fgets(newUser.username,30,stdin);
    if(Account_Srv_FindByUsrName(list,newUser.username)){
        printf("该用户已存在!!!\n");
        return 0;
    }
    newUser.id=EntKey_Srv_CompNewKey("Account");
    printf("请输入新用户的密码:");
    fgets(newUser.password,30,stdin);
    printf("请输入新用户角色(0表示匿名,1票员,2经理,9系统管理员):");
    scanf("%d",&newUser.type);
    if(Account_Srv_Add(&newUser)){
        printf("新用户添加成功!\n");
        return 1;
    }
    else{
        printf("新用户添加失败!\n");
        return 0;
    }
	return 0;

}


//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list ,char usrName[]) {
	account_list_t  user;
    if(!(user=Account_Srv_FindByUsrName(list,usrName))){
        printf("输入的用户名不存在!\n");
        return 0;
    }
    printf("请输入新的密码:");
    fgets(user->data.password,30,stdin);
    if(Account_Srv_Modify(&user->data)){
    	return 1;
    }
    return 0;
}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_list_t user;
	if(!(user=Account_Srv_FindByUsrName(list,usrName))){
		printf("输入的用户名不存在!\n");
		return 0;
	}
	if(Account_Srv_DeleteByID(user->data.id)){
		printf("删除成功!\n");
		return 0;
	}else{
		printf("删除失败!\n");
		return 1;
	}
}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_list_t user;
	printf("\n===============查询用户信息================\n");
	if((!(user=Account_Srv_FindByUsrName(list,usrName)))){
		printf("用户名不存在!\n");
		return 0;
	}else{
		printf("************************用户信息************************\n");
		printf("%5s  %10s  %10s  %10s\n","编号","用户名","密码","职位");
		printf("--------------------------------------------------------\n");
		printf("%5d  %10s  %10s  %10d\n",user->data.id,user->data.username,user->data.password,user->data.type);

	}

	return 1;
}


