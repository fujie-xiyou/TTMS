/*
* File name:  Play_UI.c
* File ID:	  TTMS_SSF_Play_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/list.h"
#include "../Service/play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>


	/*
	 * Function:    Play_UI_ShowList
	 * Function ID:	TTMS_SCU_Play_UI_Show
	 * Description: ���б�ģʽ��ʾ��Ŀ��Ϣ
	 * Input:       list��Ŀ��Ϣ�����ͷָ�룬paging��ҳ���ò���
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ���ز��ҵ��ļ�¼��Ŀ
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	int i;
	play_node_t *pos;

	List_Init(list, studio_node_t);
	paging.offset = 0;
	paging.pageSize = ;//��֪��

	//��������
	paging.totalRecords = Play_Srv_FetchAll(play);
	Paging_Locate_FirstPage(play, paging); 
	printf("***************��Ŀ��Ϣ********************\n");
	printf("ID\t����\t����\t��Ʒ����\t�ȼ�\tʱ��\t��ʼʱ��\t����ʱ��\tƱ��\n");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (play_node_t *) (paging.curPos);
				pos != list && i < paging.pageSize; i++) {
			printf("ID:%d\t",rec.id);
		printf("��Ŀ����:%s\t",rec.name);
		fflush(stdin);
		printf("\n");
		printf("��Ŀ����:");//���� 
		printf("��Ʒ����:%s\t", rec.area);
		printf("��Ŀ�ȼ���");//����
		printf("ʱ��:%d\t",rec.duration);
		printf("��ʼ���ڣ�");//���� 
		printf("�������ڣ�");//����
		printf("Ʊ��:%d\t",rec.price);
			pos = pos->next;
		}
		printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		
		
	//�ͷ�����ռ�
	List_Destroy(list, studio_node_t);

}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: ��Ŀ��Ϣ�������
 * Input:       flagΪ0���������ҳ�棬flagΪ1�����ѯ����
 * Output:      ��Ŀ�������
 * Return:      ��
 */
void Play_UI_MgtEntry(int flag){
	int i, id;
	char choice;

	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.pageSize = 0;///��֪���õ��ڼ� 

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	do {
		
		
		printf(
				"******************************************************************\n");
		printf(
				"[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [S]howlist | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Play_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, play_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (play_UI_Delete(id)) {	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 's':
		case 'S'://////������ 
			Play_UI_ShoeList(play_list_t list,Pagination_t paging) 
			paging.totalRecords = Play_Srv_FetchAll(head);
			List_Paging(head, paging, play_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, play_node_t);


}


/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: ���һ����Ŀ��Ϣ
 * Input:       ��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Play_UI_Add(void)
{
	play_t rec;
	int newRecCount = 0;
	char choice;

	do {
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("****************  ����µľ�Ŀ ****************\n");
		printf("-------------------------------------------------------\n");
		printf("��Ŀ����:");
		scanf("%s",rec.name);
		printf("��Ŀ����:");
		//buhui
		printf("��Ʒ������%d");
		scanf("%d\n", rec.area);
		printf("��Ŀ�ȼ���");
		//����
		printf("ʱ��:");
		scanf("%d\n",rec.duration);
		printf("��ʼ���ڣ�");
		//���� 
		printf("�������ڣ�");
		//����
		printf("Ʊ�ۣ�");
		scanf("%d\n",rec.price);
		 
		printf("=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&rec)) {
			newRecCoun+=1;
			printf("��ӳɹ�!\n");
		} else
			printf("���ʧ��!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	
	return newRecCoun;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: ���¾�Ŀ��Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_UI_Modify(int id){
	play_t rec;
	int rtn = 0;

	/*Load record*/
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("not exit!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}
	else
	{
		printf("\n=======================================================\n");
		printf("****************  ԭ����Ŀ��Ϣ ****************\n");
		printf("-------------------------------------------------------\n");
		printf("ID\t����\t����\t��Ʒ����\t�ȼ�\tʱ��\t��ʼʱ��\t����ʱ��\tƱ��\n");
		printf("ID:%d\t",rec.id);
		printf("��Ŀ����:%s\t",rec.name);
		fflush(stdin);
		printf("\n");
		printf("��Ŀ����:");//���� 
		printf("��Ʒ����:%s\t", rec.area);
		printf("��Ŀ�ȼ���");//����
		printf("ʱ��:%d\t",rec.duration);
		printf("��ʼ���ڣ�");//���� 
		printf("�������ڣ�");//����
		printf("Ʊ��:%d\t",rec.price);
		 
		printf("=======================================================\n");
		printf("****************  �����µľ�Ŀ��Ϣ ****************\n");
		printf("-------------------------------------------------------\n");
		printf("��Ŀ����:");
		scanf("%s",rec.name);
		printf("��Ŀ����:");
		//buhui
		printf("��Ʒ������%d");
		scanf("%d\n", rec.area);
		printf("��Ŀ�ȼ���");
		//����
		printf("ʱ��:");
		scanf("%d\n",rec.duration);
		printf("��ʼ���ڣ�");
		//���� 
		printf("�������ڣ�");
		//����
		printf("Ʊ�ۣ�");
		scanf("%d\n",rec.price);
		 
		printf("=======================================================\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"�޸ĳɹ�!\nPress [Enter] key to return!\n");
	} else
		printf("�޸�ʧ��!\nPress [Enter] key to return!\n");

	getchar();
	return rtn;
	return 0;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: ����ID��ɾ����Ŀ��Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_UI_Delete(int id){
	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("ɾ���ɹ�!\n");
		rtn = 1;
	} else {
		printf("ɾ��ʧ��!\nPress [Enter] key to return!\n");
	}

	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: ����ID�Ų�ѯ��Ŀ��Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_UI_Query(int id){
	int rtn=0;
	if(Play_Srv_FetchByID��id��)
	{
		printf("�ɹ�!\n");
		rtn=1;
	}
	else
	{
		printf("ʧ�ܣ�");
	}

	return rtn;
}