/*
 * Account_Persist.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */

#include "Account_Persist.h"
#include "../Service/account.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>
#include <string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";

//�ж��˺��ļ��Ƿ���ڣ�����1���ڣ�����0������
int Account_Perst_CheckAccFile() {
    FILE *fp==fopen(ACCOUNT_DATA_FILE,"rb");
    if(fp==NULL){
        return 0;
    }
    fclose(fp);
	return 1;
}

//�����û��������˺�,����ɹ� return 1������ return 0
int Account_Perst_SelByName(char usrName[], account_t *buf) {
    FILE *fp=fopen(ACCOUNT_DATA_FILE,"rb");
    account_t data;
    int rtn=0;
    if(fp==NULL){
        printf("%s��ʧ��!\n",ACCOUNT_DATA_FILE);
        return rtn;
    }
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        if(strcmp(data.username,usrName)==0){
            *buf=data;
            rtn=1;
            break;
        }
    }
    fclose(fp);
	return rtn;

}

//���˺�д���˺��ļ��У�����ʵ��д������ݿ���Ŀ
int Account_Perst_Insert(const account_t *data) {
    int rtn=0;
    FILE *fp=fopen(ACCOUNT_DATA_FILE,"ab");
    if(fp==NULL){
        printf("%s��ʧ��!\n",ACCOUNT_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(account_t),1,fp);
    fclose(fp);
	return rtn;
}

//���˺��ļ��в���������˺�ƥ����˺ţ��ҵ� return 1������ return 0�������и�����д
int Account_Perst_Update(const account_t * data) {
    int rtn=0;
    FILE *fp=fopen(ACCOUNT_DATA_FILE,wb+);
    if(fp==NULL){
        printf("%s��ʧ��!\n",ACCOUNT_DATA_FILE);
        return rtn;
    }
    account_t buf;
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id==data->id){
            fseek(fp,-sizeof(buf),SEEK_CUR);
            rtn=fwrite(data,sizeof(buf),1,fp);
        }
    }
    fclose(fp);
	return rtn;
}

//���˺��ļ���ɾ�������idƥ����˺ţ�ɾ���ɹ� return 1������ return 0��
int Account_Perst_DeleteByID(int id) {
    int rtn=0;
    if(!rename(ACCOUNT_DATA_FILE,ACCOUNT_DATA_TEMP_FILE)){
        printf("%s������ʧ��!",ACCOUNT_DATA_FILE);
        return rtn;
    }
    FILE *fp=fopen(ACCOUNT_DATA_TEMP_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��!\n",ACCOUNT_DATA_TEMP_FILE);
        return rtn;
    }
    FILE *fd=fopen(ACCOUNT_DATA_FILE,"wb");
    if(fd==NULL){
        printf("%s��ʧ��\n",ACCOUNT_DATA_FILE)
    }
    account_t buf;
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(buf.id!=id){
            fwrite(&buf,sizeof(buf),1,fd);
        }else{
            rtn=1;
        }
    }
    fclose(fp);
    fclose(fd);
    remove(ACCOUNT_DATA_TEMP_FILE);
	return rtn;

}

//���˺��ļ��в��������idƥ����˺ţ���ͨ��ָ��buf������ƥ��ɹ� return 1������ return 0��
int Account_Perst_SelectByID(int id, account_t *buf) {
    int rtn=0;
    FILE *fopen(ACCOUNT_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��\n",ACCOUNT_DATA_FILE);
        return rtn;
    }
    account_t data;
    while(!feof(fp)){
        fread(&data,sizeof(data),1,fp);
        if(data.id==id){
            *buf=data;
            rtn=1;
            break;
        }
    }
    fclose(fp);
	return rtn;
}

//������ACCOUNT_DATA_FILE�ļ�����̬�����û��˺�list����,list Ϊ����ͷָ�룬����list����
int Account_Perst_SelectAll(account_list_t list) {
    assert(NULL!=list);
    int rtn=0;
    List_Free(list,account_node_t);
    FILE *fp=fopen(ACCOUNT_DATA_FILE,"rb");
    if(fp==NULL){
        printf("&s��ʧ��!\n");
        return rtn;
    }
    account_t buf;
    account_list_t newNode=NULL;
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(!newNode=(account_list_t)malloc(sizeof(account_node_t))){
           printf("�ڴ�����ʧ��!\n");
            return rtn;
        }
        newNode->data=buf;
        List_AddTail(list,newNode);
        rtn++;
    }
    fclose(fp);
	return rtn;
}

