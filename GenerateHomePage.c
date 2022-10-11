#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "HtmlDataProcessor.h"
#include "GenerateHTML.h"
#include "GenerateSamplePage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)


void overallSamplesDynamicTable() {
	// overall samples dynamic table
	snapshotsList* currentS = snapshotListHead;
	strcpy(buffer, "");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", 5);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", 5);
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
		strcpy(updatedFileName, "");
		generateSample(currentS);
		processCnt = processesCount(currentS->snapshotData);
		dllCnt = totalSnapshotDlls(currentS->snapshotData);
		totalMemoryUsage = totalMemoryUsageCount(currentS->snapshotData);
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
	strcpy(buffer, ""); // clear buffer

	strcat(buffer, "<tr>\n");
	strcat(buffer, "<td>\n");
	strcat(buffer, "<a class=\"text-light bg-dark\" href = \"#.css\">");
	sprintf(converter, "%d", 5);
	strcat(buffer, converter);
	strcat(buffer, "</a>\n");
	strcat(buffer, "</td>\n");
	strcat(buffer, "</tr>\n");

}

void generateHomePage() {

	overallSamplesDynamicTable();
	insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\StaticHomePage.html", "dynamic", "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html");
	snapshotsDynamicTable();
	insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html", "dynamicTable", "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html");
	dllsDynamicTable();
	insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html", "dynamicDllTable", "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html");

}