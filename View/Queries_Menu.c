#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"

void Queries_Menu(void){
	char choice;
	do {
		printf("\n==================================================================\n");
		printf("************************信息查询*************************\n");
		printf("[S]演出厅查询.\n");
		printf("[P]剧目查询.\n");
		printf("[T]票信息查询.\n");
		printf("[C]演出计划查询\n");
		printf("[R]返回.\n");
		printf("\n==================================================================\n");
		printf("请输入功能选项:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's'://Studio_query;
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(1);
			break;
		case 'T':
		case 't': //Ticket_query;
			break;
		case 'C':
		case 'c':
			Schedule_UI_ListAll();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
