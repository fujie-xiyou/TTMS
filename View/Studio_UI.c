/*
 * Studio_UI.c
 *
 *  Created on: 2015年4月24日
 *      Author: Administrator
 */
#include "Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"
#include "../Common/Common.h"

#define ACCOUNT_PAGE_SIZE 5 


static const int STUDIO_PAGE_SIZE = 5;

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

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("clear");
		printf("\n==================================================================\n");
		printf("********************** 演出厅列表 **********************\n");
		printf("%5s  %-18s  %-10s  %-10s  %-10s\n", "id", "演出厅名", "行数",
				"列数", "座位数");
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %-18s  %-10d  %-10d  %-10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("\n------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页|[N]下一页 | [A]添加|[D]删除|[U]修改 | [S]座位 | [R]返回");
		printf(
				"\n==================================================================\n");
		printf("功能选择:");//Your Choice
		//ffflush();
		scanf("%c", &choice);
		ffflush();

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("输入ID:");
			scanf("%d", &id);
			ffflush();
			if (Studio_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("输入ID:");
			scanf("%d", &id);
			ffflush();
			if (Studio_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("输入ID:");
			scanf("%d", &id);
			ffflush();
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
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("clear");
		printf("\n=======================================================\n");
		printf("****************  添加新演出厅 ****************\n");// Add New Projection Room 
		printf("-------------------------------------------------------\n");
		printf("演出厅名称:");//Room Name
		//ffflush();
		sgets(rec.name,30);
		//ffflush();
		printf("座位的排数:");//Row Count of Seats
		scanf("%d", &(rec.rowsCount));
		ffflush();
		printf("座位的列数:");//Column Count of Seats
		scanf("%d", &(rec.colsCount));
		ffflush();
		rec.seatsCount = 0;
		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Studio");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("新演出厅添加成功!\n");//The new room added successfully	
		} else
			printf("新演出厅添加失败!\n");//The new room added failed!
		printf("-------------------------------------------------------\n");
		printf("[A]添加更多, [R]返回:");
		scanf("%c", &choice);
		ffflush();
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
		printf("该演出厅不存在!\n按[Enter]键返回!\n");
		//The room does not exist!\nPress [Enter] key to return!\n
		getchar();
		return 0;
	}
	system("clear");
	printf("\n=======================================================\n");
	printf("****************  更新演出厅  ****************\n");//Update Projection Room
	printf("-------------------------------------------------------\n");
	printf("演出厅编号:%d\n", rec.id);
	printf("演出厅名[%s]:", rec.name);
	sgets(rec.name,30);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("座位的排数应该 >= [%d]:", rec.rowsCount);//Row Count of Seats should
			scanf("%d", &(newrow));
			ffflush();
			printf("座位的列数应该 >= [%d]:", rec.colsCount);//Column Count of Seats should
			scanf("%d", &(newcolumn));
			ffflush();
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("座位的排数:");//Row Count of Seats
		scanf("%d", &rec.rowsCount);
		ffflush();
		printf("座位的列数:");
		scanf("%d", &rec.colsCount);
		ffflush();
		rec.seatsCount=rec.colsCount*rec.rowsCount;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"演出厅数据更新成功!\n按[Enter]返回!\n");//The room data updated successfully
	} else
		printf("演出厅数据更新失败!\n按[Enter]返回\n");//The room data updated failed

	getchar();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("演出厅座位删除成功!\n");//The seats of the room deleted successfully
		printf(
				"该演出厅删除成功!\n按[Enter]键返回!\n");//The room deleted successfully!\nPress [Enter] key to return
		rtn = 1;
	} else {
		printf("该演出厅不存在!\n按[Enter]键返回!\n");//The room does not exist!\nPress [Enter] key to return
	}

	getchar();
	return rtn;
}

