#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetInfo.h"
#include "UserMenu.h"
#include "saveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

char fileNameTemplate[100] = "snapshotsData_";


void saveToFile() {
	char saveDate[100];
	time_t date = time(NULL);
	struct tm tm = *localtime(&date);
	sprintf(saveDate, "%s%d.%d.%d", fileNameTemplate, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	strcat(saveDate, ".bin");
	header.version = 1;
	header.snapshotsCount = snapshotCounter;
	FILE* fp;
	fp = fopen(saveDate, "wb");

	if (fp == NULL) {
		printf("Error opening file");
		// write to log
		return;
	}
	 // write to log
	//int processesNum;
	snapshotsList* currentS = snapshotListHead;
	fwrite(&header, sizeof(snapList_Header), 1, fp);
	for (int i = 0; i < header.snapshotsCount; i++) {
		processinformation* currentP = currentS->snapshotData;
		//processesNum = currentP->loadedProcesses;
		//fwrite(&currentP->loadedProcesses, sizeof(unsigned int), 1, fp);
		process_header.loadedProcesses = processesCount(currentP);
		fwrite(&process_header, sizeof(processesList_Header), 1, fp);
		for(int i = 0; i < process_header.loadedProcesses; i++){
		fwrite(&currentP->snapshotCounter, sizeof(unsigned int), 1, fp);
		fwrite(&currentP->snapshotTime, sizeof(char) * 100, 1, fp);
		fwrite(&currentP->processID, sizeof(unsigned int), 1, fp);
		fwrite(&currentP->processName, sizeof(char) * 260, 1, fp);
		fwrite(&currentP->memoryinfo, sizeof(memoryinfo), 1, fp);
		fwrite(&currentP->totalLoadedDlls, sizeof(unsigned int), 1, fp);
		dllInfo* currentD = currentP->dllInfo;
		//int dllNumber = currentD->privateLoadedDlls;
		dlls_header.privateLoadedDlls = dllsCount(currentD);
		fwrite(&dlls_header, sizeof(dllsList_Header), 1, fp);
		//fwrite(&currentD->privateLoadedDlls, sizeof(unsigned int), 1, fp);
			for (int j = 0; j < dlls_header.privateLoadedDlls; j++) {
			//if (currentD->dllName == NULL) {
			//	break;
		//	}
			fwrite(&currentD->dllName, sizeof(char) * 260, 1, fp);
			currentD = currentD->next;
			}
		currentP = currentP->next;
		}
		currentS = currentS->nextSnap;
	}
}

void loadFromFile() {
	
	char saveDate[100];
	//printf("Enter the date of the Snapshots List you want to loadFromFile (dd.mm.yyyy): ");
	//scanf("%s", saveDate);
	//strcat(fileNameTemplate, saveDate);
	//strcat(fileNameTemplate, ".bin");
	strcpy(fileNameTemplate , "snapshotsData_5.10.2022.bin");
	FILE* fp;
	fp = fopen(fileNameTemplate, "rb");
	if (fp == NULL) {
		printf("Error opening file");
		// write to log
		return;
	}
	if (fread(&header, sizeof(snapList_Header), 1, fp) != 0) {
		snapshotListHead = NULL;
		for (int snapshotNumber = 0; snapshotNumber < header.snapshotsCount; snapshotNumber++)
		{
			processinformation* newSnap = NULL;
			//processinformation* currSnap = (processinformation*)malloc(sizeof(processinformation));
				//fread(&currSnap->loadedProcesses, sizeof(unsigned int), 1, fp);
			   // fread(&currSnap->snapshotCounter, sizeof(unsigned int), 1, fp);
				//fread(&currSnap->snapshotTime, sizeof(char) * 100, 1, fp);
				//fread(&currSnap->processID, sizeof(unsigned int), 1, fp);
				//fread(&currSnap->processName, sizeof(char) * 260, 1, fp);
			//	fread(&currSnap->memoryinfo, sizeof(memoryinfo), 1, fp);
			//	fread(&currSnap->totalLoadedDlls, sizeof(unsigned int), 1, fp);
				
				//dllInfo* currDll = (dllInfo*)malloc(sizeof(dllInfo));
				//fread(&currDll->privateLoadedDlls, sizeof(unsigned int), 1, fp);
				//for (int j = 0; j < currDll->privateLoadedDlls; j++) {
				//	fread(&currDll->dllName, sizeof(currDll->dllName), 1, fp);
				//	if (currDll->dllName == NULL) {
				//		break;
				//	}
			//		currSnap->dllInfo=addDllToList(currDll);
				//}
				
			//	addProcess(currSnap);
			//	int processesCount = currSnap->loadedProcesses - 1;
			if (fread(&process_header, sizeof(processesList_Header), 1, fp) != 0) {
				for (int i = 0; i < process_header.loadedProcesses; i++) {
					processinformation* currSnap = (processinformation*)malloc(sizeof(processinformation));
					//fread(&currSnap->loadedProcesses, sizeof(unsigned int), 1, fp);
					fread(&currSnap->snapshotCounter, sizeof(unsigned int), 1, fp);
					fread(&currSnap->snapshotTime, sizeof(char) * 100, 1, fp);
					fread(&currSnap->processID, sizeof(unsigned int), 1, fp);
					fread(&currSnap->processName, sizeof(char) * 260, 1, fp);
					fread(&currSnap->memoryinfo, sizeof(memoryinfo), 1, fp);
					fread(&currSnap->totalLoadedDlls, sizeof(unsigned int), 1, fp);
					dllInfo* currDll = (dllInfo*)malloc(sizeof(dllInfo));
					if (fread(&dlls_header, sizeof(dllsList_Header), 1, fp) != 0) {

						//fread(&currDll->privateLoadedDlls, sizeof(unsigned int), 1, fp);
						for (int j = 0; j < dlls_header.privateLoadedDlls; j++) {
							fread(&currDll->dllName, sizeof(currDll->dllName), 1, fp);
							//if (currDll->dllName == NULL) {
							//	break;
							//}
							currSnap->dllInfo = addDllToList(currDll);
						}
						dllListInit();
					}
					newSnap = addProcess(currSnap);
				}
			}
				addSnapshot(newSnap);
				listInit();
			}
			
		}
	}
	
	
