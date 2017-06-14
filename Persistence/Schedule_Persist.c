/*
* File name:  Schedule_Persist.c
* File ID:	  TTMS_SSF_Schedule_Perst_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Schedule_Persist.h"
#include "../Service/Schedule.h"
#include "../Common/List.h"
#include "../Service/Ticket.h"
//#include "../Persistence/Common_Persist.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//保存演出计划信息
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////删除或更新时，暂时保存演出计划信息

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: 在演出计划信息文件末尾写入一条演出计划信息
 * Input:       待加入文件的演出计划信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
 */
int Schedule_Perst_Insert(const schedule_t *data)
{
    FILE *fp;
    int rtn=0;
    if(fp=fopen(SCHEDULE_DATA_FILE,"ab")==NULL){
        printf("%s打开失败!\n",SCHEDULE_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(schedule_t),1,fp);
    fclose(fp);
    return rtn;
}

/*
 * Function:    Schedule_Perst_Update
 * Function ID:	TTMS_SCU_Schedule_Perst_Mod
 * Description: 按照演出计划ID号更新文件中的演出计划信息
 * Input:       待在文件中更新的演出计划信息数据
 * Output:      打开文件失败的信息
 * Return:      更新的演出计划信息数，0表示未找到，1表示找到并更新
 */
int Schedule_Perst_Update(const schedule_t *data){
    FILE *fp=fopen(SCHEDULE_DATA_FILE,"wb+");
    schedule_t buf;
    int rtn=0;
    if(fp==NULL){//看看人家这么写也挺不错的嘻嘻
        printf("%s打开失败!\n");
        return 0;
    }
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id==data->id){
            fseek(fp,-sizeof(buf),SEEK_CUR);
            if(!fwrite(data,sizeof(buf),1,fp)){
                printf("%s写入失败!\n",SCHEDULE_DATA_FILE);
            }
            else{
                rtn=1;
            }
            break;
    }
    fclose(fp);
    return rtn;
    }
}

/*
 * Function:    Schedule_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Perst_DelByID
 * Description: 按照演出计划ID号删除演出计划的信息
 * Input:       待删除的演出计划ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_Perst_DeleteByID(int ID){

    rename(SCHEDULE_DATA_FILE,SCHEDULE_DATA_TEMP_FILE);
    schedule_t buf;
    int rtn=0;
    FILE *fp=fopen(SCHEDULE_DATA_TEMP_FILE,"rb");
    if(fp==NULL){
        printf("%s打开失败\n",SCHEDULE_DATA_TEMP_FILE);
        return 0;
    }
    FILE *fd=fopen(SCHEDULE_DATA_FILE,"wb");
    if(fd==NULL){
        printf("%s打开失败!\n",SCHEDULE_DATA_FILE);
        return 0;
    }
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id!=ID){
            fwrite(&buf,sizeof(buf),1,fd);
        }
        else{
            rtn=1;
        }
    }
    return rtn;

}

/*
 * Function:    Schedule_Perst_SelectByID
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByID
 * Description: 按照演出计划ID号删除演出计划的信息
 * Input:       待查找的演出计划ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
    int rtn=0;
    FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s打开失败!\n");
        return rtn;
    }
    schedule_t data;
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        if(data.id==ID){
            *buf=data;
            rtn=1;
            break;
        }
    }
    fclose(fp);
    return rtn;
}

/*
 * Function:    Schedule_Perst_SelectAll
 * Function ID:	TTMS_SCU_Schedule_Perst_SelAll
 * Description: 将所有演出计划信息建立成一条链表
 * Input:       list为演出计划信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Schedule_Perst_SelectAll(schedule_list_t list){

    int recCount=0;
    schedule_list_t newNode;
    schedule_t data;

    assert(NULL!=list);

    List_Free(list,schedule_node_t);

    FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s打开失败!\n");
        return recCount;
    }
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        newNode=(schedule_list_t)malloc(sizeof(schedule_node_t));
        if(newNode==NULL){
            printf("内存申请失败!\n");
            break;
        }
        newNode->data=data;
        List_AddTail(list,newNode);
        recCount++;
    }
    fclose(fp);


    return recCount;

}

/*
 * Function:    Schedule_Perst_SelectByPlay
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByPlay
 * Description: 将同意剧目的演出计划信息搜索出来，建立一条链表
 * Input:       list为满足条件的演出计划信息链表的头指针，剧目的ID号
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){

    int recCount=0;
    schedule_list_t newNode;
    schedule_t data;
    assert(NULL!=list);
    List_Free(list,schedule_node_t);
    FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s打开失败!\n",SCHEDULE_DATA_FILE);
        return recCount;
    }
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        if(data.id==play_id){
            newNode=(schedule_list_t)malloc(sizeof(schedule_node_t));
            if(newNode==NULL){
                printf("内存申请失败!\n");
                break;
            }
            newNode->data=data;
            List_AddTail(list,newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;

}
