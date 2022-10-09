#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include "ProcessesLinkedList.h"
#include <psapi.h>
#include "GetProcessesInfo.h"
#pragma warning(disable:4996)

snapshotsList* snapshotListTail = NULL;
processinformation* processesHead = NULL; 
processinformation* processesTail = NULL;
dllInfo* headDll = NULL;
dllInfo* tailDll = NULL;

/// <summary>
/// Adds a process to the linked list
/// returns the head of the list
/// </summary>
struct processinformation* addProcess(processinformation* current)
{
    processinformation* currentProcess; 
	currentProcess = (processinformation*)malloc(sizeof(processinformation)); // allocate memory for the new process
    strcpy(currentProcess->processName,current->processName); 
	currentProcess->processID = current->processID;
	currentProcess->memoryinfo = current->memoryinfo;
	currentProcess->dllInfo = current->dllInfo;
	currentProcess->totalLoadedDlls = current->totalLoadedDlls;
	currentProcess->snapshotCounter = current->snapshotCounter;
    strcpy(currentProcess->snapshotTime, current->snapshotTime);
	
    
    if (processesHead == NULL) // if the list is empty
    {
        processesHead = currentProcess;
        processesTail = currentProcess;
        currentProcess->prev = NULL;
		currentProcess->next = NULL;
   }
    else
   {
		processesTail->next = currentProcess;
		currentProcess->prev = processesTail;
		processesTail = currentProcess;
        
    }
    processesTail->next = NULL;

	return processesHead;
}

/// <summary>
	/// Adds an extra process to the end of the linked list
	/// </summary>
	/// <param name="destList"></param>
	/// <param name="addToList"></param>
	/// <returns>Updated list</returns>
struct processinformation* addExtraProcess(processinformation* destList, processinformation* addToList) {
	
	if (addToList->processID == 3452816845) { // A process that has no name(system idle process)
		return destList;
	}
	processinformation* findLast;
	findLast = destList;
	while (findLast->next != NULL)
	{
		findLast = findLast->next;
	}
	findLast->next = addToList;
	addToList->prev = findLast;
	addToList->next = NULL;
	return destList;
}

/// <summary>
	/// Checks if the dll is already in the list and if not adds it to the end of the list 
	/// </summary>
	/// <param name="destDll"></param>
	/// <param name="dllToAdd"></param>
	/// <returns>Updated dlls list</returns>
struct dllInfo* extraDllsCheck(dllInfo* destDll, dllInfo* dllToAdd) {
	int foundIdenticalDll;
	dllInfo* destDllRunner; 
	dllInfo* dllToAddRunner = dllToAdd;  
	while (dllToAddRunner != NULL) { // while there are dlls from the current list to check
		foundIdenticalDll = 0;
		destDllRunner = destDll; // reset the runner to the start of the list
		while (destDllRunner != NULL) { // while there are dlls in the dest list to check

			if (strcmp(destDllRunner->dllName, dllToAddRunner->dllName) == 0) { 
				foundIdenticalDll = 1;
				break;
			}
			destDllRunner = destDllRunner->next;
		}
		if (foundIdenticalDll == 0) { // if the dll is not in the list add it
			destDll = addExtraDlls(destDll, dllToAddRunner); 
			
		}
		dllToAddRunner = dllToAddRunner->next;
	}
	return destDll; // return the updated list
}

/// <summary>
/// Adds an extra dll to the end of the linked list
/// </summary>
struct dllInfo* addExtraDlls(dllInfo* destDlls, dllInfo* dllToAdd) {
	
	dllInfo* findLast;
	findLast = destDlls;
	while (findLast->next != NULL)
	{
		findLast = findLast->next;
	}
	findLast->next = dllToAdd;
	dllToAdd->prev = findLast;
	dllToAdd->next = NULL;
	return destDlls;
}

/// <summary>
   /// Adds a dll to the linked list and returns the head of the list
   /// </summary>
struct dllInfo* addDllToList(dllInfo* current)
{
    dllInfo* currentDll;
    currentDll = (dllInfo*)malloc(sizeof(dllInfo));
    strcpy(currentDll->dllName, current->dllName);

    if (headDll == NULL) // if the list is empty
    {
        headDll = currentDll;
        tailDll = currentDll;
        currentDll->prev = NULL;
		currentDll->next = NULL;
    }
    else
    {
        currentDll->prev = tailDll;
        tailDll->next = currentDll;
        tailDll = currentDll;
       
    }
    tailDll->next = NULL;

	return headDll; // return the head of the list
}
/// <summary>
	   /// Adds a snapshot to the linked list
	   /// </summary>
	   /// <param name="currentSnapshot"></param>
void addSnapshot(struct processinformation* currentSnapshot) {
        snapshotsList* currentSnap;
        currentSnap = (snapshotsList*)malloc(sizeof(snapshotsList));
        currentSnap->snapshotData = currentSnapshot; 
        if (snapshotListHead == NULL) // if the list is empty
        {
            snapshotListHead = currentSnap;
            snapshotListTail = currentSnap;
            currentSnap->prevSnap = NULL;
            currentSnap->nextSnap = NULL;
        }
        else
        {
            snapshotListTail->nextSnap = currentSnap;
            currentSnap->prevSnap = snapshotListTail;
            snapshotListTail = currentSnap;
        }
        snapshotListTail->nextSnap = NULL;	
}


//Utility function
void printSnapshots() {
	snapshotsList* curr;
    processinformation* current;
	dllInfo* currD;
	curr = snapshotListHead;
	while (curr != NULL) {
		printf("Snapshot processesHead: %s", curr->snapshotData->processName);
		printf("Snapshot time: %s\n", curr->snapshotData->snapshotTime);
		printf("Snapshot counter: %d\n", curr->snapshotData->snapshotCounter);
        current = curr->snapshotData;
		while (current != NULL) {
			printf("Process name: %s\n", current->processName);
			printf("Process ID: %d\n", current->processID);
			printf("Process memory info: %d\n", current->memoryinfo.PageFaultCount);
			printf("Process memory info: %d\n", current->memoryinfo.WorkingSetSize);
			printf("Process memory info: %d\n", current->memoryinfo.QuotaPagedPoolUsage);
			printf("Process memory info: %d\n", current->memoryinfo.QuotaPeakPagedPoolUsage);
			printf("Process memory info: %d\n", current->memoryinfo.PagefileUsage);
			printf("Process loaded dlls: %d\n", current->totalLoadedDlls);
			printf("Process snapshot time: %s\n", current->snapshotTime);
			printf("Process snapshot counter: %d\n", current->snapshotCounter);
			currD = current->dllInfo;
			while (currD != NULL) {
				printf("Dll name: %s\n", currD->dllName);
				currD = currD->next;
			}
			current = current->next;
			
		}
		curr = curr->nextSnap;
	}
    printf("NULL");
}

/// <summary>
	/// Resets the snapshot collection
	/// </summary>
void resetSnapshotCollection() {
	snapshotsList* currSnap;
	snapshotsList* releaseSnap;
	processinformation* currentP;
	processinformation* releaseProcess;
	dllInfo* currD;
	dllInfo* releaseDll;
	
	currSnap = snapshotListHead;
	while (currSnap != NULL) {
		currentP = currSnap->snapshotData;
		while (currentP != NULL) {
			currD = currentP->dllInfo;
			while (currD != NULL) {
				releaseDll = currD;
				currD = currD->next;
				free(releaseDll);
			}
			releaseProcess = currentP;
			currentP = currentP->next;
			free(releaseProcess);
		}
		releaseSnap = currSnap;
		currSnap = currSnap->nextSnap;
		free(releaseSnap);
	}
	snapshotListHead = NULL; // reset the head of the list
	snapshotListTail = NULL; // reset the tail of the list
}

/// <summary>
		/// Initializes the processes list
		/// </summary>
void processesListInit() {
		
	
		processesHead = NULL;
		processesTail = NULL;
}

/// <summary>
		/// Initializes the dlls list
		/// </summary>
void dllListInit() {
	headDll = NULL;
	tailDll = NULL;
}