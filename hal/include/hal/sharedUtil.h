#ifndef _SHAREDUTIL_H_
#define _SHAREDUTIL_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <stdbool.h>

void runCommand(char *command);
long long getTimeInMs(void);
void sleepForMs(long long delayInMs);

#endif