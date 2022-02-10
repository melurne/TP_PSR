#ifndef __PINGPONG_H__
#define __PINGPONG_H__

#define KEY	(key_t)1000
#define MSG_SIZE	64

typedef struct pongmsg_s 
{
	long type;
	char msg[MSG_SIZE];
} pongmsg_t;

#endif
