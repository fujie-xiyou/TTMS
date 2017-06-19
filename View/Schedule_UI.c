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
	 * Description: 显示所有的演出计划信息,进行查询。
	 * Input:       无
	 * Output:      所有剧目的信息
	 * Return:      无
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
		printf("*********************** 演出计划查询 **************************\n");
		printf("%2s  %10s  %10s  %11s  %3s\n","编号","剧名","演出厅名","演出时间","票价");
		printf("--------------------------------------------------------------");
		for(i=0,pos=(schedule_node_t*)(paging.curPos);pos!=sch && i<paging.pageSize;i++){
		    Play_Srv_FetchByID(pos->data.play_id,&pla);
		    Studio_Srv_FetchByID(pos->data.studio_id,&stu);
		    printf("%2d  %10s  %10s  %2d/%2d %2d:%2d   %3d\n",pos->data.id,pla.name,stu.name,
		    		pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pla.price);
		    pos=pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页|[N]下一页|[R]返回");
		printf(
				"\n==================================================================\n");
		printf("功能选择:");//Your Choice
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
 * Description: 添加一条演出计划信息
 * Input:       相关剧目的ID号
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
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
		printf("\t\t****************  添加演出计划  *****************\n");
		printf("-------------------------------------------------------\n");
		rec.play_id=play_id;
		printf("放映厅编号:");
		scanf("%d",&rec.studio_id);
		printf("演出日期(yyyy/mm/dd):");
		scanf("%d/%d/%d",&rec.date.year,&rec.date.month,&rec.date.day);
		printf("演出时间(hh:mm:ss):");
		scanf("%d/%d/%d",&rec.time.hour,&rec.time.minute,&rec.time.second);
		rec.seat_count=Seat_Srv_FetchValidByRoomID(seatList,rec.studio_id);
		//获取主键
		rec.id=EntKey_Srv_CompNewKey("Schedule");
		if(Schedule_Srv_Add(&rec)){
			printf("添加演出计划成功!\n");
			newRecCount++;
		}
		else {
			printf("添加演出计划失败!\n");
		}
		printf("-------------------------------------------------------\n");
		printf("[A]添加更多, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	}while('a'==choice || 'A'==choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: 按照ID号更新演出计划信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Schedule_UI_Modify(int id){
	schedule_t rec;
	seat_list_t seatList;
	List_Init(seatList,seat_node_t);
	int rtn=0;
	if(!Schedule_Srv_FetchByID(id,&rec)){
		printf("该演出厅不存在!回车返回!\n");
		getchar();
		return 0;
	}
	printf("\n=======================================================\n");
	printf("\t\t**************** 修改演出计划 ****************\n");
	printf("-------------------------------------------------------\n");
	printf("演出计划编号: %d\n",rec.id);
	printf("演出的剧目编号:[%d]",rec.play_id);
	scanf("%d",&rec.play_id);
	printf("演出的播放厅编号:[%d]",rec.studio_id);
	printf("演出日期:[%4d/%2d/%2d]",rec.date.year,rec.date.month,rec.date.day);
	scanf("%d/%d/%d",&rec.date.year,&rec.date.month,&rec.date.day);
	printf("演出时间:[%2d/%2d/%2d]",rec.time.hour,rec.time.minute,rec.time.second);
	scanf("%d/%d/%d",&rec.time.hour,&rec.time.minute,&rec.time.second);
	rec.seat_count=Seat_Srv_FetchValidByRoomID(seatList,rec.studio_id);
	if(Schedule_Srv_Modify(&rec)){
		printf("修改演出计划成功!回车返回\n");
		rtn=1;
	}
	else{
		printf("修改演出计划失败!回车返回!\n");
	}
	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_UI_Delete(int id){
	int rtn=0;
	if(Schedule_Srv_DeleteByID(id)){
		printf("演出计划删除成功!\n");
		rtn = 1;
		//删除是成功时,同时删除对应的票
		if(Ticket_Srv_DeleteBatch(id)){
			printf("对应票删除成功!\n");
			rtn=2;
		}else{
			printf("对应票删除失败!\n");
			rtn=3;
		}
	}else{
		printf("演出计划删除失败!\n");
	}
	getchar();
	return rtn;;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: 按照ID号查询演出计划信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
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
	printf("*********************** 演出计划查询 **************************\n");
	printf("%2s  %10s  %10s  %11s  %3s\n","编号","剧名","演出厅名","演出时间","票价");
	printf("--------------------------------------------------------------");
	printf("%2d  %10s  %10s  %2d/%2d %2d:%2d   %3d\n",buf.id,pla.name,stu.name,
			buf.date.month,buf.date.day,buf.time.hour,buf.time.minute,pla.price);
	printf("查询完成,请按[Enter]返回\n");
	getchar();
	return rtn;
	return 0;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: 以列表模式显示给定剧目的演出计划信息
 * Input:       list为查找到的演出计划信息链表的头指针，play为剧目信息，paging为显示分页设置参数
 * Output:      无
 * Return:      返回查找到的记录数目
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
		printf("*********************** 演出计划查询 **************************\n");
		printf("%2s  %10s  %10s  %11s  %3s\n","编号","剧名","演出厅名","演出时间","票价");
		printf("--------------------------------------------------------------");
		for(i=0,pos=(schedule_node_t*)(paging.curPos);pos!=list && i<paging.pageSize;i++){
		    Play_Srv_FetchByID(pos->data.play_id,&pla);
		    Studio_Srv_FetchByID(pos->data.studio_id,&stu);
		    printf("%2d  %10s  %10s  %2d/%2d %2d:%2d   %3d\n",pos->data.id,pla.name,stu.name,
		    		pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pla.price);
		    pos=pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页|[N]下一页|[R]返回");
		printf(
				"\n==================================================================\n");
		printf("功能选择:");//Your Choice
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
 * Description: 演出计划管理界面，演出计划按照剧目进行管理，以剧目ID号为输入
 * Input:       相关剧目ID号
 * Output:      演出计划信息
 * Return:      无
 */
void Schedule_UI_MgtEntry(int play_id){
	char choice;
	printf("\n=======================================================\n");
	printf("\t\t**************** 管理演出计划 ****************\n");
	printf("-------------------------------------------------------\n");
	play_t ply;
	Play_Srv_FetchByID(play_id,&ply);
	printf("\t\t当前剧目: %s\n",ply.name);
	printf("\t请选择功能:\n");
	printf("\t\t[A]添加演出计划\n");
	printf("\t\t[D]删除演出计划\n");
	printf("\t\t[U]修改演出计划\n");
	printf("\t\t[Q]查询演出计划\n");
	printf("\t\t[N]列出当前剧目演出计划\n");
	printf("\t\t[L]列出所有演出计划\n");
	printf("\t\t[R]返回上级\n");
	printf("-------------------------------------------------------\n");
	printf("\t\t请输入功能选项:");
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
		printf("敬请期待!\n");
		break;
	case 'r':
	case 'R':
		break;
	}
}
