#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "HtmlDataProcessor.h"
#include "GenerateSamplePage.h"
#include "GenerateHTML.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

#define sampleNameTemplate "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\Sample"
char sampleNum[50];
processinformation* sortedList;


int saveTempFile(char* fileName, char* buff,char fileAction[5])
{
	FILE* fi = fopen(fileName, fileAction);
	if (!fi)
	{
		return 0;
	}

	fputs(buff, fi);


	fclose(fi);
}

void dynamicChosenSample(snapshotsList* currS) {
	strcpy(buffer, "");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", currS->snapshotData->snapshotCounter);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	strcat(buffer, currS->snapshotData->snapshotTime);
	strcat(buffer, "</td>");
}

void dynamicProcessesDetails(processinformation* currProcList) {
	int addIcon = highestMemoryUsage(currProcList);
	int count = 1;
	while (currProcList != NULL) {
		strcpy(buffer, "");
		strcat(buffer, "<tr>\n");
		strcat(buffer, "<td class=\"border name\">");
		strcat(buffer, currProcList->processName);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%d", currProcList->memoryinfo.PageFaultCount);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		
		sprintf(converter, "%d", currProcList->memoryinfo.WorkingSetSize);
		strcat(buffer, converter);
		if (atoi(converter) == addIcon) {
			strcat(buffer, "\t<img src=\"C:/Users/Amir Offir/.vscode/Process-Monitor/Images/icons8-error-48.png\" width=\"22\" height=\"22\" class=\"d-inline-block align-bottom\" alt=\"\"/>");
		}
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%d", currProcList->memoryinfo.QuotaPagedPoolUsage);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%d", currProcList->memoryinfo.QuotaPeakPagedPoolUsage);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%d", currProcList->memoryinfo.PagefileUsage);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%d", currProcList->processID);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%d", currProcList->totalLoadedDlls);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border ProcTableHead\">\n");
		
		strcat(buffer, "<div class=\"dropdown-left\">\n");
		strcat(buffer, "<button type=\"button\" class=\"btn btn-secondary dropdown-toggle\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"false\"> DLL's List </button>");
		strcat(buffer, "<div class=\"dropdown dropdown-dlls\">");
		dllInfo* runner = currProcList->dllInfo;
		while (runner != NULL) {
			strcat(buffer, "<a class=\"dropdown-item border-bottom\">");
			strcat(buffer, runner->dllName);
			strcat(buffer, "</a>");
			runner = runner->next;
		}
		
		strcat(buffer, "</div>\n");
		strcat(buffer, "</div>\n");
		strcat(buffer, "</td>\n");
		strcat(buffer, "</tr>\n");
		if (count == 1) {
			saveTempFile("dynamicProcessesDetails.html", buffer,"w");
		}
		else {
				saveTempFile("dynamicProcessesDetails.html", buffer, "a");
		}
		count++;
		currProcList = currProcList->next;
	}
	
}


void generateSample(snapshotsList* currS) {
	
	strcpy(updatedFileName, sampleNameTemplate);
	sprintf(sampleNum, "%d%s", currS->snapshotData->snapshotCounter,".html");
	strcat(updatedFileName, sampleNum);
	sortedList = SortByLoadedDlls(currS);
	dynamicChosenSample(currS);
	insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\StaticSamplePage.html", "dynamicChosenSample", updatedFileName);
	dynamicProcessesDetails(sortedList /*currS->snapshotData*/);
	insertDataFromFile(updatedFileName,"dynamicProcessesDetails.html","dynamicProcessesDetails", updatedFileName);
}