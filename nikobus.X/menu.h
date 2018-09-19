/* 
 * File:   menu.h
 * Author: ron
 *
 * Created on 21 april 2013, 11:26
 */

#ifndef MENU_H
#define	MENU_H

#include "main.h"

struct MenuStatus {
    uint8_t currentItem;
    struct Awning *selectedAwning;
    uint8_t selected;

    uint8_t *changeValue;
    uint8_t maxValue;
    uint8_t minValue;
    bool overFlow;
};

void turnRight(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus);
void turnLeft(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus);
void pressButton(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus);
void updateStatus(struct SystemStatus *systemStatus, struct MenuStatus *menuStatus);

#endif	/* MENU_H */

