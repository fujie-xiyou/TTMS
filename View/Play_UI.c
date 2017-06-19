/*
* File name:  Play_UI.c
* File ID:	  TTMS_SSF_Play_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
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
/*
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	//play_list_t head;
	//play_list_t pos;

	printf("\n============================================================\n");
	printf("*********************** ��Ŀ�б� **************************\n");
	printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s","���","����","����","����","����","ʱ��",
			"��ӳ����","��������","Ʊ��");
	printf("--------------------------------------------------------\n");

}
*/

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: ��Ŀ��Ϣ�������
 * Input:       flagΪ0���������ҳ�棬flagΪ1�����ѯ����
 * Output:      ��Ŀ�������
 * Return:      ��
 */
void Play_UI_MgtEntry(int flag){
	if(flag==1){
		printf("1");
	}
	play_list_t head;
	play_list_t pos;
	Pagination_t paging;
	int i,id;
	char choice;
	List_Init(head,play_node_t);
	paging.offset=0;
	paging.pageSize=PLAY_PAGE_SIZE;
	paging.totalRecords=Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head,paging);
	do{
		//system("cls");
		printf("\n============================================================\n");
		printf("*********************** ��Ŀ�б� **************************\n");
		printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s\n","���","����","����","����","����","ʱ��",
					"��ӳ����","��������","Ʊ��");
		printf("--------------------------------------------------------\n");
		for(i=0,pos=(play_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
			printf("%2d  %10s  %4d  %6s  %4d  %3d  %2d-%2d  %2d-%2d  %2d",pos->data.id
					,pos->data.name,pos->data.type,pos->data.area,pos->data.rating
					,pos->data.duration,pos->data.start_date.month,pos->data.start_date.day
					,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);
			pos = pos->next;
		}
		printf("------- ȫ����¼:%2d ----------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]��һҳ|[N]��һҳ | [A]���|[D]ɾ��|[U]�޸�|[Q]��ѯ|[R]����");
		printf("\n==================================================================\n");
		printf("����ѡ��:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
				case 'a':
				case 'A':
					if (Play_UI_Add()) //����ӳɹ����������һҳ��ʾ
					{
						paging.totalRecords = Play_Srv_FetchAll(head);
						Paging_Locate_LastPage(head, paging, play_node_t);
					}
					break;
				case 'd':
				case 'D':
					printf("����ID:");
					scanf("%d", &id);
					if (Play_UI_Delete(id)) {	//������������
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'u':
				case 'U':
					printf("����ID:");
					scanf("%d", &id);
					if (Play_UI_Modify(id)) {	//������������
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'q':
				case 'Q':
					printf("����ID:");
					scanf("%d", &id);
					Play_UI_Query(id);
					//paging.totalRecords = Play_Srv_FetchAll(head);
					//List_Paging(head, paging, play_node_t);
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
	List_Destroy(head,play_node_t);
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
		printf("\t\t****************  ����¾�Ŀ ***************\n");
		printf("-------------------------------------------------------\n");
		printf("��Ŀ����:");
		fflush(stdin);
		fgets(rec.name,30,stdin);
		printf("��Ŀ����(1��ʾ��Ӱ,2���,3���ֻ�):");
		scanf("%d", &(rec.type));
		printf("��Դ����:");
		fgets(rec.area,9,stdin);
		printf("��Ŀ����(1��ʾ��ͯ,2������,3���ֻ�):");
		scanf("%d", &(rec.rating));
		printf("�ݳ�ʱ��(����):");
		scanf("%d", &(rec.duration));
		printf("��ӳ����:(yyyy/mm/dd):");
		scanf("%d/%d/%d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
		printf("��������:(yyyy/mm/dd):");
		scanf("%d/%d/%d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);
		printf("=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Play");
		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("�¾�Ŀ��ӳɹ�!\n");
		} else
			printf("�¾�Ŀ���ʧ��!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]��Ӹ���, [R]����:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
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
	//schedule__list_t sch;
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("�þ�Ŀ������!\n��[Enter]������!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("\t\t****************  �޸ľ�Ŀ  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("��Ŀ���:%d\n", rec.id);
	printf("��Ŀ����:[%s]",rec.name);
	fflush(stdin);
	fgets(rec.name,30,stdin);
	printf("��Ŀ����(1��ʾ��Ӱ,2���,3���ֻ�):[%d]",rec.type);
	scanf("%d", &(rec.type));
	printf("��Դ����:[%s]",rec.area);
	fgets(rec.area,9,stdin);
	printf("��Ŀ����(1��ʾ��ͯ,2������,3���ֻ�)[%d]:",rec.rating);
	scanf("%d", &(rec.rating));
	printf("�ݳ�ʱ��(����):[%d]",rec.duration);
	scanf("%d", &(rec.duration));
	printf("��ӳ����:(yyyy/mm/dd):[%4d/%2d/%2d]",rec.start_date.year,rec.start_date.month,rec.start_date.day);
	scanf("%d/%d/%d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
	printf("��������:(yyyy/mm/dd)::[%4d/%2d/%2d]",rec.end_date.year,rec.end_date.month,rec.end_date.day);
	scanf("%d/%d/%d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);
	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf("��Ŀ�����޸ĳɹ�!\n��[Enter]����!\n");
	} else
		printf("��Ŀ�����޸�ʧ��!\n��[Enter]����\n");
	getchar();
	return rtn;
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
	if(Play_Srv_DeleteByID(id)){
		printf("��Ŀɾ���ɹ�!\n");
		return 1;
	}else{
		printf("��Ŀɾ��ʧ��!\n");
		return 0;
	}

	if (1) {
		//��ɾ����Ŀʱ��ͬʱɾ���ݳ��ƻ�

		if(Schedule_Srv_DeleteByID(id))
			printf("�ݳ�����λɾ���ɹ�!\n");//The seats of the room deleted successfully
		printf(	"���ݳ���ɾ���ɹ�!\n��[Enter]������!\n");//The room deleted successfully!\nPress [Enter] key to return
		rtn = 1;
	} else {
		printf("���ݳ���������!\n��[Enter]������!\n");//The room does not exist!\nPress [Enter] key to return
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
    play_t buf;
    rtn=Play_Srv_FetchByID(id,&buf);
	printf("\n============================================================\n");
	printf("*********************** ��Ŀ��ѯ **************************\n");
	printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s","���","����","����","����","����","ʱ��",
				"��ӳ����","��������","Ʊ��");
	printf("%2d  %10s  %4d  %6s  %4d  %4d  %2d-%2d  %2d-%2d  %2d\n",buf.id
						,buf.name,buf.type,buf.area,buf.rating
						,buf.duration,buf.start_date.month,buf.start_date.day
						,buf.end_date.month,buf.end_date.day,buf.price);
	printf("��ѯ���,�밴[Enter]����\n");
	getchar();
	return rtn;
}
