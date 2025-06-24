#pragma once
#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t (*GroundBtnRead)(void);

typedef struct ConfButton {
    uint32_t listenRange;
    uint32_t countWithoutChange;
		uint8_t currStableState;
    uint8_t currInstantState;
    uint8_t prevStableState;
    uint8_t prevInstantState;
		GroundBtnRead groundBtnRead;
}ConfButton;

ConfButton InitButton(GroundBtnRead groundBtnRead);
void HandlerButton(ConfButton* button);
uint8_t PushButtListnr(ConfButton* button);

