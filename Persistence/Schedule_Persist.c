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


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//�����ݳ��ƻ���Ϣ
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////ɾ�������ʱ����ʱ�����ݳ��ƻ���Ϣ

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: ���ݳ��ƻ���Ϣ�ļ�ĩβд��һ���ݳ��ƻ���Ϣ
 * Input:       �������ļ����ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
 */
int Schedule_Perst_Insert(const schedule_t *data)
{
    FILE *fp;
    int rtn=0;
    if(fp=fopen(SCHEDULE_DATA_FILE,"ab")==NULL){
        printf("%s��ʧ��!\n",SCHEDULE_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(schedule_t),1,fp);
    fclose(fp);
    return rtn;
}

/*
 * Function:    Schedule_Perst_Update
 * Function ID:	TTMS_SCU_Schedule_Perst_Mod
 * Description: �����ݳ��ƻ�ID�Ÿ����ļ��е��ݳ��ƻ���Ϣ
 * Input:       �����ļ��и��µ��ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      ���µ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_Perst_Update(const schedule_t *data){
    FILE *fp=fopen(SCHEDULE_DATA_FILE,"wb+");
    schedule_t buf;
    int rtn=0;
    if(fp==NULL){//�����˼���ôдҲͦ���������
        printf("%s��ʧ��!\n");
        return 0;
    }
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id==data->id){
            fseek(fp,-sizeof(buf),SEEK_CUR);
            if(!fwrite(data,sizeof(buf),1,fp)){
                printf("%sд��ʧ��!\n",SCHEDULE_DATA_FILE);
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
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_Perst_DeleteByID(int ID){

    rename(SCHEDULE_DATA_FILE,SCHEDULE_DATA_TEMP_FILE);
    schedule_t buf;
    int rtn=0;
    FILE *fp=fopen(SCHEDULE_DATA_TEMP_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��\n",SCHEDULE_DATA_TEMP_FILE);
        return 0;
    }
    FILE *fd=fopen(SCHEDULE_DATA_FILE,"wb");
    if(fd==NULL){
        printf("%s��ʧ��!\n",SCHEDULE_DATA_FILE);
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
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
    int rtn=0;
    FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��!\n");
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
 * Description: �������ݳ��ƻ���Ϣ������һ������
 * Input:       listΪ�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectAll(schedule_list_t list){

    int recCount=0;
    schedule_list_t newNode;
    schedule_t data;

    assert(NULL!=list);

    List_Free(list,schedule_node_t);

    FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��!\n");
        return recCount;
    }
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        newNode=(schedule_list_t)malloc(sizeof(schedule_node_t));
        if(newNode==NULL){
            printf("�ڴ�����ʧ��!\n");
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
 * Description: ��ͬ���Ŀ���ݳ��ƻ���Ϣ��������������һ������
 * Input:       listΪ�����������ݳ��ƻ���Ϣ�����ͷָ�룬��Ŀ��ID��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){

    int recCount=0;
    schedule_list_t newNode;
    schedule_t data;
    assert(NULL!=list);
    List_Free(list,schedule_node_t);
    FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��!\n",SCHEDULE_DATA_FILE);
        return recCount;
    }
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        if(data.id==play_id){
            newNode=(schedule_list_t)malloc(sizeof(schedule_node_t));
            if(newNode==NULL){
                printf("�ڴ�����ʧ��!\n");
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
