#include "Sale_Persist.h"
#include "../Common/list.h"
#include "../Service/Sale.h"
#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>


static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";

int Sale_Perst_Insert(const sale_t *data) {
    FILE *fp=fopen(SALE_DATA_FILE,"ab");
    int rtn=0;
    if(fp==NULL){
        printf("&s打开失败!\n",SALE_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(sale_t),1,fp);
    fclose(fp);
    return rtn;
}

int Sale_Perst_DeleteByID(int saleID) {
    if(!rename(SALE_DATA_FILE,SALE_DATA_TEMP_FILE)){

    }

    FILE *fp=fopen(SALE_DATA_FILE,"wb+");
    sale_t buf;
    if(fp==NULL){
        printf("%s打开失败！\n",SALE_DATA_FILE);
        return 0;
    }
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id!=saleID){

        }
    }

	return 0;
}

//根据用户ID载入给定时间区间内的销售记录
int Sale_Perst_SelectByUsrID(sale_list_t list, int usrID, user_date_t stDate,
		user_date_t endDate) {

	
	return 0;
}


//根据给定时间区间内的销售记录
int Sale_Perst_SelectByDate(sale_list_t list, user_date_t stDate,
		user_date_t endDate) {

	
	return 0;
}

/*

//queryUid = null,query all the uid Sale information return the all uid Sale information number
//queryUid =  uid, query the uid Sale information return the uid Sale  number
 *
int  Sale_Infor_By_Uid(char * queryUid, Sale_list_t head)
{

	return 0;
}

int Sale_Infor_By_Time(char  queryTime[][30], Sale_list_t head)
{

	
	return 0;
}

//根据票的id获得票的基本信息 
int Select_Price_By_Playid(int id, ticket_t *buf)
{
	
		return 0;
}
*/

