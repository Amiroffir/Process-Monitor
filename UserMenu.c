
#include "GetProcessesInfo.h"
#include "ProcessesLinkedList.h"
#include "saveAndLoad.h"
#include "GenerateHTML.h"
#include "userMenu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

/// <summary>
   /// Function that sums the information of two identical processes & DLL's of two differnt snapshots.   /// The function returns the updated snapshot.
  /// </summary>
struct processinformation* sumProcessesInfo(processinformation* destSnapshot, processinformation* processToAdd) {
	int foundIdenticalProcess;
	processinformation* destRunner;
	processinformation* processToAddRunner;
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
	oneSnapHead = (processinformation*)malloc(sizeof(processinformation));
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
			twentySnapHead = (processinformation*)malloc(sizeof(processinformation));
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
	
	longSnapHead = (processinformation*)malloc(sizeof(processinformation));
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



 

/// <summary>
/// User response menu. the function running until the user chosen number is 9("Quit").
/// </summary>
void userMenu() {

// Snapshot list status info for user
strcpy(snapshotListStatus, "Snapshot list status: Empty");

int chosenNumber = 0;
while (chosenNumber != 9)
{
	printf("%s\n", snapshotListStatus);
	printf("\n*********MENU************\ntype number for execution\n'1'. Take One SnapShot\n'2'. Take 20 SnapShots\n'3'. Start Long SnapShot\n'4'. End Long SnapShot\n'5'. Generate HTML Report\n'6'. Reset Collections\n'7'. Save in File\n'8'. Load from File\n'9'. Quit\n");
	scanf("%d", &chosenNumber);
	
		switch (chosenNumber)
		{
		case 1:
			oneSnapshot();
			LogEvent("One Snapshot taken\n");
			break;
		case 2:
			twentySecSnapshot();
			LogEvent("20 Seconds Snapshot taken\n");
			break;
		case 3:
			longSnapshot();
			LogEvent("Long Snapshot taken\n");
			break;
		case 4:
			printf("\nEnd Long Snapshot\n");
			break;
		case 5:
			printf("\nGenerate HTML Report\n");
			generateHtml();
			LogEvent("HTML Report generated\n");
			break;
		case 6:
			snapshotCounter = 0; //reset the snapshot counter
			printf("\nReset snapshot collection\n");
			resetSnapshotCollection();
			strcpy(snapshotListStatus, "Snapshot list status: Empty");
			LogEvent("Snapshot collection reset\n");
			break;
		case 7:
			printf("\nSave in File\n");
			saveToFile();
			LogEvent("Data saved to file\n");
			break;
		case 8:
			printf("\nLoad from File\n");
			loadFromFile();
			printf("Snapshots Loaded\n\n");
			snapshotCounter = header.snapshotsCount; // update the snapshot counter
			strcpy(snapshotListStatus, "Snapshot list status: Snapshot taken ");
			for (int i = 1; i < snapshotCounter; i++) // update the list status after loading
			{
				strcat(snapshotListStatus, "-> Snapshot taken");
			}
			LogEvent("Data loaded from file\n");
			break;
		case 9:
			printSnapshots();
			printf("\nDone :)\n");
			printf("\nThank you & See you soon!\n");
			LogEvent("Program ended\n");
			break;

		default:
			printf("\n***Invalid input, please try again***\n");
			break;
		}
	}
}

