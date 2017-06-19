/*
 * account.c
 *
 *  Created on: 2015年6月12日
 *      Author: Administrator
 */
#include "Account.h"
#include "EntityKey.h"
#include "../Common/List.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>

extern account_t gl_CurUser;

//创建系统初始化账号admin
void Account_Srv_InitSys(){
    if(Account_Perst_CheckAccFile("Account.dat")) return;
    account_t data_admin;
    data_admin.id=EntKey_Srv_CompNewKey("Account");
    data_admin.type=9;
    strcpy(data_admin.username,"admin");
    strcpy(data_admin.password,"admin");
    Account_Srv_Add(&data_admin);
}

//验证登录账号是否已存在，存在，保存登录用户信息到全局变量gl_CurUser，return 1；否则return 0
inline int Account_Srv_Verify(char usrName[], char pwd[]){
    account_t usr;
    if(!Account_Perst_SelByName(usrName,&usr)){
        return 0;
    }
    if(strcmp(usr.password,pwd)){
       return 0; 
    }
    gl_CurUser=usr;
    return 1;

}


//验证用户名是否存在，遍历list，若存在，返回对应结点指针；否则，返回null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {
    account_list_t pos;
    List_ForEach(list,pos){
        if(!strcmp(pos->data.username,usrName)){
            return pos;
        }
    }
	return NULL ;
}

//添加一个用户账号，通过调用Account_Perst_Insert(data)函数实现
inline int Account_Srv_Add(const account_t *data){

	return Account_Perst_Insert(data);
}

//修改一个用户账号，通过调用Account_Perst_Update(data)函数实现
inline int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update(data);
}

//删除一个用户账号，通过调用Account_Perst_DeleteByID(usrID)函数实现
inline int Account_Srv_DeleteByID(int usrID){
	return Account_Perst_DeleteByID(usrID);
}

//提取usrID对应的用户账号信息，通过调用Account_Perst_SelectByID(usrID, buf)函数实现
inline int Account_Srv_FetchByID(int usrID, account_t *buf){
	return Account_Perst_SelectByID(usrID,buf);
}

//提取usrName对应的用户账号信息，通过调用Account_Perst_SelByName(usrName, buf)函数实现
inline int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName,buf);
}

//提取所有用户账号信息，保存到list链表中，通过调用Account_Perst_SelectAll(list)函数实现
inline int Account_Srv_FetchAll(account_list_t list){
	return Account_Perst_SelectAll(list);
}


