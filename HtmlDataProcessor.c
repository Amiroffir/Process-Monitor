#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
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