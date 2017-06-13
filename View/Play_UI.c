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
	 * Description: 以列表模式显示剧目信息
	 * Input:       list剧目信息链表的头指针，paging分页设置参数
	 * Output:      所有剧目的信息
	 * Return:      返回查找到的记录数目
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	int i;
	play_node_t *pos;

	List_Init(list, studio_node_t);
	paging.offset = 0;
	paging.pageSize = ;//不知道

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(play);
	Paging_Locate_FirstPage(play, paging); 
	printf("***************剧目信息********************\n");
	printf("ID\t名称\t类型\t出品地区\t等级\t时长\t开始时间\t结束时间\t票价\n");
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (play_node_t *) (paging.curPos);
				pos != list && i < paging.pageSize; i++) {
			printf("ID:%d\t",rec.id);
		printf("剧目名称:%s\t",rec.name);
		fflush(stdin);
		printf("\n");
		printf("剧目类型:");//不会 
		printf("出品地区:%s\t", rec.area);
		printf("剧目等级：");//不会
		printf("时长:%d\t",rec.duration);
		printf("开始日期：");//不会 
		printf("结束日期：");//不会
		printf("票价:%d\t",rec.price);
			pos = pos->next;
		}
		printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		
		
	//释放链表空间
	List_Destroy(list, studio_node_t);

}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: 剧目信息管理界面
 * Input:       flag为0，进入管理页面，flag为1进入查询界面
 * Output:      剧目管理界面
 * Return:      无
 */
void Play_UI_MgtEntry(int flag){
	int i, id;
	char choice;

	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.pageSize = 0;///不知道该等于几 

	//载入数据
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
			if (Play_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, play_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (play_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 's':
		case 'S'://////有问题 
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
	//释放链表空间
	List_Destroy(head, play_node_t);


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
		printf("****************  添加新的剧目 ****************\n");
		printf("-------------------------------------------------------\n");
		printf("剧目名称:");
		scanf("%s",rec.name);
		printf("剧目类型:");
		//buhui
		printf("出品地区：%d");
		scanf("%d\n", rec.area);
		printf("剧目等级：");
		//不会
		printf("时长:");
		scanf("%d\n",rec.duration);
		printf("开始日期：");
		//不会 
		printf("结束日期：");
		//不会
		printf("票价：");
		scanf("%d\n",rec.price);
		 
		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&rec)) {
			newRecCoun+=1;
			printf("添加成功!\n");
		} else
			printf("添加失败!\n");
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
 * Description: 更新剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
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
		printf("****************  原来剧目信息 ****************\n");
		printf("-------------------------------------------------------\n");
		printf("ID\t名称\t类型\t出品地区\t等级\t时长\t开始时间\t结束时间\t票价\n");
		printf("ID:%d\t",rec.id);
		printf("剧目名称:%s\t",rec.name);
		fflush(stdin);
		printf("\n");
		printf("剧目类型:");//不会 
		printf("出品地区:%s\t", rec.area);
		printf("剧目等级：");//不会
		printf("时长:%d\t",rec.duration);
		printf("开始日期：");//不会 
		printf("结束日期：");//不会
		printf("票价:%d\t",rec.price);
		 
		printf("=======================================================\n");
		printf("****************  输入新的剧目信息 ****************\n");
		printf("-------------------------------------------------------\n");
		printf("剧目名称:");
		scanf("%s",rec.name);
		printf("剧目类型:");
		//buhui
		printf("出品地区：%d");
		scanf("%d\n", rec.area);
		printf("剧目等级：");
		//不会
		printf("时长:");
		scanf("%d\n",rec.duration);
		printf("开始日期：");
		//不会 
		printf("结束日期：");
		//不会
		printf("票价：");
		scanf("%d\n",rec.price);
		 
		printf("=======================================================\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"修改成功!\nPress [Enter] key to return!\n");
	} else
		printf("修改失败!\nPress [Enter] key to return!\n");

	getchar();
	return rtn;
	return 0;
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

	if (Play_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("删除成功!\n");
		rtn = 1;
	} else {
		printf("删除失败!\nPress [Enter] key to return!\n");
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
	if(Play_Srv_FetchByID（id）)
	{
		printf("成功!\n");
		rtn=1;
	}
	else
	{
		printf("失败！");
	}

	return rtn;
}