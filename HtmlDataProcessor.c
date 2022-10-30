#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "Dictionary.h"
#include "HtmlDataProcessor.h"
#include "SnapshotsTypes.h"
#include "saveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

processInformation* procInfoHead = NULL;
processInformation* procInfoTail = NULL;

/// <summary>
	///  Counts the total number of dlls in a snapshot
/// </summary>
int totalSnapshotDlls(processInformation* snapHead) {
	
	int dllCount = 0;
	processInformation* current = snapHead;
	while (current != NULL)
	{
		dllCount += dllsCount(current->dllInfo); //dllsCount() is a function that counts the number of dlls in a process
		current = current->next;
	}		
	return dllCount;
}

/// <summary>
	///  Counts the total number of processes in a collection
/// </summary>
int totalProcessesCount(snapshotsList* head) {
	int count = 0;
	snapshotsList* current = head;
	while (current != NULL)
	{
		count += processesCount(current->snapshotData); //processesCount is a function that counts the number of processes in a snapshot
		current = current->nextSnap;
	}
	return count;
}

/// <summary>
	///  Counts the total memory usage of a snapshot
/// </summary>
unsigned long long totalMemoryUsageCount(processInformation* snapHead) {
	unsigned long long count = 0;
	processInformation* current = snapHead;
	while (current != NULL)
	{
		count += current->memoryinfo.WorkingSetSize;
		current = current->next;
	}
	return count;
}

/// <summary>
	///  Counts the average memory usage of a collection
/// </summary>
unsigned long memoryUsageAvg(snapshotsList* head) {
	   
	int count = totalProcessesCount(head); //totalProcessesCount() is a function that counts the total number of processes in a collection
		unsigned long long totalMemoryUsage = 0;
		snapshotsList* current = head;
		while (current != NULL)
		{
			totalMemoryUsage += totalMemoryUsageCount(current->snapshotData); //totalMemoryUsageCount is a function that counts the total memory usage of a snapshot
			current = current->nextSnap;
		}
		return totalMemoryUsage / count; //returns the average memory usage of a collection
}

/// <summary>
   ///  Swaps the order of two processes
/// </summary>
void swap(processInformation* Address)
{
	if (Address == NULL || Address->next == NULL)
	{
		return;
	}
	processInformation* temp = (processInformation*)malloc(sizeof(processInformation));
	temp->next = Address->next->next;
	temp->prev = Address->next;
	Address->next->next = Address;
	Address->next->prev = Address->prev;
	Address->next = temp->next;
	Address->prev = temp->prev;
	free(temp);

	// if the item changed was the head
	if (Address->prev->prev == NULL)
	{
		procInfoHead = Address->prev;
	}

	else
	{
		Address->prev->prev->next = Address->prev;
	}
	// if the item is now the tail
	if (Address->next == NULL)
	{
		procInfoTail = Address;
	}
	else
	{
		Address->next->prev = Address;
	}
}

/// <summary>
	///  Sorts the processes (shaker sort) in a snapshot by the number of loaded dlls
	/// returns the head of the sorted list
/// </summary>
processInformation* SortByLoadedDlls(snapshotsList* snapHead) {
		
		procInfoHead = (processInformation*)malloc(sizeof(processInformation));
		procInfoHead = snapHead->snapshotData;
		processInformation* current = procInfoHead;
		processInformation* temp = procInfoHead;
		
		int swapped = 1;
		while (swapped == 1) 
		{
			swapped = 0; //reset the swapped flag on entering the loop
			while (current->next != NULL)
			{
				// if the current process has les dlls than the next one
				if (current->totalLoadedDlls < current->next->totalLoadedDlls) 
				{
					swap(current);
					swapped = 1; //shows that a swap occurred
				}
				else
				{
					current = current->next;
				}
			}
			if (swapped == 0)
			{ 
				break; // exit the outer loop if no swaps occurred.
			}
			swapped = 0;
			while (current->prev != NULL)
			{
				if (current->totalLoadedDlls > current->prev->totalLoadedDlls)
				{
					swap(current->prev);
					swapped = 1;
				}
				else
				{
					current = current->prev;
				}
			}
		}
		snapHead->snapshotData = procInfoHead; //update the head of the snapshot processes list
		return procInfoHead; //return the head of the sorted processes list
}

///<summary>
	///  Finds the highest memory usage process of a snapshot
/// </summary>
unsigned long long highestMemoryUsage(processInformation* snapHead) {	
	
		unsigned long long highest = 0;
		processInformation* current = snapHead;
		while (current != NULL)
		{
			if (current->memoryinfo.WorkingSetSize > highest)
			{
				highest = current->memoryinfo.WorkingSetSize; 
			}
			current = current->next;
		}
		return highest;
	}
	
/// <summary>
	///  Counts the total number of processes that used a given dll
/// </summary>
int totalProcThatUsed(dllInfo* currD) {
	
	dllDict* identicalDll = searchDll(currD, NULL); // retrieve the current Dll from the dll dictionary
	processDict* current = identicalDll->usedBy;
	 int procesessCount = 0;
	
 // count the number of processes that used the dll
	while (current != NULL) 
	{
		procesessCount++;
		current = current->next;
	}
	return procesessCount;
}