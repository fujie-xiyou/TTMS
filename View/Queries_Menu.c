#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "../Common/Common.h"
#include "Ticket_UI.h"

void Queries_Menu(void){
	char choice;
	int id;
	do {
		system("clear");
		printf("\n==================================================================\n");
		printf("************************ 信息查询 *************************\n");
		printf("[S]演出厅查询.\n");
		printf("[P]剧目查询.\n");
		printf("[T]票查询.\n");
		printf("[C]演出计划查询\n");
		printf("[R]返回.\n");
		printf("\n==================================================================\n");
		printf("请输入你的选择:");
		choice = getchar();
		ffflush(stdin);
		switch (choice) {
		case 'S':
		case 's'://Studio_query;
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			DisplayQueryPlay();
			break;
		case 'T':
		case 't': //Ticket_query;
			printf("请输入要查询的票ID:");
			scanf("%d",&id);
			ffflush();
			QueryTicket(id);
			break;
		case 'C':
		case 'c':
			Schedule_UI_ListAll();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
