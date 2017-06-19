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
	 * Description: 以列表模式显示剧目信息
	 * Input:       list剧目信息链表的头指针，paging分页设置参数
	 * Output:      所有剧目的信息
	 * Return:      返回查找到的记录数目
	 */
/*
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	//play_list_t head;
	//play_list_t pos;

	printf("\n============================================================\n");
	printf("*********************** 剧目列表 **************************\n");
	printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s","编号","名称","类型","地区","级别","时长",
			"上映日期","结束日期","票价");
	printf("--------------------------------------------------------\n");

}
*/

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: 剧目信息管理界面
 * Input:       flag为0，进入管理页面，flag为1进入查询界面
 * Output:      剧目管理界面
 * Return:      无
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
		printf("*********************** 剧目列表 **************************\n");
		printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s\n","编号","名称","类型","地区","级别","时长",
					"上映日期","结束日期","票价");
		printf("--------------------------------------------------------\n");
		for(i=0,pos=(play_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
			printf("%2d  %10s  %4d  %6s  %4d  %3d  %2d-%2d  %2d-%2d  %2d",pos->data.id
					,pos->data.name,pos->data.type,pos->data.area,pos->data.rating
					,pos->data.duration,pos->data.start_date.month,pos->data.start_date.day
					,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);
			pos = pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[P]上一页|[N]下一页 | [A]添加|[D]删除|[U]修改|[Q]查询|[R]返回");
		printf("\n==================================================================\n");
		printf("功能选择:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
				case 'a':
				case 'A':
					if (Play_UI_Add()) //新添加成功，跳到最后一页显示
					{
						paging.totalRecords = Play_Srv_FetchAll(head);
						Paging_Locate_LastPage(head, paging, play_node_t);
					}
					break;
				case 'd':
				case 'D':
					printf("输入ID:");
					scanf("%d", &id);
					if (Play_UI_Delete(id)) {	//从新载入数据
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'u':
				case 'U':
					printf("输入ID:");
					scanf("%d", &id);
					if (Play_UI_Modify(id)) {	//从新载入数据
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'q':
				case 'Q':
					printf("输入ID:");
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
 * Description: 添加一条剧目信息
 * Input:       无
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Play_UI_Add(void)
{
	play_t rec;
	int newRecCount = 0;
	char choice;

	do {
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("\t\t****************  添加新剧目 ***************\n");
		printf("-------------------------------------------------------\n");
		printf("剧目名称:");
		fflush(stdin);
		fgets(rec.name,30,stdin);
		printf("剧目类型(1表示电影,2歌剧,3音乐会):");
		scanf("%d", &(rec.type));
		printf("来源地区:");
		fgets(rec.area,9,stdin);
		printf("剧目级别(1表示儿童,2青少年,3音乐会):");
		scanf("%d", &(rec.rating));
		printf("演出时长(分钟):");
		scanf("%d", &(rec.duration));
		printf("上映日期:(yyyy/mm/dd):");
		scanf("%d/%d/%d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
		printf("结束日期:(yyyy/mm/dd):");
		scanf("%d/%d/%d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);
		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Play");
		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("新剧目添加成功!\n");
		} else
			printf("新剧目添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]添加更多, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: 更新剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_UI_Modify(int id){
	play_t rec;
	int rtn = 0;
	/*Load record*/
	//schedule__list_t sch;
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("该剧目不存在!\n按[Enter]键返回!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("\t\t****************  修改剧目  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("剧目编号:%d\n", rec.id);
	printf("剧目名称:[%s]",rec.name);
	fflush(stdin);
	fgets(rec.name,30,stdin);
	printf("剧目类型(1表示电影,2歌剧,3音乐会):[%d]",rec.type);
	scanf("%d", &(rec.type));
	printf("来源地区:[%s]",rec.area);
	fgets(rec.area,9,stdin);
	printf("剧目级别(1表示儿童,2青少年,3音乐会)[%d]:",rec.rating);
	scanf("%d", &(rec.rating));
	printf("演出时长(分钟):[%d]",rec.duration);
	scanf("%d", &(rec.duration));
	printf("上映日期:(yyyy/mm/dd):[%4d/%2d/%2d]",rec.start_date.year,rec.start_date.month,rec.start_date.day);
	scanf("%d/%d/%d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
	printf("结束日期:(yyyy/mm/dd)::[%4d/%2d/%2d]",rec.end_date.year,rec.end_date.month,rec.end_date.day);
	scanf("%d/%d/%d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);
	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf("剧目数据修改成功!\n按[Enter]返回!\n");
	} else
		printf("剧目数据修改失败!\n按[Enter]返回\n");
	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_UI_Delete(int id){
	int rtn = 0;
	if(Play_Srv_DeleteByID(id)){
		printf("剧目删除成功!\n");
		return 1;
	}else{
		printf("剧目删除失败!\n");
		return 0;
	}

	if (1) {
		//在删除剧目时，同时删除演出计划

		if(Schedule_Srv_DeleteByID(id))
			printf("演出厅座位删除成功!\n");//The seats of the room deleted successfully
		printf(	"该演出厅删除成功!\n按[Enter]键返回!\n");//The room deleted successfully!\nPress [Enter] key to return
		rtn = 1;
	} else {
		printf("该演出厅不存在!\n按[Enter]键返回!\n");//The room does not exist!\nPress [Enter] key to return
	}

	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: 按照ID号查询剧目信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Play_UI_Query(int id){
    int rtn=0;
    play_t buf;
    rtn=Play_Srv_FetchByID(id,&buf);
	printf("\n============================================================\n");
	printf("*********************** 剧目查询 **************************\n");
	printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s","编号","名称","类型","地区","级别","时长",
				"上映日期","结束日期","票价");
	printf("%2d  %10s  %4d  %6s  %4d  %4d  %2d-%2d  %2d-%2d  %2d\n",buf.id
						,buf.name,buf.type,buf.area,buf.rating
						,buf.duration,buf.start_date.month,buf.start_date.day
						,buf.end_date.month,buf.end_date.day,buf.price);
	printf("查询完成,请按[Enter]返回\n");
	getchar();
	return rtn;
}
