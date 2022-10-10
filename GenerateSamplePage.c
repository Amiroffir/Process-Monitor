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


int SaveTempFile(char* fileName, char* buff)
{
	FILE* fi = fopen(fileName, "a");
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
		strcat(buffer, "<td class=\"border\">");
		dllInfo* runner = currProcList->dllInfo;
		while (runner != NULL) {
			strcat(buffer, runner->dllName);
			strcat(buffer, "\n");
			runner = runner->next;
		}
		strcat(buffer, "</td>\n");
		strcat(buffer, "</tr>\n");
		SaveTempFile("dynamicProcessesDetails.html", buffer);
		currProcList = currProcList->next;
	}
	
}


void generateSample(snapshotsList* currS) {
	strcpy(updatedFileName, sampleNameTemplate);
	sprintf(sampleNum, "%d%s", currS->snapshotData->snapshotCounter,".html");
	strcat(updatedFileName, sampleNum);
	
	dynamicChosenSample(currS);
	insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\StaticSamplePage.html", "dynamicChosenSample", updatedFileName);
	dynamicProcessesDetails(currS->snapshotData);
	insertDataFromFile(updatedFileName, "dynamicProcessesDetails", updatedFileName);
}