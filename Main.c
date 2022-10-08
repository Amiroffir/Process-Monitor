#include "MyUtilities.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "UserMenu.h"
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)





int main() {
	// need to change to time stamp log
	LogInit("main.log");
	LogEvent("Log file initialized\n");
	LogEvent("Program start running\n");
	userMenu();
	
};


