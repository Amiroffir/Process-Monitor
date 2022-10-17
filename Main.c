#include "MyUtilities.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "UserMenu.h"
//#include "SnapshotsTypes.h"
//#include "GetProcessesInfo.h"
//#include "HtmlDataProcessor.h"
//#include "saveAndLoad.h"
//#include "ProcessesLinkedList.h"
#include <stdlib.h>
//#include <windows.h>
#pragma warning(disable:4996)




int main() {
	// a time stamp
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char logName[100]; // a string to hold the time stamp
	// format the time stamp
	sprintf(logName, "%s%d.%d.%d.%s","Logs\\",tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, "log");
	LogInit(logName);
	LogEvent("Log file initialized\n");
	LogEvent("Program start running\n");
	userMenu();
	
};


