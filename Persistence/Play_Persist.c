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

static const char PLAY_DATA_FILE[] = "Play.dat";//保存剧目信息
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//删除或更新时，暂时保存剧目信息

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: 在剧目信息文件末尾写入一条剧目信息
 * Input:       待加入文件的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
 */
int Play_Perst_Insert(const play_t *data) {
    FILE *fp;
    int rtn=0;
    if(fp=fopen(PLAY_DATA_FILE,"ab")==NULL){
        fprintf(stderr,"%s打开失败!\n",PLAY_DATA_FILE);
        return rtn;
    }
    rtn=fwrite(data,sizeof(play_t),1,fp);
    fclose(fp);
    return rtn;
}

/*
 * Function:    Play_Perst_Update
 * Function ID:	TTMS_SCU_Play_Perst_Mod
 * Description: 按照剧目ID号更新文件中的剧目信息
 * Input:       待在文件中更新的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_Perst_Update(const play_t *data) {
    FILE *fp;
    int found=0;
    play_t buf;
    if(fp=fopen(PLAY_DATA_FILE,"wb+")==NULL){
        fprintf("stderr","%s打开失败!",PLAY_DATA_FILE);
        return found;
    }
    while(!feof(fp)){//如果未到文件结尾,继续循环
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
 * Description: 按照剧目ID号删除剧目的信息
 * Input:       待删除的剧目ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_Perst_DeleteByID(int ID) {
    FILE *fp,*fd;
    int found=0;
    play_t buf;
    if(rename(PLAY_DATA_FILE,PLAY_DATA_TEMP_FILE)){
        fprintf(stderr,"%s重命名失败!\n",PLAY_DATA_FILE);
        return found;
    }
    if(fp=fopen(PLAY_DATA_TEMP_FILE,"wb+")==NULL || 
       pd=fopen(PLAY_DATA_FILE,"wb")==NULL){
        fprintf(stderr,"%s或%s打开失败!\n",PLAY_DATA_FILE,PLAY_DATA_TEMP_FILE);
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
 * Description: 按照剧目ID号查找剧目的信息
 * Input:       待查找的剧目ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 * 注意:        实体buf必须已经在主调函数中初始化;
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {
    FILE *fp;
    int found=0;
    play_t data;
    if(fp=fopen(PLAY_DATA_FILE,"wb+")==NULL){
        printf("%s打开失败!\n",PLAY_DATA_FILE);
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
 * Description: 将所有剧目信息建立成一条链表
 * Input:       list剧目信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 * 注意:        list务必在主调函数中初始化!!!
 */
int Play_Perst_SelectAll(play_list_t list) {
    FILE *fp;
    int recCount=0;
    play_t buf;
    play_list_t newNode;
    if(fp=fopen(PLAY_DATA_FILE,"rb")==NULL){
        printf("%s打开失败!\n",PLAY_DATA_FILE);
        return 0;
    }
    while(!feof(fp)){
        fread(buf,sizeof(buf),1,fp);
        if(newNode=(play_list_t)malloc(sizeof(play_node_t))==NULL){
            printf("内存申请失败!\n");
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
 * Description: 按照剧目名称查找剧目的信息
 * Input:       list为查找到的剧目信息链表，condt为模糊查询的关键字
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 * 注意:        list务必在主调函数中初始化!!!
 */
int Play_Perst_SelectByName(play_list_t list, char condt[]) {
    int recCount=0;
    FILE *fp;
    play_t buf;
    play_list_t newNode;
    if(fp=fopen(PLAY_DATA_FILE,"rb")==NULL){
        printf("%s打开失败!",PLAY_DATA_FILE);
        return 0;
    }
    while(!feof(fp)){
        fread(&buf,sizeof(buf),1,fp);
        if(strstr(buf.name,condt)){
            if(newNode=(play_list_t)malloc(sizeof(play_node_t))){
                printf("内存申请失败!\n");
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
