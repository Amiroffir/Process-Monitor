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

processinformation* procInfoHead = NULL;
processinformation* procInfoTail = NULL;




int totalSnapshotDlls(processinformation* snapHead) {
	
	int dllCount = 0;
	processinformation* current = snapHead;
	while (current != NULL)
	{
		dllCount += dllsCount(current->dllInfo);
		current = current->next;
	}		
	return dllCount;
}

int totalProcessesCount(snapshotsList* head) {
	int count = 0;
	snapshotsList* current = head;
	while (current != NULL)
	{
		count += processesCount(current->snapshotData);
		current = current->nextSnap;
	}
	return count;
}

unsigned long totalMemoryUsageCount(processinformation* snapHead) {
	unsigned long count = 0;
	processinformation* current = snapHead;
	while (current != NULL)
	{
		count += current->memoryinfo.WorkingSetSize;
		current = current->next;
	}
	return count;
}


unsigned long memoryUsageAvg(snapshotsList* head) {
	
	    int count = totalProcessesCount(head);
		unsigned long totalMemoryUsage = 0;
		snapshotsList* current = head;
		while (current != NULL)
		{
			totalMemoryUsage += totalMemoryUsageCount(current->snapshotData);
			current = current->nextSnap;
		}
		return totalMemoryUsage / count;
}


void swap(processinformation* Address)
{
	if (Address == NULL || Address->next == NULL)
	{
		return;
	}
	processinformation* temp = (processinformation*)malloc(sizeof(processinformation));
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

processinformation* SortByLoadedDlls(snapshotsList* snapHead) {
	{
		
		procInfoHead = (processinformation*)malloc(sizeof(processinformation));
		procInfoHead = snapHead->snapshotData;
		processinformation* current = procInfoHead;
		processinformation* temp = procInfoHead;
		
		int swapped = 1;
		while (swapped == 1)
		{
			swapped = 0;
			while (current->next != NULL)
			{
				if (current->totalLoadedDlls < current->next->totalLoadedDlls)
				{
					swap(current);
					swapped = 1;
				}
				else
				{
					current = current->next;
				}
			}
			if (swapped == 0)
			{
				break;
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
			snapHead->snapshotData = procInfoHead;
		return procInfoHead;
	}
}

int highestMemoryUsage(processinformation* snapHead) {
	
		int highest = 0;
		processinformation* current = snapHead;
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
	
int totalProcThatUsed(dllInfo* currD) {
	dllDict* identicalDll = searchDll(currD,NULL);
	processDict* current = identicalDll->usedBy;
	 int procesessCount = 0;
	while (current != NULL)
	{
		procesessCount++;
		current = current->next;
	}
	return procesessCount;
}