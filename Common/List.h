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
#define List_Init(list,list_node_t) {\
    list=(list_node_t*)malloc(sizeof(list_node_t));\
    list->next=list;\
    list->prev=list;\
}

#define List_Free(list,list_node_t) {\
    list_node_t *pnow=list,*temp;\
    while(pnow->next!=list){\
        temp=pnow->next->next;\
        free(pnow->next);\
        pnow->next=temp;\
    }\
}

#define List_Destroy(list,list_node_t) {\
    list_node_t *pnow=list,*temp;\
    while(pnow->next!=list){\
        temp=pnow->next->next;\
        free(pnow->next);\
        pnow->next=temp;\
    }\
    free(list);\
}

#define List_AddHead(list,newNode) {\
    newNode->next=list->next;\
    newNode->prev=list;\
    list->next->prev=newNode;\
    list->next=newNode;\
}

#define List_AddTail(list,newNode) {\
    newNode->next=list;\
    newNode->prev=list->prev\
    list->prev->next=newNode;\
    list->prev=newNode;\
}

#define List_InsertBefore(node,newNode) {\
    newNode->next=node;\
    newNode->prev=node->prev;\
    node->prev->next=newNode;\
    node->prev=newNode;\
}

#define List_InsertAfter(node,newNode) {\
    newNode->next=node->next;\
    newNode->prev=node;\
    node->next->prev=newNode;\
    node->next=newNode;\
}


#define List_IsEmpty(list) (list->next==list)?(true):(false);


#define List_DelNode() fprintf(stderr,"fujie认为函数List_DelNode()没有必要实现,如果你看到这句,请联系他。\n");


#define List_FreeNode(node) {\
    node->next->prev=node->prev;\
    node->prev->next=node->next;\
    free(node);
}

#define List_ForEach(list,curPos) {\
    curPos=list;\
    while(curPos=curPos->next,curPos!=list)\
}

typedef struct{
    int totalRecords; //总数据记录器
    int offset; //当前页起始记录相对于第一条记录的偏移记录数
    int pageSize; //页面大小
    void *curPos; //当前页面起始记录在链表中的结点地址
} Pagination_t; //分页结构体,简称分页器类型

#define List_Paging(list,paging,list_node_t) {\
    int i=paging->offset;\
    list_node_t *temp=list;\
    while(i--){\
        paging->curPos=temp->next;\
        temp=temp->next;\
    }\
}
#define Paging_Locate_FirstPage(list,paging) {
    
} 

