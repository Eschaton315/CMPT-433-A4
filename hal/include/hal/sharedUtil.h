#ifndef _SHAREDUTIL_H_
#define _SHAREDUTIL_H_

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>

void runCommand(char *command);
long long getTimeInMs(void);
void sleepForMs(long long delayInMs);
void EchoToFile(char* filePath, char* contents);
#endif