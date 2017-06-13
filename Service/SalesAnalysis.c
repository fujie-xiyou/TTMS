/*
 * salesanalysis.c
 *
 *  Created on: 2015��6��12��
 *      Author: Administrator
 */
#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"

//����Ա��usrID�ڸ���ʱ����������۶�������۶�
inline int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate){
    int amount=0;
    sale_list_t saleList;
    sale_list_t pSale;
    List_Init(saleList,sale_node_t);
    Sale_Perst_SelectByUsrID(saleList,usrID);
    List_ForEach(saleList,pSale){
        amount+=pSale->data.value;
    }
	return amount;
}

//��Ͼ�ĿPlay.dat,ͳ���������ݣ��������۷���salesanalysis_list_t list��������list������
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {
    assert(NULL!=list);
    play_list_t playlist;
    play_list_t pos;
    salesanalysis_list_t newNode;
    int sold=0;
    int total=0;
    List_Free(list,salesanalysis_node_t);
    List_Init(playlist,play_node_t);
    Play_Ser_FetchAll(playlist);
    List_ForEach(playlist,pos){
        if(!newNode=(salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t))){
            printf("�ڴ�����ʧ��!\n");
            return ;
        }
        List_AddTail(list,pos);
    }

	return 1;
}

//�Ծ�Ժ��ӰƱ������
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){

}


