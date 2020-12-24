#include "Submenu.h"
#include "includes.h"


const MENUITEMS submenuItems = {
//   title
LABEL_SUBMENU,
// icon                       label
  {{ICON_HEAT_FAN,    LABEL_UNIFIEDHEAT},
    {ICON_HOME_MOVE,  LABEL_UNIFIEDMOVE},
    {ICON_EXTRUDE,    LABEL_EXTRUDE},
    {ICON_STOP,       LABEL_EMERGENCYSTOP},
    {ICON_GCODE,      LABEL_TERMINAL},
    {ICON_CUSTOM,     LABEL_CUSTOM},
    {ICON_SETTINGS,   LABEL_SETTINGS},
    {ICON_BACK,       LABEL_BACK}}
  };

void menuSubmenu(void)
{
  KEY_VALUES key_num = KEY_IDLE;
  menuDrawPage(&submenuItems);
  while(infoMenu.menu[infoMenu.cur] == menuSubmenu)
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: infoMenu.menu[++infoMenu.cur] = menuUnifiedHeat;     break;
      case KEY_ICON_1: infoMenu.menu[++infoMenu.cur] = menuUnifiedMove;     break;
      case KEY_ICON_2: infoMenu.menu[++infoMenu.cur] = menuExtrude;         break;
      case KEY_ICON_3: storeCmd("M112\n"); break;     // Emergency Stop : Used for emergency stopping, a reset is required to return to operational mode.
                                                      // it may need to wait for a space to open up in the command queue.
                                                      // Enable EMERGENCY_PARSER in Marlin Firmware for an instantaneous M112 command.
      case KEY_ICON_4: infoMenu.menu[++infoMenu.cur] = menuSendGcode;       break;
      case KEY_ICON_5: infoMenu.menu[++infoMenu.cur] = menuCustom;          break;
      case KEY_ICON_6: infoMenu.menu[++infoMenu.cur] = menuSettings;        break;
      case KEY_ICON_7: infoMenu.cur--;        break;
      default:break;
    }
    loopProcess();
  }
}

