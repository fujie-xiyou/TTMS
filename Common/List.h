/*************************************************************************
	>    File Name: List.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年06月09日 星期五 19时28分27秒
 ************************************************************************/
#include<stdlib.h>
#ifndef _LIST_H
#define _LIST_H
#endif
#define List_Init(list,list_node_t) \
    list=(list_node_t*)malloc(sizeof(list_node_t));\
    list->next=list;\
    list->prev=list;\


#define List_Free(list,list_node_t) \
    list_node_t *pnow=list,*temp;\
    while(pnow->next!=list){\
        temp=pnow->next->next;\
        free(pnow->next);\
        pnow->next=temp;\
    }\


#define List_Destroy(list,list_node_t) \
    list_node_t *pnow=list,*temp;\
    while(pnow->next!=list){\
        temp=pnow->next->next;\
        free(pnow->next);\
        pnow->next=temp;\
    }\
    free(list);\


#define List_AddHead(list,newNode) \
    newNode->next=list->next;\
    newNode->prev=list;\
    list->next->prev=newNode;\
    list->next=newNode;\


#define List_AddTail(list,newNode) \
    newNode->next=list;\
    newNode->prev=list->prev\
    list->prev->next=newNode;\
    list->prev=newNode;\


#define List_InsertBefore(node,newNode) \
    newNode->next=node;\
    newNode->prev=node->prev;\
    node->prev->next=newNode;\
    node->prev=newNode;\


#define List_InsertAfter(node,newNode) \
    newNode->next=node->next;\
    newNode->prev=node;\
    node->next->prev=newNode;\
    node->next=newNode;\


#define List_IsEmpty(list) (list->next==list)?(true):(false);


#define List_DelNode() fprintf(stderr,"fujie认为函数List_DelNode()没有必要实现,如果你看到这句,请联系他。");


#define List_FreeNode(node) \
    node->next->prev=node->prev;\
    node->prev->next=node->next;\
    free(node);

#define List_ForEach(list,curPos) \
    curPos=list;\
        while(curPos=curPos->next,curPos!=list)
