#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
//#include "saveAndLoad.h"
//#include "GenerateHTML.h"
//#include "GenerateHomePage.h"
//#include "HtmlDataProcessor.h"
#include "Dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)


processDict* procDictTail = NULL; // tail of the processes dictionary
dllDict* dllDictTail = NULL; // tail of the dlls dictionary

/// <summary>
	///  Adds a process to the used by list of a given DLL
	/// </summary>
	/// <param name="usedByHead"></param>
	/// <param name="processToAdd"></param>
	/// <returns>head of updated used by list</returns>
struct processDict* addProcessToUsedBy(processDict* usedByHead, processinformation* processToAdd) {
	
	// allocate memory for the new process dictionary && copy the relevant details
	processDict* processDictToAdd = (processDict*)malloc(sizeof(processDict));  
	strcpy(processDictToAdd->processName, processToAdd->processName); 
	processDictToAdd->processID = processToAdd->processID;
	
	processDict* findLast;
	findLast = usedByHead;  
	while (findLast != NULL) // find the last process in the used by list
	{
		// if the process is already in the list, free the allocated memory and return the head of the original list
		if (strcmp(findLast->processName, processDictToAdd->processName) == 0 && findLast->processID == processDictToAdd->processID) { 
			free(processDictToAdd);  
			return usedByHead;
		}
		if (findLast->next == NULL) { // enable to check the last process in the list while searching for the last process.
			break;
		}
		findLast = findLast->next;
	}
	// add the new process to the end of the list
	findLast->next = processDictToAdd;
	processDictToAdd->prev = findLast;
	processDictToAdd->next = NULL;
	return usedByHead; // return the head of the updated list
}

/// <summary>
	///  Adds a process to the processes dictionary
	/// </summary>
	/// <param name="processName"></param>
	/// <param name="processID"></param>
void addProcToDict(char processName[MAX_PATH], unsigned int processID) {
	processDict* curr = malloc(sizeof(processDict));
	strcpy(curr->processName, processName);
	curr->processID = processID;
	
	if (procDictHead == NULL) { // if the dictionary is empty, add the process as the head of the dictionary
		curr->next = NULL;
		curr->prev = NULL;
		procDictHead = curr;
		procDictTail = curr;
	}
	else { // if the dictionary is not empty, add the process to the end of the dictionary
		procDictTail->next = curr;
		curr->prev = procDictTail;
		procDictTail = curr;
	}
	procDictTail->next = NULL;
	PCounter++; // increase the number of processes in the dictionary (unique processes)
}

/// <summary>
	///  Adds a DLL to the DLLs dictionary and initializes his usedBy list head
	/// </summary>
	/// <param name="dllName"></param>
	/// <param name="proc"></param>
void addDllToDict(char dllName[MAX_PATH], processinformation* parentProc) {
	// get the parent process which the dll runs in and
	// allocate memory for the new process after copy the relevant details 
	processDict* processDictToAdd = (processDict*)malloc(sizeof(processDict)); 
	strcpy(processDictToAdd->processName, parentProc->processName);
	processDictToAdd->processID = parentProc->processID;
	processDictToAdd->next = NULL;
	processDictToAdd->prev = NULL;
	
	dllDict* curr = malloc(sizeof(dllDict)); 
	strcpy(curr->dllName, dllName);
	curr->usedBy = processDictToAdd; // add the process to the used by list of the DLL
	
	if (dllDictHead == NULL) { // if the dictionary is empty, add the DLL as the head of the dictionary
		curr->next = NULL;
		curr->prev = NULL;
		dllDictHead = curr;
		dllDictTail = curr;
	}
	else { // if the dictionary is not empty, add the DLL as the tail of the dictionary
		dllDictTail->next = curr;
		curr->prev = dllDictTail;
		dllDictTail = curr;
	}
	dllDictTail->next = NULL;
	DCounter++; // increase the number of DLLs in the dictionary (unique DLLs)
}

/*void printProcDictionary() {

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
*/

/// <summary>
	///  Searches for a process in the processes dictionary
	/// returns the process if found, NULL otherwise
	/// </summary>
struct processDict* searchProcess(char processName[MAX_PATH], unsigned int processID) { 
	
	processDict* curr = procDictHead; 
	while (curr != NULL) {  
		if (strcmp(curr->processName, processName) == 0 && curr->processID == processID) { // if the process is found, return it
			
			return curr;
		}
		curr = curr->next;
	}
	return NULL; 
}

/// <summary>
	///  Searches for a DLL in the DLLs dictionary
	/// returns the DLL if found, NULL otherwise
	/// </summary>
struct dllDict* searchDll(char dllName[MAX_PATH],processinformation* parentProcess) {
	dllDict* curr = dllDictHead;
	while (curr != NULL) {
		if (strcmp(curr->dllName, dllName) == 0) { 
			if (parentProcess != NULL) { 
				curr->usedBy = addProcessToUsedBy(curr->usedBy, parentProcess); // if the DLL is found, add the process to the used by list of the DLL
			}
			return curr;
		}
		curr = curr->next;
	}
	return NULL; // if the DLL is not found, return NULL
}