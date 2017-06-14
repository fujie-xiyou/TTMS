/*
 * Studio_UI.c
 *
 *  Created on: 2015��4��24��
 *      Author: Administrator
 */
#include "Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"
#define ACCOUNT_PAGE_SIZE 5 


static const int STUDIO_PAGE_SIZE = 5;// // 

#include <stdio.h>

void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		/*system("cls");*/
		printf("\n==================================================================\n");
		printf("********************** �ݳ����б� **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "���", "�ݳ�����", "����",
				"����", "��λ��");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("------- ȫ����¼:%2d ----------------------- ҳ�� %2d/%2d ----\n",//Total Records,page
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ|[N]��һҳ | [A]���|[D]ɾ��|[U]�޸� | [S]��λ | [R]����");
		printf(
				"\n==================================================================\n");
		printf("����ѡ��:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("����ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("����ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("����ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("****************  ������ݳ��� ****************\n");// Add New Projection Room 
		printf("-------------------------------------------------------\n");
		printf("�ݳ�������:");//Room Name
		fflush(stdin);
		gets(rec.name);
		printf("��λ������:");//Row Count of Seats
		scanf("%d", &(rec.rowsCount));
		printf("��λ������:");//Column Count of Seats
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = 0;
		printf("=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Studio");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("���ݳ�����ӳɹ�!\n");//The new room added successfully	
		} else
			printf("���ݳ������ʧ��!\n");//The new room added failed!
		printf("-------------------------------------------------------\n");
		printf("[A]��Ӹ���, [R]����:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("���ݳ���������!\n��[Enter]������!\n");//The room does not exist!\nPress [Enter] key to return!\n
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  �����ݳ���  ****************\n");//Update Projection Room
	printf("-------------------------------------------------------\n");
	printf("�ݳ������:%d\n", rec.id);
	printf("�ݳ�����[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷����������
			printf("��λ������Ӧ�� >= [%d]:", rec.rowsCount);//Row Count of Seats should
			scanf("%d", &(newrow));
			printf("��λ������Ӧ�� >= [%d]:", rec.colsCount);//Column Count of Seats should
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("��λ������:");//Row Count of Seats
		scanf("%d", &rec.rowsCount);
		printf("��λ������:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"�ݳ������ݸ��³ɹ�!\n��[Enter]����!\n");//The room data updated successfully
	} else
		printf("�ݳ������ݸ���ʧ��!\n��[Enter]����\n");//The room data updated failed

	getchar();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("�ݳ�����λɾ���ɹ�!\n");//The seats of the room deleted successfully
		printf(
				"���ݳ���ɾ���ɹ�!\n��[Enter]������!\n");//The room deleted successfully!\nPress [Enter] key to return
		rtn = 1;
	} else {
		printf("���ݳ���������!\n��[Enter]������!\n");//The room does not exist!\nPress [Enter] key to return
	}

	getchar();
	return rtn;
}

