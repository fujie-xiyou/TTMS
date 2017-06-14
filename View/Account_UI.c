/*
  * Account_UI.c
 *
 *  Created on: 2015��5��8��
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

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() 
{
	char pwd[16];
	char name[18];
	Account_Srv_InitSys();
	int i=0;
	printf("**********�����û���������***********\n");
	do
	{
		
		printf("�û���:");
		scanf("%s",name);
		printf("����:");
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

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
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
		printf("********************** �û���Ϣ **********************\n");
		printf("------------------------------------------------------------------\n");
		printf("�û���\t����\n");
		//��ʾ����
		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("�û���:%s\t����:%s\n",);//����
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
			if (Account_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Account_UI_Delete(id)) {	//������������
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Account_UI_Modify(id)) {	//������������
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
	//�ͷ�����ռ�
	List_Destroy(head, account_node_t);

}


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {
	account_t rec;
	int newRecCount = 0;
	char choice; 

		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("****************  ������û�  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("�û���:");
		scanf("%s",rec.username);
		printf("����:");
		scanf()//����
		printf("����:");
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
			printf("�Ѵ���!\n");
			return 0;
		}
		 
	

	return newRecCount;

}

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) {
		account_t rec;
		if ((Account_Srv_FindByUsrName(list,usrName))) {
				rec=list->data;
			printf("����������:");
			scanf("%s",rec.password);
			if(Account_Srv_Modify(&rec))
			{
				return 1;
			 } 
				
			
		}
		 else
		{
			printf("������!\n");
			return 0;
		}
		//e���� 
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t rec; 
		if ((Account_Srv_FindByUsrName(list,usrName))) {
			rec=list->data;

			
			if(Account_Srv_DeleteByID(rec.id))//���󣬲���Ӧ����id 
			{
				return 1;
			 } 	
		}
		 else
		{
			printf("������!\n");
			return 0;
		}
//e���� 
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t rec; 
		if ((Account_Srv_FindByUsrName(list,usrName))) {
	
			rec=list->date;
			if(Account_Srv_DeleteByID(rec.id))//���󣬲���Ӧ����id 
			{
				return 1;
			 } 	
		}
		 else
		{
			printf("������!\n");
			return 0;
		}
//e����


	return 1;
}

