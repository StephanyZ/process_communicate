/*************************************************************************
	> File Name: server.c
	> Author:xfzhang 
	> Mail:923036400@qq.com 
	> Created Time: 2018年09月10日 星期一 22时12分32秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<string.h>
#define MSG_INFO_FILE "."
typedef struct userdata{
    unsigned long int imsi;
    unsigned long int msisdn;
}userdata_t;
typedef struct info{
    char name[10];
    int old;
    int studentId;
}info_t;
typedef struct msg{
    int opcode;
    char data[1000];
}msg_t;
typedef struct message{
    long msg_tunnel;
    char buffer[1024];
}message_t;

int print_buffter(char *buffer,int len){
	int i = 0;
	for(i = 0;i<len+8;i++){
        printf("%x ",buffer[i]);
       	if(i%16 == 15)
        	printf("\n");
    }
    printf("\n");
	return 0;
}
int main(){
    userdata_t stUserData;
	info_t stdinfo;
	userdata_t *pstUserData = NULL;
	info_t *pstdinfo = NULL;
    msg_t stmsg;
	msg_t *pstmsg = NULL;
    key_t key =0;
    int type = 0;
    int qid =0;
    message_t message;
    memset(&message,0,sizeof(message_t));
    memset(&stmsg,0,sizeof(msg_t));
    memset(&stUserData,0,sizeof(userdata_t));
	memset(&stdinfo,0,sizeof(stdinfo));
    stUserData.imsi = 460012345678;
    stUserData.msisdn = 8618861822869;
	strcpy(stdinfo.name,"xfzhang");
	stdinfo.old = 23;
	stdinfo.studentId = 201310;
    
    int i = 0;
    int len = 0;
    if((key = ftok(MSG_INFO_FILE,'a')) == -1){
        perror("ftok");
        exit(1);
    }
    printf("key:%d\n",key);
    if((qid = msgget(key, IPC_CREAT|0777)) == -1){
        perror("msgget error");
        exit(1);
    }
    printf("qid:%d\n",qid);
    while(1){
		msgrcv(qid,&message,1024,888,0);
		pstmsg = (msg_t *)(char *)message.buffer;
		printf("recv opcode:%x\n",pstmsg->opcode);
		memset(&stmsg,0,sizeof(msg_t));
		if(pstmsg->opcode == 0x101){
			memset(&stmsg,0,sizeof(msg_t));
			stmsg.opcode = 1;
			memcpy(stmsg.data,&stUserData,sizeof(userdata_t));
			len = 4+sizeof(userdata_t);
			//printf("imsi:%lu ,msisdn:%lu\n",pstUserData->imsi,pstUserData->msisdn);
		}
		else if(pstmsg->opcode == 0x202){
			memset(&stmsg,0,sizeof(msg_t));
			stmsg.opcode = 1;
			memcpy(stmsg.data,&stdinfo,sizeof(info_t));
			len = 4+sizeof(info_t);
			//printf("name:%s , old:%d ,stuId:%d\n",pstdinfo->name,pstdinfo->old,pstdinfo->studentId);
		}
		memset(&message,0,sizeof(message_t));
        message.msg_tunnel = 999;
    	if(len >=1024)
        	len = 1024;
    	memcpy(message.buffer,&stmsg,len);
		printf("send message.buffter(len = %d):\n",len);
		print_buffter(message.buffer, len);
        msgsnd(qid,&message,sizeof(message.buffer),0);
    }
	return 0;
}
