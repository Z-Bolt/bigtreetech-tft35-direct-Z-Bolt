#ifndef _CHANGE_H_
#define _CHANGE_H_


#include "stdint.h"
#include "stdbool.h"
#include "Configuration.h"
#include "Settings.h"

void menuChange(void);


void waitTill210(void);

void updateNextHeatCheckTime2(void);
void menuConfirmLoad(void);
void menuConfirmUnload(void);
void Load(void);
void Unload(void);

/*
bool heatHasWaiting2(void);

void loopCheckHeater2(void);
*/

#endif
