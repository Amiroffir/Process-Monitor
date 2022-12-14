#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include <time.h>
#include "Dictionary.h"
#include "saveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

char fileNameTemplate[120] = "Saved-Monitoring-Files\\snapshotsData_";

/// <summary>
	/// Counts the number of dlls in a given process
/// </summary>
/// <param name="dllHead"></param>
/// <returns>Number of DLL's</returns>
int dllsCount(dllInfo* dllHead) {
	dllInfo* currentD;
	currentD = dllHead;
	int dllCount = 0;
	while (currentD != NULL)
	{
		dllCount++;
		currentD = currentD->next;
	}
	return  dllCount;
}

/// <summary>
	/// Counts the number of processes in a given snapshot
/// </summary>
/// <param name="snapHead"></param>
/// <returns>Number of processes</returns>
unsigned long processesCount(processInformation* snapHead) {
	processInformation* current;
	current = snapHead;
	unsigned int procesessCount = 0;
	while (current != NULL)
	{
		procesessCount++;
		current = current->next;
	}
	
	return procesessCount;
}

/// <summary>
	/// Saves the snapshots list to a bin file
	/// </summary>
void saveToFile() {
	
	// Get the current date and add to the file name template
	char saveDate[120]; 
	time_t date = time(NULL); 
	struct tm tm = *localtime(&date); 
	sprintf(saveDate, "%s%d.%d.%d", fileNameTemplate, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	strcat(saveDate, ".bin");
	
	// Get the snapshots list details to the header
	header.version = 1; 
	header.snapshotsCount = snapshotCounter;  
	
	FILE* fp;
	fp = fopen(saveDate, "wb"); // Open the file to write
	if (fp == NULL) {
		printf("Error opening file\n");
		LogError(strcat(strerror(GetLastError()),"\n"));
		return;
	}
	LogEvent("File opened successfully for writing\n");

	snapshotsList* currentS = snapshotListHead; // Get the head of the snapshots list
	fwrite(&header, sizeof(snapList_Header), 1, fp); // Write the header to the file
	
	// For each snapshot in the list, write the snapshot details to the file
	for (int i = 0; i < header.snapshotsCount; i++) {
		
		processInformation* currentP = currentS->snapshotData; // Get the head of the processes list of a given snapshot
		process_header.loadedProcesses = processesCount(currentP); // Get the number of processes in the snapshot
		fwrite(&process_header, sizeof(processesList_Header), 1, fp); // Write the processes list header to the file
		
		for(int i = 0; i < process_header.loadedProcesses; i++){ 
		fwrite(&currentP->snapshotCounter, sizeof(unsigned int), 1, fp);
		fwrite(&currentP->snapshotTime, sizeof(char) * 100, 1, fp);
		fwrite(&currentP->processID, sizeof(unsigned int), 1, fp);
		fwrite(&currentP->processName, sizeof(char) * 260, 1, fp);
		fwrite(&currentP->memoryinfo, sizeof(memoryinfo), 1, fp);
		fwrite(&currentP->totalLoadedDlls, sizeof(unsigned int), 1, fp);
		
		dllInfo* currentD = currentP->dllInfo; 
		dlls_header.privateLoadedDlls = dllsCount(currentD); 
		fwrite(&dlls_header, sizeof(dllsList_Header), 1, fp);
		
			for (int j = 0; j < dlls_header.privateLoadedDlls; j++) {
			fwrite(&currentD->dllName, sizeof(char) * 260, 1, fp);
			currentD = currentD->next;
			}
		currentP = currentP->next;
		}
		currentS = currentS->nextSnap;
	}
}

/// <summary>
	/// Loads the snapshots list from a bin file
	/// </summary>
void loadFromFile() {
	
	// Get the user desired file name
	char saveDate[120];
	printf("Enter the date of the Snapshots List you want to load (dd.mm.yyyy): ");
	scanf("%s", saveDate);
	strcat(fileNameTemplate, saveDate);
	strcat(fileNameTemplate, ".bin");
	
	// Open the file to read
	FILE* fp;
	fp = fopen(fileNameTemplate, "rb");
	if (fp == NULL) {
		printf("Error opening file\n");
		printf("%s\n", strerror(errno));
		LogError(strcat(strerror(GetLastError()),"\n"));
		return;
	}
	LogEvent("File opened successfully for reading\n");
	
	if (fread(&header, sizeof(snapList_Header), 1, fp) != 0) { // Read the header from the file
		snapshotListHead = NULL; // Initialize the snapshots list head
		
		for (int snapshotNumber = 0; snapshotNumber < header.snapshotsCount; snapshotNumber++)  // for each snapshot in the file
		{  
			processInformation* newSnap = NULL; // Initialize the processes list head
			
			if (fread(&process_header, sizeof(processesList_Header), 1, fp) != 0) { // Read the processes list header from the file
				
				for (int i = 0; i < process_header.loadedProcesses; i++) { // for each process in the snapshot
					processInformation* currSnap = (processInformation*)malloc(sizeof(processInformation));
					fread(&currSnap->snapshotCounter, sizeof(unsigned int), 1, fp);
					fread(&currSnap->snapshotTime, sizeof(char) * 100, 1, fp);
					fread(&currSnap->processID, sizeof(unsigned int), 1, fp);
					fread(&currSnap->processName, sizeof(char) * 260, 1, fp);
					fread(&currSnap->memoryinfo, sizeof(memoryinfo), 1, fp);
					fread(&currSnap->totalLoadedDlls, sizeof(unsigned int), 1, fp);
					
					dllInfo* currDll = (dllInfo*)malloc(sizeof(dllInfo));
					if (fread(&dlls_header, sizeof(dllsList_Header), 1, fp) != 0) { // Read the dlls list header from the file

						for (int j = 0; j < dlls_header.privateLoadedDlls; j++) { // for each dll in the process
							fread(&currDll->dllName, sizeof(currDll->dllName), 1, fp);
							
							// If the dll is not in the dictionary, add it
							if (searchDll(currDll->dllName, currSnap) == NULL) {
								addDllToDict(currDll->dllName, currSnap);
							}
							currSnap->dllInfo = addDllToList(currDll); // Add the dll to the dlls list
						}
						dllListInit(); // Initialize the dlls list head
					}
					// if the process is not in the dictionary, add it
					if (searchProcess(currSnap->processName, currSnap->processID) == NULL) {
						addProcToDict(currSnap->processName, currSnap->processID);
					}//
					newSnap = addProcess(currSnap); // Add the process to the processes list
				}
			}
			addSnapshot(newSnap); // Add the snapshot to the snapshots list
			processesListInit(); // Initialize the processes list head
		}
	}
}
	
	
