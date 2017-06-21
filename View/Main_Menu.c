#include <stdio.h>

#include "Main_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Queries_Menu.h"
#include "Account_UI.h"
#include "Sale_UI.h"
#include "SalesAnalysis_UI.h"

extern account_t gl_CurUser;
void Main_Menu(void) {
	char choice;
	do {
		system("clear");
		printf("\n=======================================================\n");
		printf("**************** HLW剧院票务管理系统 ****************\n");
		printf("[S]演出厅管理.\n");
		printf("[P]剧目管理.\n");
		printf("[T]售票.\n");
		printf("[R]退票.\n");
		printf("[Q]查询.\n");
		printf("[N]排序和统计.\n");
		printf("[A]账户管理.\n");
		printf("[E]退出.\n");
		printf("\n=========================================================\n");
		printf("功能选择:");
		scanf("%c",&choice);
		ffflush();
		switch (choice) {
		case 'S':
		case 's':
			if(gl_CurUser.type!=USR_ADMIN){
				printf("您不是系统管理员!无权使用此功能!");
				getchar();
				break;
			}
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			if(gl_CurUser.type!=USR_MANG){
				printf("您不是经理!无权使用此功能!");
				getchar();
				break;
			}
			Play_UI_MgtEntry(0);
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			if(gl_CurUser.type!=USR_CLERK){
				printf("您不是售票员!无权使用此功能!");
				getchar();
				break;
			}
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			if(gl_CurUser.type!=USR_CLERK){
				printf("您不是售票员!无权使用此功能!");
				getchar();
				break;
			}
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAanalysis_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}

