//
// Created by droc101 on 6/19/25.
//

#ifndef PLATFORM_CONSOLE_H
#define PLATFORM_CONSOLE_H

#include <stdbool.h>

// Uncomment to allow terminals that do not meet requirements (at least 10x10, 24bit color) to run.
//#define ALLOW_INVALID_TERMINALS

bool consoleInit(const char *flag);

void consoleLoop();

void consoleCleanup();

#endif //PLATFORM_CONSOLE_H
