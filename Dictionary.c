#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "GenerateHTML.h"
#include "GenerateHomePage.h"
#include "HtmlDataProcessor.h"
#include "Dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)


processDict* procDictTail = NULL;
dllDict* dllDictTail = NULL;


struct processDict* addProcessToUsedBy(processDict* usedByHead, processinformation* processToAdd) {
	processDict* processDictToAdd = (processDict*)malloc(sizeof(processDict));
	strcpy(processDictToAdd->processName, processToAdd->processName);
	processDictToAdd->processID = processToAdd->processID;
	processDict* findLast;
	findLast = usedByHead;
	while (findLast != NULL)
	{
		if (strcmp(findLast->processName, processDictToAdd->processName) == 0 && findLast->processID == processDictToAdd->processID) {
			free(processDictToAdd);
			return usedByHead;
		}
		if (findLast->next == NULL) {
			break;
		}
		findLast = findLast->next;
	}
	findLast->next = processDictToAdd;
	processDictToAdd->prev = findLast;
	processDictToAdd->next = NULL;
	return usedByHead;
}

void addProcToDict(char processName[MAX_PATH], unsigned int processID) {
	processDict* curr = malloc(sizeof(processDict));
	strcpy(curr->processName, processName);
	curr->processID = processID;
	if (procDictHead == NULL) {
		curr->next = NULL;
		curr->prev = NULL;
		procDictHead = curr;
		procDictTail = curr;
	}
	else {
		procDictTail->next = curr;
		curr->prev = procDictTail;
		procDictTail = curr;
	}
	procDictTail->next = NULL;
	PCounter++;
}

void addDllToDict(char dllName[MAX_PATH], processinformation* proc) {
	processDict* processDictToAdd = (processDict*)malloc(sizeof(processDict));
	strcpy(processDictToAdd->processName, proc->processName);
	processDictToAdd->processID = proc->processID;
	processDictToAdd->next = NULL;
	processDictToAdd->prev = NULL;
	dllDict* curr = malloc(sizeof(dllDict));
	strcpy(curr->dllName, dllName);
	curr->usedByCount = 1;
	curr->usedBy = processDictToAdd;
	if (dllDictHead == NULL) {
		curr->next = NULL;
		curr->prev = NULL;
		dllDictHead = curr;
		dllDictTail = curr;
	}
	else {
		dllDictTail->next = curr;
		curr->prev = dllDictTail;
		dllDictTail = curr;
	}
	dllDictTail->next = NULL;
	DCounter++;
}

void printProcDictionary() {
	 processDict* curr = procDictHead;
	while (curr != NULL) {
		printf("Process Name: %s\t Process ID: %d\n", curr->processName, curr->processID);
		curr = curr->next;
	}
}

void printDllDictionary() {
	dllDict* curr = dllDictHead;
	while (curr != NULL) {
		printf("Dll Name: %s", curr->dllName);
		curr = curr->next;
	}
}

struct processDict* searchProcess(char processName[MAX_PATH], unsigned int processID) {
	processDict* curr = procDictHead;
	while (curr != NULL) {
		if (strcmp(curr->processName, processName) == 0 && curr->processID == processID) {
			
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

struct dllDict* searchDll(char dllName[MAX_PATH],processinformation* processToAdd) {
	dllDict* curr = dllDictHead;
	while (curr != NULL) {
		if (strcmp(curr->dllName, dllName) == 0) {
			if (processToAdd != NULL) {
			curr->usedByCount++;
			curr->usedBy = addProcessToUsedBy(curr->usedBy, processToAdd);
			}
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}