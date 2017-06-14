/*
 * salesanalysisPersist.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */

#include "../Persistence/EntityKey_Persist.h"
//#include "../Service/play.h"
#include "../Service/Sale.h"
#include "../Service/SalesAnalysis.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//static const char PLAY_DATA_FILE[] = "play.dat";
static const char SALE_DATA_FILE[] = "sale.dat";
static const char SALESANALYSIS_DATA_FILE[] = "salesanalysis.dat";

//��һ��salesanalysis��¼��*data��д��salesanalysis.dat�ļ����ɹ�return 1������return 0
int Salesanalysis_Perst_Insert(const salesanalysis_t *data) {

    int rtn=0;
    FILE *fp=fopen(SALESANALYSIS_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��!\n",SALESANALYSIS_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(salesanalysis_t),1,fp);
    fclose(fp);
	return rtn;


}

//������salesanalysis.dat�ļ��������۷�����salesanalysis������
int SalesAnalysis_Perst_SelectAll(salesanalysis_list_t list) {

    assert(NULL!=list);
    int rtn=0;
    List_Free(list,salesanalysis_node_t);
    FILE *fp=fopen(SALESANALYSIS_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��\n",SALESANALYSIS_DATA_FILE);
        return rtn;
    }
    salesanalysis_t buf;
    salesanalysis_list_t newNode=NULL;
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(!(newNode=(salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t)))){
            printf("�ڴ�����ʧ��\n");
            break;
        }
        newNode->data=buf;
        List_AddTail(list,newNode);
        rtn++;
    }
    fclose(fp);
	return rtn;


}

//������Sale.dat�ļ�����list��sale_list_t�� ������������list����
int Sale_Perst_SelectAll(sale_list_t list) {

    assert(NULL!=list);
    List_Free(list,sale_node_t);
    int rtn=0;
    sale_list_t newNode;
    FILE *fp=fopen(SALE_DATA_FILE,"rb");
    if(fp==NULL){
        printf("%s��ʧ��!\n",SALE_DATA_FILE);
        return rtn;
    }
    sale_t buf;
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(!(newNode=(sale_list_t)malloc(sizeof(sale_node_t)))){
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

