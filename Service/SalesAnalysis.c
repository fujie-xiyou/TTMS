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
#include "../Common/List.h"

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
    int rtn=0;
    List_Free(list,salesanalysis_node_t);
    List_Init(playlist,play_node_t);
    Play_Srv_FetchAll(playlist);
    List_ForEach(playlist,pos){
        if(!newNode=(salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t))){
            printf("�ڴ�����ʧ��!\n");
            return rtn;
        }
        newNode->data.play_id=pos->data.id;
        strcpy(newNode->data.name,pos->data.name);
        strcpy(newNode->data.area,pos->data.area);
        newNode->data.duration=pos->data.duration;
        newNode->data.sales=Schedule_Srv_StatRevByPlay(newNode->data.play_id,&sold,&total);
        newNode->data.totaltickets=sold;
        newNode->data.price=pos->data.price;
        newNode->data.start_date=pos->data.start_date;
        newNode->data.end_date=pos->data.end_date;
        List_AddTail(list,newNode);
        rtn++;
    }
    List_Destroy(playlist,play_node_t);

	return rtn;
}

//�Ծ�Ժ��ӰƱ������
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){
    if(List_IsEmpty(list))
        return;
    salesanalysis_list_t p,q,listLeft;
    list->prev->next=NULL;
    listLeft=list;
    list->next=list->prev=list;
    List_ForEach(listLeft,p){
        if(List_IsEmpty(list)){
            List_AddTail(list,p);
        }else{
            List_ForEach(lsit,q){
                if(q->data.sales>p->data.sales){
                    List_InsertBefore(q,p);
                }
            }
        }
    }


}


