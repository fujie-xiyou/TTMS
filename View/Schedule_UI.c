/*
* File name:  Schedule_UI.c
* File ID:	  TTMS_SSF_Schedule_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Seat.h"
#include "../Service/Ticket.h"
#include <stdio.h>
#include <stdlib.h>

	/*
	 * Function:    Schedule_UI_ListAll
	 * Function ID:	TTMS_SCU_Schedule_UI_List
	 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
	 * Input:       ��
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ��
	 */
void Schedule_UI_ListAll(void) {
	schedule_list_t sch,pos;
	Pagination_t paging;
	play_t pla;
	studio_t stu;
	int i;
	char choice;
	List_Init(sch,schedule_node_t);

	paging.offset=0;
	paging.pageSize=SCHEDULE_PAGE_SIZE;
	paging.totalRecords=Schedule_Srv_FetchAll(sch);
	Paging_Locate_FirstPage(sch,paging);
	do {
		//system("cls");
		printf("\n============================================================\n");
		printf("*********************** �ݳ��ƻ���ѯ **************************\n");
		printf("%2s  %10s  %10s  %11s  %3s\n","���","����","�ݳ�����","�ݳ�ʱ��","Ʊ��");
		printf("--------------------------------------------------------------");
		for(i=0,pos=(schedule_node_t*)(paging.curPos);pos!=sch && i<paging.pageSize;i++){
		    Play_Srv_FetchByID(pos->data.play_id,&pla);
		    Studio_Srv_FetchByID(pos->data.studio_id,&stu);
		    printf("%2d  %10s  %10s  %2d/%2d %2d:%2d   %3d\n",pos->data.id,pla.name,stu.name,
		    		pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pla.price);
		    pos=pos->next;
		}
		printf("------- ȫ����¼:%2d ----------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ|[N]��һҳ|[R]����");
		printf(
				"\n==================================================================\n");
		printf("����ѡ��:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(sch, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(sch, paging, 1, schedule_node_t);
			}
			break;
		}
	}while('r'!=choice && 'R'!=choice);

}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(int play_id) {
	schedule_t rec;
	seat_list_t seatList;
	List_Init(seatList,seat_node_t);
	int newRecCount=0;
	char choice;
	do{
		//system("cls);
		printf("\n=======================================================\n");
		printf("\t\t****************  ����ݳ��ƻ�  *****************\n");
		printf("-------------------------------------------------------\n");
		rec.play_id=play_id;
		printf("��ӳ�����:");
		scanf("%d",&rec.studio_id);
		printf("�ݳ�����(yyyy/mm/dd):");
		scanf("%d/%d/%d",&rec.date.year,&rec.date.month,&rec.date.day);
		printf("�ݳ�ʱ��(hh:mm:ss):");
		scanf("%d/%d/%d",&rec.time.hour,&rec.time.minute,&rec.time.second);
		rec.seat_count=Seat_Srv_FetchValidByRoomID(seatList,rec.studio_id);
		//��ȡ����
		rec.id=EntKey_Srv_CompNewKey("Schedule");
		if(Schedule_Srv_Add(&rec)){
			printf("����ݳ��ƻ��ɹ�!\n");
			newRecCount++;
		}
		else {
			printf("����ݳ��ƻ�ʧ��!\n");
		}
		printf("-------------------------------------------------------\n");
		printf("[A]��Ӹ���, [R]����:");
		fflush(stdin);
		scanf("%c", &choice);
	}while('a'==choice || 'A'==choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID�Ÿ����ݳ��ƻ���Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id){
	schedule_t rec;
	seat_list_t seatList;
	List_Init(seatList,seat_node_t);
	int rtn=0;
	if(!Schedule_Srv_FetchByID(id,&rec)){
		printf("���ݳ���������!�س�����!\n");
		getchar();
		return 0;
	}
	printf("\n=======================================================\n");
	printf("\t\t**************** �޸��ݳ��ƻ� ****************\n");
	printf("-------------------------------------------------------\n");
	printf("�ݳ��ƻ����: %d\n",rec.id);
	printf("�ݳ��ľ�Ŀ���:[%d]",rec.play_id);
	scanf("%d",&rec.play_id);
	printf("�ݳ��Ĳ��������:[%d]",rec.studio_id);
	printf("�ݳ�����:[%4d/%2d/%2d]",rec.date.year,rec.date.month,rec.date.day);
	scanf("%d/%d/%d",&rec.date.year,&rec.date.month,&rec.date.day);
	printf("�ݳ�ʱ��:[%2d/%2d/%2d]",rec.time.hour,rec.time.minute,rec.time.second);
	scanf("%d/%d/%d",&rec.time.hour,&rec.time.minute,&rec.time.second);
	rec.seat_count=Seat_Srv_FetchValidByRoomID(seatList,rec.studio_id);
	if(Schedule_Srv_Modify(&rec)){
		printf("�޸��ݳ��ƻ��ɹ�!�س�����\n");
		rtn=1;
	}
	else{
		printf("�޸��ݳ��ƻ�ʧ��!�س�����!\n");
	}
	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){
	int rtn=0;
	if(Schedule_Srv_DeleteByID(id)){
		printf("�ݳ��ƻ�ɾ���ɹ�!\n");
		rtn = 1;
		//ɾ���ǳɹ�ʱ,ͬʱɾ����Ӧ��Ʊ
		if(Ticket_Srv_DeleteBatch(id)){
			printf("��ӦƱɾ���ɹ�!\n");
			rtn=2;
		}else{
			printf("��ӦƱɾ��ʧ��!\n");
			rtn=3;
		}
	}else{
		printf("�ݳ��ƻ�ɾ��ʧ��!\n");
	}
	getchar();
	return rtn;;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_UI_Query(int id){
    int rtn=0;
    schedule_t buf;
    play_t pla;
    studio_t stu;
    rtn=Play_Srv_FetchByID(id,&pla);
    Play_Srv_FetchByID(buf.play_id,&pla);
    Studio_Srv_FetchByID(buf.studio_id,&stu);
	printf("\n============================================================\n");
	printf("*********************** �ݳ��ƻ���ѯ **************************\n");
	printf("%2s  %10s  %10s  %11s  %3s\n","���","����","�ݳ�����","�ݳ�ʱ��","Ʊ��");
	printf("--------------------------------------------------------------");
	printf("%2d  %10s  %10s  %2d/%2d %2d:%2d   %3d\n",buf.id,pla.name,stu.name,
			buf.date.month,buf.date.day,buf.time.hour,buf.time.minute,pla.price);
	printf("��ѯ���,�밴[Enter]����\n");
	getchar();
	return rtn;
	return 0;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
	schedule_list_t pos;
	play_t pla;
	studio_t stu;
	int i;
	char choice;
	paging.totalRecords=Schedule_Srv_FetchByPlay(list,play->id);
	paging.offset=0;
	paging.pageSize=SCHEDULE_PAGE_SIZE;
	Paging_Locate_FirstPage(list,paging);
	do {
		//system("cls");
		printf("\n============================================================\n");
		printf("*********************** �ݳ��ƻ���ѯ **************************\n");
		printf("%2s  %10s  %10s  %11s  %3s\n","���","����","�ݳ�����","�ݳ�ʱ��","Ʊ��");
		printf("--------------------------------------------------------------");
		for(i=0,pos=(schedule_node_t*)(paging.curPos);pos!=list && i<paging.pageSize;i++){
		    Play_Srv_FetchByID(pos->data.play_id,&pla);
		    Studio_Srv_FetchByID(pos->data.studio_id,&stu);
		    printf("%2d  %10s  %10s  %2d/%2d %2d:%2d   %3d\n",pos->data.id,pla.name,stu.name,
		    		pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pla.price);
		    pos=pos->next;
		}
		printf("------- ȫ����¼:%2d ----------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ|[N]��һҳ|[R]����");
		printf(
				"\n==================================================================\n");
		printf("����ѡ��:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
			}
			break;
		}
	}while('r'!=choice && 'R'!=choice);

}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
 */
void Schedule_UI_MgtEntry(int play_id){
	char choice;
	printf("\n=======================================================\n");
	printf("\t\t**************** �����ݳ��ƻ� ****************\n");
	printf("-------------------------------------------------------\n");
	play_t ply;
	Play_Srv_FetchByID(play_id,&ply);
	printf("\t\t��ǰ��Ŀ: %s\n",ply.name);
	printf("\t��ѡ����:\n");
	printf("\t\t[A]����ݳ��ƻ�\n");
	printf("\t\t[D]ɾ���ݳ��ƻ�\n");
	printf("\t\t[U]�޸��ݳ��ƻ�\n");
	printf("\t\t[Q]��ѯ�ݳ��ƻ�\n");
	printf("\t\t[N]�г���ǰ��Ŀ�ݳ��ƻ�\n");
	printf("\t\t[L]�г������ݳ��ƻ�\n");
	printf("\t\t[R]�����ϼ�\n");
	printf("-------------------------------------------------------\n");
	printf("\t\t�����빦��ѡ��:");
	fflush(stdin);
	scanf("%c", &choice);
	fflush(stdin);

	switch (choice) {
	case 'a':
	case 'A':
		Schedule_UI_Add(play_id);
		break;
	case 'd':
	case 'D':
		Schedule_UI_Delete(play_id);
		break;
	case 'u':
	case 'U':
		Schedule_UI_Modify(play_id);
		break;
	case 'q':
	case 'Q':
		Schedule_UI_Query(play_id);
		break;
	case 'N':
	case 'n':
		//Schedule_UI_ListByPlay();
		printf("�����ڴ�!\n");
		break;
	case 'r':
	case 'R':
		break;
	}
}
