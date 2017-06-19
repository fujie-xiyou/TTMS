/*
  * Account_UI.c
 *
 *  Created on: 2015��5��8��
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

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() {
    Account_Srv_InitSys();
    int i=0;
    printf("==========��ӭʹ��HLW��ԺƱ�����ϵͳ!==========\n\n\n\t\t�����ȵ�¼!");
    char usrName[30];
    char passwd[30];
    while(i<3){
        printf("�������˺�:");
        scanf("%s",usrName);
        printf("����������:");
        scanf("%s",passwd);
        if(Account_Srv_Verify(usrName,passwd)){
            return 1;
        }
        printf("�˺Ż��������!����������\n");

    }
    printf("������󳬹�3��!\n");


	return 0;

}

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
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
        printf("************************�û��б�************************\n");
        printf("%5s  %10s  %10s  %10s\n","���","�û���","����","ְλ");
        printf("--------------------------------------------------------\n");
        for(i=0,pos=(account_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
            printf("%5d  %10s  %10s  %10d\n",pos->data.id,pos->data.username,pos->data.password,pos->data.type);
        }
	printf("--------------------------------------------------------\n");
        printf("\n[A]����|[M]�޸�|[D]ɾ��|[Q]��ѯ|[P]��һҳ|[N]��һҳ|[R]����\n");
        printf("�����빦��ѡ��:");
        scanf("%c",&choice);
        switch(choice){
            case 'A':
            case 'a':
            Account_UI_Add(head);
                break;
            case 'M':
            case 'm':
                printf("������Ҫ�޸�������û���:");
                fgets(name,30,stdin);
                Account_UI_Modify(head,name);
                break;
            case 'D':
            case 'd':
                printf("������Ҫɾ�����û���:");
                fgets(name,30,stdin);
                Account_UI_Delete(head,name);
                break;
            case 'Q':
            case 'q':
                printf("������Ҫ��ѯ���û���:");
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


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {
    account_t newUser;
    printf("\n===================������û�=====================\n");
    printf("---------------------------------------------------\n");
    printf("������Ҫ��ӵ��û���:");
    fgets(newUser.username,30,stdin);
    if(Account_Srv_FindByUsrName(list,newUser.username)){
        printf("���û��Ѵ���!!!\n");
        return 0;
    }
    newUser.id=EntKey_Srv_CompNewKey("Account");
    printf("���������û�������:");
    fgets(newUser.password,30,stdin);
    printf("���������û���ɫ(0��ʾ����,1ƱԱ,2����,9ϵͳ����Ա):");
    scanf("%d",&newUser.type);
    if(Account_Srv_Add(&newUser)){
        printf("���û���ӳɹ�!\n");
        return 1;
    }
    else{
        printf("���û����ʧ��!\n");
        return 0;
    }
	return 0;

}


//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) {
	account_list_t  user;
    if(!(user=Account_Srv_FindByUsrName(list,usrName))){
        printf("������û���������!\n");
        return 0;
    }
    printf("�������µ�����:");
    fgets(user->data.password,30,stdin);
    if(Account_Srv_Modify(&user->data)){
    	return 1;
    }
    return 0;
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_list_t user;
	if(!(user=Account_Srv_FindByUsrName(list,usrName))){
		printf("������û���������!\n");
		return 0;
	}
	if(Account_Srv_DeleteByID(user->data.id)){
		printf("ɾ���ɹ�!\n");
		return 0;
	}else{
		printf("ɾ��ʧ��!\n");
		return 1;
	}
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_list_t user;
	printf("\n===============��ѯ�û���Ϣ================\n");
	if((!(user=Account_Srv_FindByUsrName(list,usrName)))){
		printf("�û���������!\n");
		return 0;
	}else{
		printf("************************�û���Ϣ************************\n");
		printf("%5s  %10s  %10s  %10s\n","���","�û���","����","ְλ");
		printf("--------------------------------------------------------\n");
		printf("%5d  %10s  %10s  %10d\n",user->data.id,user->data.username,user->data.password,user->data.type);

	}

	return 1;
}


