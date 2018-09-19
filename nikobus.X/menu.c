#include "menu.h"

#define STATUS_SCREEN  0x00
#define SCREEN_MASK    0b1111 // Get the screen number without the AWNING / MAIN identification bit.
#define IS_MAIN_SCREEN 0b10000
#define AWNING_SCREEN  0x10
#define WINTER_SCREEN  0x11
#define ALARM_SCREEN   0x12
#define DN_SCREEN      0x13
#define BL_SCREEN      0x14
#define EXIT_SCREEN    0x15

#define IS_AWNING_SCREEN        0b100000 //Bit mask to detect awning screens.
#define AWNING_ADDRESS          0x20
#define AWNING_UP_WIND_MAX      0x21
#define AWNING_UP_WIND_AVG      0x22
#define AWNING_UP_WIND_DEL      0x23
#define AWNING_UP_SUN_AVG       0x24
#define AWNING_UP_SUN_DEL       0x25
#define AWNING_DOWN_WIND_MAX    0x26
#define AWNING_DOWN_WIND_AVG    0x27
#define AWNING_DOWN_WIND_DEL    0x28
#define AWNING_DOWN_SUN_AVG     0x29
#define AWNING_DOWN_SUN_DEL     0x2A
#define AWNING_WINTER_ACTIVE    0x2B
#define AWNING_EXIT             0x2C


const char menuString[] =
        "Awning\0"
        "Winter\0"
        "Alarm\0"
        "Day and Night\0"
        "Backlight\0"
        "Exit\0";


void modifyChangeValue(struct MenuStatus *menuStatus, bool increase) {
    if (increase) {
        if (*menuStatus->changeValue >= menuStatus->maxValue) {
            if (menuStatus->overFlow) {
                *menuStatus->changeValue = menuStatus->minValue;
            }
        } else {
            (*menuStatus->changeValue)++;
        }

    } else {
        if (*menuStatus->changeValue <= menuStatus->minValue) {
            if (menuStatus->overFlow) {
                *menuStatus->changeValue = menuStatus->maxValue;
            }
        } else {
            (*menuStatus->changeValue)--;
        }
    }
}
//Public methods

void controlBacklight(struct SystemStatus *systemStatus) {
    systemStatus->backlightOn = 0xff;
}

void enterMainMenu(struct MenuStatus *menuStatus) {
    menuStatus->changeValue = &menuStatus->currentItem;
    menuStatus->minValue = 0x10;
    menuStatus->maxValue = 0x15;
    menuStatus->overFlow = true;
}

void enterAwningMenu(struct MenuStatus *menuStatus) {
    menuStatus->changeValue = &menuStatus->currentItem;
    menuStatus->minValue = 0x20;
    menuStatus->maxValue = 0x2C;
    menuStatus->overFlow = true;
}

void updateCurrentScreen(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {
    if(menuStatus->currentItem == STATUS_SCREEN)
    {
        updateStatus(systemStatus,menuStatus);
    }
    else if (menuStatus->selected) {
        clearLCD(); //Clears LCD put cursor back to 0x00
        switch (menuStatus->currentItem) {
            case AWNING_SCREEN:
                showConfigureAwning(systemStatus, menuStatus);
                break;
            case WINTER_SCREEN:
                showConfigureWinter(systemStatus,menuStatus);
                break;
            case ALARM_SCREEN:
                break;
            case DN_SCREEN:
                break;
            case BL_SCREEN:
                showConfigureBacklight(systemStatus, menuStatus);
                break;
            case EXIT_SCREEN:
                break;
        }
    } else if (menuStatus->currentItem & IS_MAIN_SCREEN) {
        showMainMenu(menuStatus);
    } else if (menuStatus->currentItem & IS_AWNING_SCREEN) {
        showAwningMenu(menuStatus);
    }
}

void pressButtonAwningMenu(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {

}

void pressButtonMainMenu(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {
    if (menuStatus->selected) {
        menuStatus->selected = false;
        saveConfiguration();
        enterMainMenu(menuStatus);

    } else {
        menuStatus->selected = true;

        switch (menuStatus->currentItem) {
            case AWNING_SCREEN:
                break;
            case WINTER_SCREEN:
                menuStatus->changeValue = &systemStatus->winter;
                menuStatus->minValue=0;
                menuStatus->maxValue=3;
                menuStatus->overFlow = true;
                break;
            case ALARM_SCREEN:
                break;
            case DN_SCREEN:
                break;
            case BL_SCREEN:
                menuStatus->changeValue = &systemStatus->desiredBacklight;
                menuStatus->minValue = 0;
                menuStatus->maxValue = 15;
                menuStatus->overFlow = false;

                break;
            case EXIT_SCREEN:
                menuStatus->selected = false;
                menuStatus->currentItem = STATUS_SCREEN;
                break;
        }
    }
}

void turnRight(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {
    controlBacklight(systemStatus);

    if (menuStatus->currentItem == STATUS_SCREEN) {
        menuStatus->currentItem = AWNING_SCREEN;
        enterMainMenu(menuStatus);
    } else {
        modifyChangeValue(menuStatus, true);
    }
    updateCurrentScreen(systemStatus, menuStatus);
}

void turnLeft(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {
    controlBacklight(systemStatus);
    if (menuStatus->currentItem == STATUS_SCREEN) {
        menuStatus->currentItem = EXIT_SCREEN;
        enterMainMenu(menuStatus);
    } else {
        modifyChangeValue(menuStatus, false);
    }
    updateCurrentScreen(systemStatus, menuStatus);

}

void pressButton(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {
    controlBacklight(systemStatus);
    if (menuStatus->currentItem & IS_MAIN_SCREEN) {
        pressButtonMainMenu(systemStatus, menuStatus);
    } else if (menuStatus->currentItem & IS_AWNING_SCREEN) {
        pressButtonAwningMenu(systemStatus, menuStatus);
    }

    updateCurrentScreen(systemStatus, menuStatus);
}

void updateStatus(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus) {
    if (menuStatus->currentItem == STATUS_SCREEN) {
        char textBuffer[17];
        sprintf(textBuffer, statusString1, systemStatus->windAvg);
        setDDRamAddr(LINE1);
        putsXLCD(textBuffer);
        sprintf(textBuffer, statusString2, systemStatus->sun, systemStatus->day, systemStatus->alarm, systemStatus->east.status, systemStatus->south.status, systemStatus->west.status);
        setDDRamAddr(LINE2);
        putsXLCD(textBuffer);
    }
}