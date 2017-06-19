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
		printf("************************淇℃伅鏌ヨ*************************\n");
		printf("[S]婕斿嚭鍘呮煡璇�.\n");
		printf("[P]鍓х洰鏌ヨ.\n");
		printf("[T]绁ㄤ俊鎭煡璇�.\n");
		printf("[C]婕斿嚭璁″垝鏌ヨ\n");
		printf("[R]杩斿洖.\n");
		printf("\n==================================================================\n");
		printf("璇疯緭鍏ュ姛鑳介�夐」:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's'://Studio_query;
			break;
		case 'P':
		case 'p':
			//Play_UI_MgtEntry(1);
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
