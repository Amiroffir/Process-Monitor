#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include "ProcessesLinkedList.h"
#include <psapi.h>
#include "GetInfo.h"
#pragma warning(disable:4996)
//snapshotListHead = NULL;
snapshotsList* snapshotListTail = NULL;
processinformation* head = NULL;
processinformation* tail = NULL;
dllInfo* headDll = NULL;
dllInfo* tailDll = NULL;

struct processinformation* addProcess(processinformation* current)
{
  
    processinformation* currentProcess;
    currentProcess = (processinformation*)malloc(sizeof(processinformation));
	currentProcess->loadedProcesses = current->loadedProcesses;
    strcpy(currentProcess->processName,current->processName);
	currentProcess->processID = current->processID;
	currentProcess->memoryinfo = current->memoryinfo;
	currentProcess->dllInfo = current->dllInfo;
	currentProcess->totalLoadedDlls = current->totalLoadedDlls;
	currentProcess->snapshotCounter = current->snapshotCounter;
    strcpy(currentProcess->snapshotTime, current->snapshotTime);
	
    
    if (head == NULL)
    {
        head = currentProcess;
        tail = currentProcess;
        currentProcess->prev = NULL;
		currentProcess->next = NULL;
   }
    else
   {
		tail->next = currentProcess;
		currentProcess->prev = tail;
		tail = currentProcess;
        
    }
    tail->next = NULL;

	return head;
}

struct processinformation* addExtraProcess(processinformation* destList, processinformation* addToList) {
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

struct dllInfo* extraDllsCheck(dllInfo* destDll, dllInfo* dllToAdd) {
	int foundIdenticalDll;
	dllInfo* destDllRunner;
	dllInfo* dllToAddRunner = dllToAdd;
	while (dllToAddRunner != NULL) {
		foundIdenticalDll = 0;
		destDllRunner = destDll;
		while (destDllRunner != NULL) {

			if (strcmp(destDllRunner->dllName, dllToAddRunner->dllName) == 0) {
				foundIdenticalDll = 1;
				break;
			}
			destDllRunner = destDllRunner->next;
		}
		if (foundIdenticalDll == 0) {
			destDll = addExtraDlls(destDll, dllToAddRunner);
			destDll->privateLoadedDlls++;
		}
		dllToAddRunner = dllToAddRunner->next;
	}
	return destDll;
}

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

struct dllInfo* addDllToList(dllInfo* current)
{
    dllInfo* currentDll;
    currentDll = (dllInfo*)malloc(sizeof(dllInfo));
    strcpy(currentDll->dllName, current->dllName);

    if (headDll == NULL)
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

        return headDll;
}

void addSnapshot(struct processinformation* currentSnapshot) {
    {
        snapshotsList* currentSnap;
        currentSnap = (snapshotsList*)malloc(sizeof(snapshotsList));
        currentSnap->snapshotData = currentSnapshot;
        if (snapshotListHead == NULL)
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
}


//Utility function
void printSnapshots() {
	snapshotsList* curr;
    processinformation* current;
	dllInfo* currD;
	curr = snapshotListHead;
	while (curr != NULL) {
		printf("Snapshot head: %s", curr->snapshotData->processName);
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
//Utility function
void printProcessesList(processinformation* current)
{
		processinformation* curr;
		curr = current;
		while (curr != NULL) {
			printf("Process name: %s\n", curr->processName);
			printf("Process ID: %d\n", curr->processID);
			printf("Process memory: %d\n", curr->memoryinfo.PageFaultCount);
			printf("Process memory: %d\n", curr->memoryinfo.WorkingSetSize);
			printf("Process memory: %d\n", curr->memoryinfo.QuotaPagedPoolUsage);
			printf("Process memory: %d\n", curr->memoryinfo.PagefileUsage);
			printf("Process memory: %d\n", curr->memoryinfo.QuotaPeakPagedPoolUsage);
			
			curr = curr->next;
		}
	}

void resetSnapshotCollection() {
	snapshotsList* curr;
	snapshotsList* release;
	curr = snapshotListHead;
	while (curr != NULL) {
		release = curr;
		curr = curr->nextSnap;
		free(release);
	}
	snapshotListHead = NULL;
	snapshotListTail = NULL;
}

void listInit() {
	
		head = NULL;
		tail = NULL;
}

void dllListInit() {
	headDll = NULL;
	tailDll = NULL;
}