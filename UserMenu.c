#include "UserMenu.h"
#include "GetInfo.h"
#include "ProcessesLinkedList.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

processinformation* tempSnapHead;
processinformation* oneSnapHead;
processinformation* twentySnapHead;
 processinformation* longSnapHead;

 struct processinformation* addExtraDlls(dllInfo* destDlls, dllInfo* dllToAdd) {
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
 void dllsCount(processinformation* snapHead) {
	 processinformation* currentP;
	 currentP = snapHead;
	 dllInfo* currentD;
	 int dllCount = 0;
	 while (currentP != NULL)
	 {
		 currentD = currentP->dllInfo;
		 while (currentD != NULL)
		 {
			 dllCount++;
			 currentD = currentD->next;
		 }
		 currentP = currentP->next;
	 }
	 printf("Number of DLLs: %d\n", dllCount);
 }
 // Utility function
 void processesCount(processinformation* snapHead) {
	 
		 processinformation* current;
		 current = (processinformation*)malloc(sizeof(processinformation));
		 current = snapHead;
		 int i = 0;
		 while (current != NULL)
		 {
			 i++;
			 current = current->next;
		 }
		 printf("Number of processes: %d\n", i);
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
			 destDll =  addExtraDlls(destDll,dllToAddRunner);
		}
		 dllToAddRunner = dllToAddRunner->next;
	}
	 return destDll;
 }


struct processinformation* sumProcessesInfo(processinformation* destSnapshot, processinformation* processToAdd) {
	int foundIdenticalProcess;
	processinformation* destRunner;
	processinformation* processToAddRunner;
	processToAddRunner = processToAdd;
	while (processToAddRunner != NULL) {
		foundIdenticalProcess = 0;
		destRunner = destSnapshot;
		while (destRunner != NULL) {
			if ((strcmp(destRunner->processName,processToAddRunner->processName) == 0)&
				(destRunner->processID == processToAddRunner->processID)) {
				
				foundIdenticalProcess = 1;
				destRunner->memoryinfo.PageFaultCount += processToAddRunner->memoryinfo.PageFaultCount;
				destRunner->memoryinfo.QuotaPagedPoolUsage += processToAddRunner->memoryinfo.QuotaPagedPoolUsage;
				destRunner->memoryinfo.QuotaPeakPagedPoolUsage += processToAddRunner->memoryinfo.QuotaPeakPagedPoolUsage;
				destRunner->memoryinfo.PagefileUsage += processToAddRunner->memoryinfo.PagefileUsage;
				destRunner->memoryinfo.WorkingSetSize += processToAddRunner->memoryinfo.WorkingSetSize;
				destRunner->dllInfo = extraDllsCheck(destRunner->dllInfo, processToAddRunner->dllInfo);
				destRunner->loadedDlls += processToAddRunner->loadedDlls;
				break;
			}
			destRunner = destRunner->next;
		}
		if (foundIdenticalProcess == 0) {
			destSnapshot = addExtraProcess(destSnapshot,processToAddRunner);
		}
		processToAddRunner = processToAddRunner->next;
	}
	return destSnapshot;
}
 

char snapshotListStatus[150] = "Empty";


//user response menu function 
void userMenu() {
int chosenNumber = 0;
while (chosenNumber != 9)
{
	printf("%s", snapshotListStatus);
	printf("\n*********MENU************\ntype number for execution\n'1'. Take One SnapShot\n'2'. Take 20 SnapShots\n'3'. Start Long SnapShot\n'4'. End Long SnapShot\n'5'. Generate HTML Report\n'6'. Reset Collections\n'7'. Save in File\n'8'. Load from File\n'9'. Quit\n");
	scanf("%d", &chosenNumber);
	
		switch (chosenNumber)
		{
		case 1:
			snapshotCounter++;
			printf("\nTaking One SnapShot\n");
			oneSnapHead = (processinformation*)malloc(sizeof(processinformation));
			oneSnapHead = GetProcessesInfo();
			addSnapshot(oneSnapHead);
			//printSnapshots();
			listInit();
			 if (snapshotCounter == 1) {
				 strcpy(snapshotListStatus, "\nOne SnapShot->");
			 }
			 else {
				 strcat(snapshotListStatus, "One SnapShot-> ");
			 }
			// write to log file here (Local or Global).
			break;
		case 2:
			snapshotCounter++;
			printf("\nTaking 20 Sec SnapShot\n");
			
			for (int i = 1; i < 20; i++)
			{
				if (i == 1) {
					twentySnapHead = (processinformation*)malloc(sizeof(processinformation));
					twentySnapHead = GetProcessesInfo();
					listInit();
				}
				tempSnapHead = GetProcessesInfo();
				twentySnapHead = sumProcessesInfo(twentySnapHead, tempSnapHead);
				listInit();
				
				Sleep(1000);
			
				// if the user click 'B' the loop will stop.
				/*if (_kbhit())
				{
					if (_getch() == 'B')
					{
						break;
					}
				}
				*/
				
				// write to log file here (Local or Global).
			}
			
			addSnapshot(twentySnapHead);
			if (snapshotCounter == 1) {
				strcpy(snapshotListStatus, "\n20 sec SnapShot->");
			}
			else {
				strcat(snapshotListStatus, "20 sec SnapShot-> ");
			}
			// write to log file here (Local or Global).
			break;
		case 3:
			snapshotCounter++;
			printf("\nStart long snapshot\n");
			printf("\nPress 4 to End\n");
			longSnapHead = (processinformation*)malloc(sizeof(processinformation));
			longSnapHead = GetProcessesInfo();
			listInit();
			while (!_kbhit()) {
				tempSnapHead = GetProcessesInfo();
				longSnapHead = sumProcessesInfo(longSnapHead, tempSnapHead);
				listInit();
				Sleep(1000);
				if (_getch() == '4') {
					addSnapshot(longSnapHead);
					break;
				}
			}
			// write to log file here (Local or Global).
			break;
		case 4:
			printf("\nEnd Long SnapShot\n");
			// write to log file here (Local or Global).
			break;
		case 5:
			printf("\nGenerate HTML Report\n");
			// write to log file here (Local or Global).
			break;
		case 6:
			snapshotCounter = 0;
			printf("\nReset snapshot collection\n");
			printSnapshots();
			resetSnapshotCollection();
			printSnapshots();
			strcpy(snapshotListStatus, "Empty");
			// write to log file here (Local or Global).
			break;
		case 7:
			printf("\nSave in File\n");
			// write to log file here (Local or Global).
			break;
		case 8:
			printf("\nLoad from File\n");
			// write to log file here (Local or Global).
			break;
		case 9:
			printSnapshots();
			printf("\nDone :)\n");
			printf("\nThank you & See you soon!\n");
			// write to log file here (Local or Global).
			break;

		default:
			printf("\n***Invalid input, please try again***\n");
			// write to log file here (Local or Global).
			break;
		}
	}
}

