#ifndef _CALLBACK_H
#define _CALLBACK_H

typedef struct CallbackStruct {
	char *nichname;
	void *CallbackFtk;
	unsigned int ttl;
} CallbackItem_t;



void ModeResetCb(char *pLine);
void SetBanCb(char *pLine);
void GetBanCb(char *pLine);

#endif

