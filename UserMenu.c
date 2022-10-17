
#include "GetProcessesInfo.h"
#include "ProcessesLinkedList.h"
#include "saveAndLoad.h"
#include "GenerateHTML.h"
#include "SnapshotsTypes.h"
#include "userMenu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)


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
			if (snapshotCounter > 0) {
				printf("Load data from file will delete all current data. Are you sure? (y/n)\n");
				if (_getch() == 'n') {
					break;
				}
				resetSnapshotCollection();
			}
			
			printf("\nLoad from File\n");
			loadFromFile();
			printf("Snapshots Loaded\n\n");
			snapshotCounter = header.snapshotsCount; // update the snapshot counter
			strcpy(snapshotListStatus, "Snapshot list status: Snapshot taken ");
			for (int i = 1; i < snapshotCounter; i++) // update the list status after loading
			{
				strcat(snapshotListStatus, "-> Snapshot taken->");
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

