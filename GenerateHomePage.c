#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "HtmlDataProcessor.h"
#include "GenerateHTML.h"
#include "GenerateSamplePage.h"
#include "GenerateDllPage.h"
#include "Dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)
#define updatedHomePage "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-HTML\\Home-Page\\HomePage.html"
#define dllTableData "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-HTML\\Dynamic-Data-Pages\\dynamicDllTable.html"
#define staticHomePage "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-HTML\\Static-Src-Pages\\StaticHomePage.html"
void overallSamplesDynamicTable() {
	// overall samples dynamic table
	snapshotsList* currentS = snapshotListHead;
	strcpy(buffer, "");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", PCounter);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", DCounter);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", memoryUsageAvg(currentS));
	strcat(buffer, converter);
	strcat(buffer, "</td>");
}

void snapshotsDynamicTable() {
	// snapshots dynamic table
	snapshotsList* currentS = snapshotListHead;
	int processCnt = 0;
	int dllCnt = 0;
	unsigned long totalMemoryUsage = 0;
	char copyToBuffer[10000] = {0};
	for (int i = 1; i <= snapshotCounter; i++) {
		strcpy(buffer, ""); // clear buffer
		strcpy(updatedFileName, ""); // clear updatedFileName
		processCnt = processesCount(currentS->snapshotData);
		dllCnt = totalSnapshotDlls(currentS->snapshotData);
		totalMemoryUsage = totalMemoryUsageCount(currentS->snapshotData);
		generateSample(currentS);
		strcpy(buffer, ""); // clear buffer
		strcat(copyToBuffer, "<tr>\n");
		strcat(copyToBuffer, "<th>\n");
		strcat(copyToBuffer, "<a class=\"text-light bg-dark\" href=\"");
		strcat(copyToBuffer, updatedFileName);
		strcat(copyToBuffer, "\"> Snapshot ");
		sprintf(converter, "%d", i);
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</a>\n");
		strcat(copyToBuffer, "</th>\n");
		strcat(copyToBuffer, "<td>");
		sprintf(converter, "%d", processCnt);
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</td>\n");
		strcat(copyToBuffer, "<td>");
		sprintf(converter, "%d", dllCnt);
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</td>\n");
		strcat(copyToBuffer, "<td>");
		sprintf(converter, "%d", totalMemoryUsage / processCnt);
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</td>\n");
		strcat(copyToBuffer, "</tr>\n");
		currentS = currentS->nextSnap;
	}
	strcpy(buffer, copyToBuffer);
}

void dllsDynamicTable() {
	int count = 1;
	
	dllDict* currentDll = dllDictHead;
	for (int i = 1; i <= DCounter; i++) {
		generateDllPage(currentDll);
		strcpy(buffer, ""); // clear buffer
		strcat(buffer, "<tr>\n");
		strcat(buffer, "<td>");
		strcat(buffer, "<a class=\"text-light bg-dark\" href=\"");
		strcat(buffer, updatedFileName);
		strcat(buffer, "\">");
		strcat(buffer, currentDll->dllName);
		strcat(buffer, "</a>\n");
		strcat(buffer, "</td>\n");
		strcat(buffer, "</tr>\n");
		
	if (count == 1) {
		saveTempFile(dllTableData, buffer, "w");
		count++;
	}
	else {
		saveTempFile(dllTableData, buffer, "a");
	}
	
	currentDll = currentDll->next;
	
	}
	strcpy(updatedFileName, "");
}

void generateHomePage() {
	overallSamplesDynamicTable();
	insertDynamicData(staticHomePage, "dynamic", updatedHomePage);
	snapshotsDynamicTable();
	insertDynamicData(updatedHomePage, "dynamicTable", updatedHomePage);
	dllsDynamicTable();
	insertDataFromFile(updatedHomePage, dllTableData, "dynamicDllTable", updatedHomePage);
}