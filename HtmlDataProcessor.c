#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "HtmlDataProcessor.h"
#include "SnapshotsTypes.h"
#include "saveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)


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


