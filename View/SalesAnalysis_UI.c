/*
 * SalesAnalysis_UI.c
 *
 *  Created on: 2015��5��8��
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

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ
extern account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
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
		printf("********************** Ʊ�����а� **********************\n");
		printf("--------------------------------------------------------\n");
		printf("%2s %10s %4s %4s %4s %4s","���","����","����","����","Ʊ��","Ʊ��");
		for(i=0,pos=(salesanalysis_list_t)paging.curPos;pos!=head && i<paging.pageSize;i++){
			printf("%2d %10s %4s %4ld %4d %4ld",pos->data.play_id,pos->data.name,pos->data.area,pos->data.totaltickets
					,pos->data.price,pos->data.sales);
			pos=pos->next;
		}
		printf("------- ȫ����¼:%2d ----------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]��һҳ|[N]��һҳ|[R]����");
		printf("\n==================================================================\n");
		printf("����ѡ��:");//Your Choice
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

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void SalesAanalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){
	account_t user;
	int amount=0;
	amount=Salesanalysis_Srv_CompSaleVal(usrID,stDate,endDate);
	Account_Srv_FetchByID(usrID,&user);
	printf("��ƱԱ:%s �� %4d/%2d/%2d �� %4d/%2d/%2d �����۶�Ϊ%d\n",user.username,stDate.year
			,stDate.month,stDate.day,endDate.year,endDate.month,endDate.day,amount);
	printf("-------------------------------------------------------------\n");
}

//���۷�����ں�������ʾ�˵����˵�����"������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�"������ʾ���ѯ������ƱԱ��Ʊ�������
//����ѯ������������ĳ��ƱԱ��Ʊ�����
void SalesAanalysis_UI_MgtEntry() {
	char choice;
	user_date_t stDate,endDate;
	account_t user;
	do{
	printf("[B]������а�|[D]��ѯ��ƱԱ���������|[S]�жϵ�ǰ�û��Ƿ��Ǿ���|[R]����\n");
	printf("�������ѡ��:");
	scanf("%c",&choice);
	switch(choice)
	{
		case 'b':
		case 'B':
			if(gl_CurUser.type==USR_MANG){
				SalesAnalysis_UI_BoxOffice();
			}else{
				printf("�����Ǿ���,��Ȩ�鿴�˽���\n");
			}
		    break;
		case 'd':
		case 'D':
			printf("\n==============================================================\n");
			printf("*******************��ѯ��ƱԱ����ʱ������********************\n");
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
				printf("�����Ǿ���,��Ȩ�鿴��ҳ��!\n");
				break;
			}
			printf("\n==============================================================\n");
			printf("*******************��ѯ��ƱԱ�ڸ���ʱ������********************\n");
			printf("��������ƱԱ����:");
			fgets(user.username,30,stdin);
			if(Account_Srv_FetchByName(user.username,&user)){
				printf("�����뿪ʼʱ��:(yyyy/mm/dd)\n");
				scanf("%d/%d/%d",&stDate.year,&stDate.month,&stDate.day);
				printf("���������ʱ��:(yyyy/mm/dd)\n");
				scanf("%d/%d/%d",&endDate.year,&endDate.month,&endDate.day);
				SalesAanalysis_UI_StatSale(user.id,stDate,endDate);
			}else{
				printf("������û���������");
			}
			break;
		}
	}while('r'==choice  && 'R'==choice);
}



