#include "MyUtilities.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include <time.h>
#pragma warning(disable:4996)

int dllCounter; 
processinformation* currProcess;
char strTime[100];

	
/// <summary>
/// Gets the memory information of a process and returns it
/// </summary>
 struct processinformation* GetMemoryInfo(DWORD processID)
{
	 processinformation* currentP;
	 currentP = (processinformation*)malloc(sizeof(processinformation));
	 memoryinfo currentM;
	 dllInfo* currentD;
	 HANDLE hProcess;
	
	PROCESS_MEMORY_COUNTERS pmc;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return NULL;

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR Buffer[MAX_PATH];
	TCHAR Buffer2[MAX_PATH];

	if (GetModuleFileNameEx(hProcess, 0, Buffer, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		
		// convert the process name to char
		char processName[MAX_PATH];
		size_t numConverted1;
		wcstombs_s(&numConverted1, processName, MAX_PATH, Buffer, MAX_PATH); 
		
		strcpy(currentP->processName, processName);
		currentP->processID = processID;
		currentP->snapshotCounter = snapshotCounter;
		
	}
	else
	{
		LogError(strerror(GetLastError()));
	}


	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
		currentM.PageFaultCount = pmc.PageFaultCount;
		currentM.WorkingSetSize = pmc.WorkingSetSize;
		currentM.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		currentM.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		currentM.PagefileUsage = pmc.PagefileUsage;
		currentP->memoryinfo = currentM; // add the memory info to the current process
	}


	// Get Dlls List

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		dllListInit(); // initialize the dll list
		
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];


			// Get the full path to the module's file.

			if (GetModuleFileNameEx(hProcess, hMods[i], Buffer2, MAX_PATH))
			{
				dllCounter++; 
				currentD = (dllInfo*)malloc(sizeof(dllInfo));
				
				// convert the dll name to char
				char dllName[MAX_PATH];
				size_t numConverted;
				wcstombs_s(&numConverted, dllName, MAX_PATH, Buffer2, MAX_PATH); 
				
				strcpy(currentD->dllName, dllName);
				currentP->dllInfo = addDllToList(currentD); // add the dll to the dll list
			}
		}
			
		currentP->totalLoadedDlls = dllCounter; // add the number of dlls to the current process
		dllCounter = 0; // reset the dll counter
	}

	// Get the snapshot time by format hh:mm:ss and add it to the current process
	time_t snapT;
	time(&snapT);
	struct tm* timeinfo;
	timeinfo = localtime(&snapT);
	sprintf(strTime, "%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	strcpy(currentP->snapshotTime, strTime);
	
		CloseHandle(hProcess);
		return currentP; // return the current process
	};
	
 /// <summary>
	/// Gets the processes information and returns the processes list head
	/// </summary>
 struct processinformation* GetProcessesInfo()
{
	 processinformation* HeadOfProcessList = NULL; 
	// Get Processes
	// Receive all process ID
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		LogError(strerror(GetLastError()));
		return 1;
	}

	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);
	
	for (int i = 0; i < cProcesses; i++)  // for each process
	{ 
		currProcess = GetMemoryInfo(aProcesses[i]);
		if (currProcess != NULL) { // if the process is not null
			HeadOfProcessList = addProcess(currProcess); // add the process to the processes list
		}
	}
	return HeadOfProcessList; // return the processes list head
}
