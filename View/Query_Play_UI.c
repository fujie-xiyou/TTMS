#include "Query_Play_UI.h"
#include "../Service/Play.h"
#include "../Persistence/Query_Persist.h"
#include "../Service/Schedule.h"
#include "../Service/Account.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/Studio.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Common/Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern account_t gl_CurUser;
void DisplayQueryPlay(void)
{
	//ffflush();
	char fileName[30];
	play_t play;

	char choice;
	do {
		//ffflush();
		memset(fileName, 0, sizeof(fileName));
		printf("请输入剧目名称:");
		sgets(fileName,30);
		system("clear");
		printf("\n=======================================================\n");
		printf("\n****************  剧目信息列表  ***********\n");
		if (Query_PlayName(fileName,&play)) {
			printf("编号\t剧名\t地区\t级别\t上演时间\t结束时间\t票价\n");
			printf("-------------------------------------------------------\n");
			printf("%d\t%s\t%s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", play.id,
								play.name,	play.area,
								(play.rating==1?"儿童   ":(play.rating==2?"青年":"成人   ")),
								play.start_date.year,play.start_date.month,
								play.start_date.day,play.end_date.year,
								play.end_date.month,play.end_date.day,play.price);
			schedule_t  sched;
			studio_t studio = {0};
	//		seat_list_t list, int studio_id
			if (Query_Schedule_By_PlayId(play.id, &sched)) {
				Query_Studio_By_Schedule(sched.studio_id, &studio);
				printf("此剧目将于 %d:%d:%d %d:%d 在 %s 演出厅表演\n", sched.date.year, sched.date.month,
																			sched.date.day,sched.time.hour,
																			sched.time.minute, studio.name);
				printf("共有 %d 座位, 已售 %d, 待售 %d \n", Seat_Number_Count(sched.studio_id), Sold_Ticket_Count(sched.id),
																			Seat_Number_Count(sched.studio_id)-Sold_Ticket_Count(sched.id));
			} else {
				printf("抱歉,此剧目不上演!\n");
			}
			printf("[R]返回, [B]售票, [A]重新载入:");
			scanf("%c", &choice);
			ffflush();
			switch(choice) {
			case 'a':
			case 'A':
				break;
			case 'B':
			case 'b':
				//Sale the ticket
				if(gl_CurUser.type!=USR_CLERK){
					printf("您不是售票员!无权使用此功能!");
					getchar();
					return;
					break;
				}
				Sale_UI_ShowScheduler(play.id);
				return;
				break;
			}
		} else {
			printf("此剧目名不存在!\n");
			printf("[R]返回,[A]重新载入:");
			scanf("%c", &choice);
			ffflush();
			switch(choice) {
			case 'a':
			case 'A':
				break;
			case 'B':
			case 'b':
				//Sale the ticke
				break;
			}
		}
	} while (choice != 'r' && choice != 'R');
}


