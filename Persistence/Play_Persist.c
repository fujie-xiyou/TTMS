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
  return 0;
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
   return 0;
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
   return 0;
}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: ���վ�ĿID�Ų��Ҿ�Ŀ����Ϣ
 * Input:       �����ҵľ�ĿID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {
   return 0;
}


/*
 * Function:    Play_Perst_SelectAll
 * Function ID:	TTMS_SCU_Play_Perst_SelAll
 * Description: �����о�Ŀ��Ϣ������һ������
 * Input:       list��Ŀ��Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Play_Perst_SelectAll(play_list_t list) {
   return 0;
}


/*
 * Function:    Play_Perst_SelectByName
 * Function ID:	TTMS_SCU_Play_Perst_SelByName
 * Description: ���վ�Ŀ���Ʋ��Ҿ�Ŀ����Ϣ
 * Input:       listΪ���ҵ��ľ�Ŀ��Ϣ����condtΪģ����ѯ�Ĺؼ���
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Play_Perst_SelectByName(play_list_t list, char condt[]) {
   return 0;
}

