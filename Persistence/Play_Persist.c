/*
* File name:  Play_Persist.c
* File ID:	  TTMS_SSF_Play_Perst_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Play_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";//�����Ŀ��Ϣ
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//ɾ�������ʱ����ʱ�����Ŀ��Ϣ

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: �ھ�Ŀ��Ϣ�ļ�ĩβд��һ����Ŀ��Ϣ
 * Input:       �������ļ��ľ�Ŀ��Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
 */
int Play_Perst_Insert(const play_t *data) {
    FILE *fp;
    int rtn=0;
    if(fp=fopen(PLAY_DATA_FILE,"ab")==NULL){
        fprintf(stderr,"%s��ʧ��!\n",PLAY_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(play_t),1,fp);
    fclose(fp);
    return rtn;
}

/*
 * Function:    Play_Perst_Update
 * Function ID:	TTMS_SCU_Play_Perst_Mod
 * Description: ���վ�ĿID�Ÿ����ļ��еľ�Ŀ��Ϣ
 * Input:       �����ļ��и��µľ�Ŀ��Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_Perst_Update(const play_t *data) {
    FILE *fp;
    int found=0;
    play_t buf;
    if(fp=fopen(PLAY_DATA_FILE,"wb+")==NULL){
        fprintf("stderr","%s��ʧ��!",PLAY_DATA_FILE);
        return found;
    }
    while(!feof(fp)){//���δ���ļ���β,����ѭ��
        fread(&buf,sizeof(play_t),1,fp);
        if(buf.id==data->id){
            fseek(fp,-sizeof(buf),SEEK_CUR);
            fwrite(data,sizeof(buf),1,fp);
            found=1;           
            break;
        }
    }
    fclose(fp);
    return found;
}

/*
 * Function:    Play_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Play_Perst_DelByID
 * Description: ���վ�ĿID��ɾ����Ŀ����Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_Perst_DeleteByID(int ID) {
    FILE *fp,*fd;
    int found=0;
    play_t buf;
    if(rename(PLAY_DATA_FILE,PLAY_DATA_TEMP_FILE)){
        fprintf(stderr,"%s������ʧ��!\n",PLAY_DATA_FILE);
        return found;
    }
    if(fp=fopen(PLAY_DATA_TEMP_FILE,"wb+")==NULL || 
       pd=fopen(PLAY_DATA_FILE,"wb")==NULL){
        fprintf(stderr,"%s��%s��ʧ��!\n",PLAY_DATA_FILE,PLAY_DATA_TEMP_FILE);
        return found;
    }
    while(!feof(fp)) {
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id==ID) {
            found=1;
        }
        else {
            fwrite(&buf,sizeof(buf),1,fd);
        }
    }

    fclose(fp);
    fclose(fd);
    remove(PLAY_DATA_TEMP_FILE);
    return found;
}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: ���վ�ĿID�Ų��Ҿ�Ŀ����Ϣ
 * Input:       �����ҵľ�ĿID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 * ע��:        ʵ��buf�����Ѿ������������г�ʼ��;
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {
    FILE *fp;
    int found=0;
    play_t data;
    if(fp=fopen(PLAY_DATA_FILE,"wb+")==NULL){
        printf("%s��ʧ��!\n",PLAY_DATA_FILE);
        return found;
    }
    while(!feof(fp)){
        fread(&data,sizeof(play_t),1,fp);
        if(data.id==ID){
            *buf=data;
            found=1;
            break; 
        }
    }
    fclose(fp);
    return found;
}


/*
 * Function:    Play_Perst_SelectAll
 * Function ID:	TTMS_SCU_Play_Perst_SelAll
 * Description: �����о�Ŀ��Ϣ������һ������
 * Input:       list��Ŀ��Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 * ע��:        list��������������г�ʼ��!!!
 */
int Play_Perst_SelectAll(play_list_t list) {
    FILE *fp;
    int recCount=0;
    play_t buf;
    play_list_t newNode;
    if(fp=fopen(PLAY_DATA_FILE,"rb")==NULL){
        printf("%s��ʧ��!\n",PLAY_DATA_FILE);
        return 0;
    }
    while(!feof(fp)){
        fread(buf,sizeof(buf),1,fp);
        if(newNode=(play_list_t)malloc(sizeof(play_node_t))==NULL){
            printf("�ڴ�����ʧ��!\n");
            break;
        }
        newNode->data=buf;
        List_AddTail(list,newNode);
        recCount++;
    }
    fclose(fp);
    return recCount;
}


/*
 * Function:    Play_Perst_SelectByName
 * Function ID:	TTMS_SCU_Play_Perst_SelByName
 * Description: ���վ�Ŀ���Ʋ��Ҿ�Ŀ����Ϣ
 * Input:       listΪ���ҵ��ľ�Ŀ��Ϣ����condtΪģ����ѯ�Ĺؼ���
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 * ע��:        list��������������г�ʼ��!!!
 */
int Play_Perst_SelectByName(play_list_t list, char condt[]) {
    int recCount=0;
    FILE *fp;
    play_t buf;
    play_list_t newNode;
    if(fp=fopen(PLAY_DATA_FILE,"rb")==NULL){
        printf("%s��ʧ��!",PLAY_DATA_FILE);
        return 0;
    }
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(strstr(buf.name,condt)){
            if(newNode=(play_list_t)malloc(sizeof(play_node_t))){
                printf("�ڴ�����ʧ��!\n");
                break;
            }
            newNode->data=buf;
            List_AddTail(list,newNode);
            recCount++;
        }
    }
    fclose(fp);
    return recCount;
}
