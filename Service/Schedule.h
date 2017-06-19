/*
* File name:  Schedule.h
* File ID:	  TTMS_SSF_Schedule_Srv_Head
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/Common.h"

typedef struct {//演出计划类型定义
	int id;            //演出计划ID号
	int play_id;       //演出的剧目ID号
	int studio_id;     //演出的放映厅ID号
	user_date_t date;  //演出日期
	user_time_t time;  //演出时间
	int seat_count;    //座位数量
}schedule_t;

//定义演出计划信息的双向链表结构
typedef struct schedule_node {
	schedule_t data;
	struct schedule_node *next, *prev;
} schedule_node_t, *schedule_list_t;

/*
 * Function:    Schedule_Srv_Add
 * Function ID:	TTMS_SCU_Schedule_Srv_Add
 * Description: 添加一条演出计划信息
 * Input:       待添加的演出计划信息数据
 * Output:      无
 * Return:      添加的记录数
 */
int Schedule_Srv_Add(const schedule_t *data);

/*
 * Function:    Schedule_Srv_Modify
 * Function ID:	TTMS_SCU_Schedule_ Srv _Mod
 * Description: 更新一条演出计划信息
 * Input:       待更新的演出计划信息数据
 * Output:      无
 * Return:      更新的演出计划信息数，0表示未找到，1表示找到并更新
 */
int Schedule_Srv_Modify(const schedule_t *data);

/*
 * Function:    Schedule_Srv_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Srv_DelByID
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的演出计划ID号
 * Output:      无
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_Srv_DeleteByID(int ID);

/*
 * Function:    Schedule_Srv_FetchByID
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByID
 * Description: 按照ID号查找一条演出计划信息，存入buf所指单元
 * Input:       待查找的演出计划ID号，将查找到的信息保存到buf中
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_Srv_FetchByID(int ID, schedule_t *buf);

/*
 * Function:    Schedule_Srv_FetchAll
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchAll
 * Description: 取出所有演出计划信息，串成一条链表
 * Input:       list演出计划信息链表的头指针
 * Output:      无
 * Return:      返回查找到的记录数目
 */
int Schedule_Srv_FetchAll(schedule_list_t list);

/*
 * Function:    Schedule_Srv_FetchByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByPlay
 * Description: 按照剧目ID号，将相关演出计划信息取出，串成一条链表
 * Input:       list为查找到的演出计划信息链表的头指针，play_id为剧目的ID号
 * Output:      无
 * Return:      返回查找到的记录数目
 */
int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id);

/*
 * Function:    Schedule_Srv_StatRevByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_StatByPlay
 * Description: 根据剧目ID，统计上座率及票房，返回票房数收入
 * Input:       play_id为待统计剧目的ID号，soldCount为卖出的票数，totalCount为总票数
 * Output:      无
 * Return:      返回票房收入
 */
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount, int *totalCount);


#endif //SCHEDULE_H_
