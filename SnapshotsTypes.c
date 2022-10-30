#include "GetProcessesInfo.h"
#include "ProcessesLinkedList.h"
#include "saveAndLoad.h"
#include "GenerateHTML.h"
#include "SnapshotsTypes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)


/// <summary>
   /// Function that sums the information of two identical processes & DLL's of two differnt snapshots.   /// The function returns the updated snapshot.
/// </summary>
struct processInformation* sumProcessesInfo(processInformation* destSnapshot, processInformation* processToAdd) {
	int foundIdenticalProcess;
	processInformation* destRunner;
	processInformation* processToAddRunner;
	processToAddRunner = processToAdd; //runner for the process to add
	while (processToAddRunner != NULL) { //for each process in the processToAdd snapshot

		foundIdenticalProcess = 0; //reset the flag
		destRunner = destSnapshot; //reset the runner to the head of the destination snapshot
		while (destRunner != NULL) { //search for identical process

			if ((strcmp(destRunner->processName, processToAddRunner->processName) == 0) &
				(destRunner->processID == processToAddRunner->processID)) {

				foundIdenticalProcess = 1; //found identical process
				destRunner->memoryinfo.PageFaultCount += processToAddRunner->memoryinfo.PageFaultCount;
				destRunner->memoryinfo.QuotaPagedPoolUsage += processToAddRunner->memoryinfo.QuotaPagedPoolUsage;
				destRunner->memoryinfo.QuotaPeakPagedPoolUsage += processToAddRunner->memoryinfo.QuotaPeakPagedPoolUsage;
				destRunner->memoryinfo.PagefileUsage += processToAddRunner->memoryinfo.PagefileUsage;
				destRunner->memoryinfo.WorkingSetSize += processToAddRunner->memoryinfo.WorkingSetSize;
				//check for extra dlls
				destRunner->dllInfo = extraDllsCheck(destRunner->dllInfo, processToAddRunner->dllInfo);
				destRunner->totalLoadedDlls += processToAddRunner->totalLoadedDlls; //update the total number of dlls
				break;
			}
			destRunner = destRunner->next;
		}
		if (foundIdenticalProcess == 0) { //process not found in destination snapshot
			destSnapshot = addExtraProcess(destSnapshot, processToAddRunner); //add the process to the destination snapshot
		}
		processToAddRunner = processToAddRunner->next;
	}
	return destSnapshot;
}

/// <summary>
	/// Function that creates a snapshot of the current processes and DLL's info.
/// </summary>
void oneSnapshot() {

	snapshotCounter++; //update the snapshot counter
	printf("\nTaking One SnapShot\n");
	oneSnapHead = (processInformation*)malloc(sizeof(processInformation));
	oneSnapHead = GetProcessesInfo(); //get the processes info
	addSnapshot(oneSnapHead); //add the snapshot to the linked list
	processesListInit(); //initialize the processes list

	// Update the snapshot list status
	if (snapshotCounter == 1) {
		strcpy(snapshotListStatus, "\nSnapshot list status: Snapshot taken-> ");
	}
	else {
		strcat(snapshotListStatus, "Snapshot taken-> ");
	}
}

/// <summary>
	/// Function that creates a snapshot summarizes the processes and DLL's info in the last 20 seconds.
/// </summary>
void twentySecSnapshot() {
	snapshotCounter++;
	printf("\nTaking 20 Seconds SnapShot\n");

	for (int i = 1; i < 20; i++)
	{
		if (i == 1) { // first snapshot is taken and will used as the base for the next snapshots
			twentySnapHead = (processInformation*)malloc(sizeof(processInformation));
			twentySnapHead = GetProcessesInfo();
			processesListInit();
		}
		tempSnapHead = GetProcessesInfo();
		twentySnapHead = sumProcessesInfo(twentySnapHead, tempSnapHead); //sum the info of the dest snapshot with the current one
		processesListInit();

		Sleep(1000); //wait 1 second
	}

	addSnapshot(twentySnapHead); //add the snapshot to the linked list

	if (snapshotCounter == 1) {
		strcpy(snapshotListStatus, "\nSnapshot list status: Snapshot taken-> ");
	}
	else {
		strcat(snapshotListStatus, "Snapshot taken-> ");
	}

}

/// <summary>
	/// Function that creates a snapshot summarizes the processes and DLL's info as long as the user decide.
	/// The snapshot will stop when the user will enter 4.
/// </summary>
void longSnapshot() {
	snapshotCounter++;
	printf("\nStart long snapshot\n");
	printf("\nPress 4 to End\n"); //user will press 4 to end the snapshot

	longSnapHead = (processInformation*)malloc(sizeof(processInformation));
	longSnapHead = GetProcessesInfo();
	processesListInit();
	while (!_kbhit()) { //while the user didn't press a key
		tempSnapHead = GetProcessesInfo();
		longSnapHead = sumProcessesInfo(longSnapHead, tempSnapHead); //sum the info of the dest snapshot with the current one
		processesListInit();
		Sleep(1000);
		if (_getch() == '4') { //if the user pressed 4, add the snapshot to the linked list and stop the monitoring.
			addSnapshot(longSnapHead);
			if (snapshotCounter == 1) {
				strcpy(snapshotListStatus, "\nSnapshot list status: Snapshot taken-> ");
			}
			else {
				strcat(snapshotListStatus, "Snapshot taken-> ");
			}
			break;
		}
	}
}


