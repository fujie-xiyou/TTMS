/*
 * salesanalysis.c
 *
 *  Created on: 2015年6月12日
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

//计算员工usrID在给定时间区间的销售额，返回销售额
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

//结合剧目Play.dat,统计销售数据，构建销售分析salesanalysis_list_t list链表，返回list链表长度
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
            printf("内存申请失败!\n");
            return ;
        }
        List_AddTail(list,pos);
    }

	return 1;
}

//对剧院电影票房排序
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){

}


