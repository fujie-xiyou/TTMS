/*
  * Account_UI.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */

<<<<<<< HEAD
#include "Account_UI.h"//
#include "../Common/List.h"
=======
#include "Account_UI.h"
#include "../Common/list.h"
>>>>>>> c5a85cfa58bfe836049685a38495ca61dccf7fad
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
//#include<windows.h>
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() {

	return 1;

}

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
void Account_UI_MgtEntry() {

}


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {

	return 1;

}

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) {

	return 1;
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {

	return 1;
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {


	return 1;
}


