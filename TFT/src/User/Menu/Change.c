#include "Change.h"
#include "Heat.h"
#include "includes.h"

bool do_change;
bool do_change_load;
bool do_finish_load;

static u32     update_time = TEMPERATURE_QUERY_SLOW_DURATION;
static bool    update_waiting = false;

//static HEATER  heater = {{}, NOZZLE0, NOZZLE0};

//1��title(����), ITEM_PER_PAGE��item(ͼ��+��ǩ)
const MENUITEMS changeItems = {
//   title
LABEL_CHANGE,
// icon                       label
 {{ICON_LOAD,                 LABEL_LOAD},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_UNLOAD,               LABEL_UNLOAD},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACK,                 LABEL_BACK},}
};

void menuChange(void)
{
  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&changeItems);
  while(infoMenu.menu[infoMenu.cur] == menuChange)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {

      case KEY_ICON_0: {//загрузка

            infoMenu.menu[++infoMenu.cur] = menuConfirmLoad;
          /// storeCmd("G1 E-700 F5000\n");
          /// storeCmd("G92 E0\n");
      break;
      }
        
      case KEY_ICON_3: {//выгрузка
      
            infoMenu.menu[++infoMenu.cur] = menuConfirmUnload;
           /*    
           storeCmd("G28 F6000\n");
           storeCmd("G90\n");
           mustStoreCmd("G1 X185 Y5 F6000\n");
           storeCmd("M82\n");
           storeCmd("G92 E0\n");
           heatSetTargetTemp(1, 210);
           do_change = true;
           loopProcess();
            */
      break;
      }
      case KEY_ICON_7: infoMenu.cur--;      break;
      default:break;
    }
    loopProcess();
  }
}

void Load()
{
           storeCmd("G28 X Y F9000\n");
           storeCmd("G28 Z F1500\n");
           storeCmd("G90\n");
           mustStoreCmd("G1 X5 Y5 F9000\n");
           storeCmd("M82\n");
           storeCmd("G92 E0\n");
           //storeCmd("M104 S210\n");
         
           //storeCmd("M109 S210\n");
           //WAIT TILL 210
           heatSetTargetTemp(1, 210);
           do_change_load = true;
        loopProcess();
}

void Unload()
{
        storeCmd("G28 X Y F9000\n");
        storeCmd("G28 Z F1500\n");
        storeCmd("G90\n");
        mustStoreCmd("G1 X5 Y5 F9000\n");
        storeCmd("M82\n");
        storeCmd("G92 E0\n");
        heatSetTargetTemp(1, 210);
        do_change = true;
        loopProcess();
}


void menuConfirmLoad(void)
{
  u16 key_num = IDLE_TOUCH;

  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_CONFIRM_LOAD), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while(infoMenu.menu[infoMenu.cur] == menuConfirmLoad)
  {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch(key_num)
    {
      case KEY_POPUP_CONFIRM:
        //abortPrinting();
        Load();
        infoMenu.cur--;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.cur--;
        statusScreen_setMsg(textSelect(LABEL_STATUS), textSelect(LABEL_READY));
        break;
    }
    loopProcess();
  }
}

void menuConfirmUnload(void)
{
  u16 key_num = IDLE_TOUCH;

  popupDrawPage(bottomDoubleBtn, textSelect(LABEL_WARNING), textSelect(LABEL_CONFIRM_UNLOAD), textSelect(LABEL_CONFIRM), textSelect(LABEL_CANCEL));

  while(infoMenu.menu[infoMenu.cur] == menuConfirmUnload)
  {
    key_num = KEY_GetValue(2, doubleBtnRect);
    switch(key_num)
    {
      case KEY_POPUP_CONFIRM:
        //abortPrinting();
        Unload();
        infoMenu.cur--;
        break;

      case KEY_POPUP_CANCEL:
        infoMenu.cur--;
        statusScreen_setMsg(textSelect(LABEL_STATUS), textSelect(LABEL_READY));
        break;
    }
    loopProcess();
  }
}


/*
u32 nextHeatCheckTime2 = 0;
void updateNextHeatCheckTime2(void)
{
  nextHeatCheckTime2 = OS_GetTimeMs() + update_time;
}*/


/*

    if(heatGetCurrentTemp(1) > 210 - 1)
    {
      storeCmd("G1 E-700 F5000\n");
      storeCmd("G92 E0\n");

      break;
    }

*/

/*
//Check all heater if there is a heater waiting to be waited 
bool heatHasWaiting2(void)
{
 
  TOOL i;
  for(i = BED; i < (infoSettings.tool_count + 1); i++)
  {
    if(heater.T[i].waiting != WAIT_NONE)
      return true;
  }
  return false;
  
}


u32 nextHeatCheckTime2 = 0;
void updateNextHeatCheckTime2(void)
{
  nextHeatCheckTime2 = OS_GetTimeMs() + update_time;
}


void loopCheckHeater2(void)
{
  do
  {  // Send M105 query temperature continuously  
    if(update_waiting == true)                {updateNextHeatCheckTime();break;}
    if(OS_GetTimeMs() < nextHeatCheckTime2)     break;
    if(RequestCommandInfoIsRunning())          break; //to avoid colision in Gcode response processing
    if(storeCmd("M105\n") == false)            break;
    updateNextHeatCheckTime();
    update_waiting = true;
  }while(0);

  // Query the heater that needs to wait for the temperature to rise, whether it reaches the set temperature 
  for(u8 i=0; i< (infoSettings.tool_count + 1); i++)
  {
    if (heater.T[i].waiting == WAIT_NONE)                              continue;
    else if (heater.T[i].waiting == WAIT_HEATING) {
      if (heater.T[i].current+2 <= heater.T[i].target)                 continue;
    }
    else if (heater.T[i].waiting == WAIT_COOLING_HEATING) {
      if (inRange(heater.T[i].current, heater.T[i].target, 2) != true) continue;
    }

    heater.T[i].waiting = WAIT_NONE;
    if (heatHasWaiting()) continue;

    //if(infoMenu.menu[infoMenu.cur] == menuHeat) break;
    update_time = TEMPERATURE_QUERY_SLOW_DURATION;
  }
}*/


