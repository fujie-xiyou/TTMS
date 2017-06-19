#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"
//#include "../Service/Play.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

//根据计划ID，显示演出票
void Sale_UI_ShowTicket(int schID){
	ticket_list_t head,pos;
	int i;
	char choice;
	seat_list_t seatList;
	Pagination_t paging;
	schedule_t sch;
	play_t ply;
	studio_t stu;
	seat_t set;
	List_Init(head,ticket_node_t);
	paging.offset=0;
	paging.pageSize=TICKET_PAGE_SIZE;
	paging.totalRecords=Ticket_Srv_FetchBySchID(head,schID);
	Paging_Locate_FirstPage(head,paging);

	do{
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("********************** 演出计划票列表 **********************\n");
		printf("%2s %10s %10s %3s %3s %3s %4s","座位号","剧名","播放厅名","行号","列号","票价","状态");
		printf("--------------------------------------------------------\n");
		for(i=0,pos=(ticket_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
			Schedule_Srv_FetchByID(schID,&sch);
			Play_Srv_FetchByID(sch.play_id,&ply);
			Studio_Srv_FetchByID(sch.studio_id,&stu);
			Seat_Srv_FetchByID(pos->data.seat_id,&set);
			printf("%2d %10s %10s %2d %2d %2d %4s",pos->data.seat_id,ply.name,stu.name
					,set.row,set.column,pos->data.price,
					(pos->data.status==0)?("待售"):((pos->data.status==1)?("已售"):("预留")));
			pos=pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[S]售票|[P]上一页|[N]下一页|[R]返回");
		printf("\n==================================================================\n");
		printf("功能选择:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch(choice){
		case 'S':
		case 's':
			List_Init(seatList,seat_node_t);
			Seat_Srv_FetchValidByRoomID(seatList,stu.id);
			Sale_UI_SellTicket(head,seatList);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
		}
}while(choice!='R' && choice!='r');
	List_Destroy(head,ticket_node_t);
	List_Destroy(seatList,seat_node_t);
}
inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){
	int row,col;
	seat_node_t * pSeat=NULL;
	ticket_node_t *tic;
	sale_t sal;
	printf("\n===============================================================\n");
	printf("**************************  售票  ********************************\n");
	printf("------------------------------------------------------------------\n");
	printf("请输入座位行号:");
	scanf("%d",&row);
	printf("请输入座位列号:");
	scanf("%d",&col);
	return 0;
	pSeat=Seat_Srv_FindByRowCol(seatList,row,col);
	if(!pSeat){
		printf("输入的座位不存在!\n");
		return 0;
	}
	tic=Ticket_Srv_FindBySeatID(tickList,pSeat->data.id);
	if(!tic)
		return 0;
	if(tic->data.status!=0){
		printf("此票已售出或被预留!\n");
	}
	sal.id=EntKey_Srv_CompNewKey("Sale");
	sal.user_id=gl_CurUser.id;
	sal.ticket_id=tic->data.id;
	sal.date=DateNow();
	sal.time=TimeNow();
	sal.value=tic->data.price;
	sal.type=SALE_SELL;
	if(Sale_Srv_Add(&sal)){
		printf("出票成功!应收取 %d 元!",tic->data.price);
	}
}



//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID) {
	play_t buf;
	schedule_list_t head,pos;
	Pagination_t paging;
	int i,schedule_id;
	char choice;
	studio_t stu;
	Play_Srv_FetchByID(playID,&buf);
	List_Init(head,schedule_node_t);
	paging.offset=0;
	paging.totalRecords=Schedule_Srv_FetchByPlay(head,playID);
	paging.pageSize=SCHEDULE_PAGE_SIZE;
	Paging_Locate_FirstPage(head,paging);
	do{
		//system("cls");
		printf("\n========================================================\n");
	    printf("**********************剧目演出计划列表**********************\n");
	    printf("%4s  %10s  %10s  %5s %5s\n","编号","剧名","演出厅","日期","时间");
	    printf("--------------------------------------------------------\n");
	    for(i=0,pos=(schedule_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
	    	Studio_Srv_FetchByID(pos->data.studio_id,&stu);
		    printf("%4d  %10s  %10s  %2d/%2d %2d:%2d\n",pos->data.id,buf.name,stu.name,pos->data.date.month
		    		,pos->data.date.day,pos->data.time.hour,pos->data.time.minute);
		    pos = pos->next;
	    }
	    printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
	    				paging.totalRecords, Pageing_CurPage(paging),
	    				Pageing_TotalPages(paging));
	    printf("--------------------------------------------------------\n");
	    printf("[T]显示票|[P]上一页|[N]下一页|[R]返回\n");
	    printf("请输入功能选择:");
	    //getc(choice);
	    scanf("%c",&choice);
	    switch(choice){
		  	case 'T':
		 	case 't':
		       	printf("请输入要显示票的演出计划ID:");
		      	scanf("%d",&schedule_id);
		       	Sale_UI_ShowTicket(schedule_id);
		        break;
		   	case 'P':
		 	case 'p':
		 		if (1 < Pageing_CurPage(paging)) {
		               Paging_Locate_OffsetPage(head,paging,-1,schedule_node_t);
		           }
		          break;
		 	case 'N':
		 	case 'n':
		 		if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
		 			Paging_Locate_OffsetPage(head,paging,1,schedule_node_t);
		        }
		        break;
	    }
	}while(choice!='R' &&  choice!='r');
	List_Destroy(head,schedule_node_t);
}

void Sale_UI_MgtEntry() {
	play_list_t head,pos;//list;//list用于存储按名字查询到的记录
	Pagination_t paging;
	int i,play_id;
	char choice,name[30];
	List_Init(head,play_node_t);
	//List_Init(list,play_node_t);
	paging.offset=0;
	paging.pageSize=PLAY_PAGE_SIZE;
	paging.totalRecords=Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head,paging);
	do{
		//system("cls");
		printf("\n============================================================\n");
		printf("*********************** 剧目列表 **************************\n");
		printf("%2s  %10s  %4s  %6s  %4s  %4s  %6s  %6s %4s","编号","名称","类型","地区","级别","时长",
					"上映日期","结束日期","票价");
		printf("--------------------------------------------------------\n");
		for(i=0,pos=(play_node_t*)(paging.curPos);pos!=head && i<paging.pageSize;i++){
			printf("%2d  %10s  %4d  %6s  %4d  %4d  %2d-%2d  %2d-%2d  %2d",pos->data.id
					,pos->data.name,/*pos->data.type*/ 1,pos->data.area,pos->data.rating
					,pos->data.duration,pos->data.start_date.month,pos->data.start_date.day
					,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);
			pos = pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("******************************************************************\n");
		printf("[C]演出计划|[S]查找剧目名|[F]过滤|[P]上一页|[N]下一页|[R]返回");
		printf("\n==================================================================\n");
		printf("功能选择:");//Your Choice
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
			case 'C':
			case 'c':
				printf("请输入要查询演出计划的剧目编号:");
				scanf("%d",&play_id);
				Sale_UI_ShowScheduler(play_id);
				break;
			case 'S':
			case 's':
				printf("请输入要查询的关键字:");
				fgets(name,30,stdin);
				Play_Srv_FetchByName(head,name);
				break;
            case 'P':
            case 'p':
            	if (1 < Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(head,paging,-1,play_node_t);
                }
                break;
            case 'N':
            case 'n':
            	if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
                    Paging_Locate_OffsetPage(head,paging,1,play_node_t);
                }
                break;
            //暂时砍掉F
		}
	}while(choice!='R' && choice!='r');
	 List_Destroy(head,play_node_t);
}

//退票
void Sale_UI_ReturnTicket(){
	int id;
	ticket_t tic;
	printf("\n===============================================================\n");
	printf("**************************  退票  ********************************\n");
	printf("------------------------------------------------------------------\n");
	printf("请输入要退的票的ID:");
	scanf("%d",&id);
	if(!Ticket_Srv_FetchByID(id,&tic)){
		printf("要退的票不存在!\n");
		getchar();
		return ;
	}
	if(tic.status!=1){
		printf("此票尚未售出!\n");
		getchar();
		return;
	}
	tic.status=0;
	if(Ticket_Srv_Modify(&tic)){
		printf("退票成功!应退款 %d 元!",tic.price);
	}else{
		printf("抱歉!退票失败!\n");
	}
	getchar();
	return;
}
