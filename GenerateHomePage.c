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

#define updatedHomePage "Process-Monitor-HTML\\Home-Page\\HomePage.html" // the path of the updated home page
#define dllTableData "Process-Monitor-HTML\\Dynamic-Data-Pages\\dynamicDllTable.html" // the path of the dynamic data file
#define staticHomePage "Process-Monitor-HTML\\Static-Src-Pages\\StaticHomePage.html" // the path of the static home page

/// <summary>
///  This function generates the dynamic data of the overall Samples table
/// </summary>
void overallSamplesDynamicTable() {
	
	snapshotsList* currentS = snapshotListHead; 
	strcpy(buffer, "");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", PCounter); // get the total number of unique processes
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", DCounter); // get the total number of unique dlls
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", memoryUsageAvg(currentS)); // get the average memory usage of all the processes
	strcat(buffer, converter);
	strcat(buffer, "</td>");
}

/// <summary>
///  This function generates the dynamic data of the snapshots details table
/// </summary>
void snapshotsDynamicTable() {
	snapshotsList* currentS = snapshotListHead;
	int processCnt = 0;
	int dllCnt = 0;
	unsigned long totalMemoryUsage = 0;
	char copyToBuffer[10000] = {0};
	// loop through all the snapshots
	for (int i = 1; i <= snapshotCounter; i++) { 
		
		// clear buffer and updatedFileName after each iteration
		strcpy(buffer, ""); 
		strcpy(updatedFileName, "");  
		
		// get the data headlines of the current snapshot
		processCnt = processesCount(currentS->snapshotData); 
		dllCnt = totalSnapshotDlls(currentS->snapshotData); 
		totalMemoryUsage = totalMemoryUsageCount(currentS->snapshotData); 
		
		generateSample(currentS); // generate the sample page of the current snapshot
		
		strcpy(buffer, ""); // clear buffer after generating the sample page
		strcat(copyToBuffer, "<tr>\n");
		strcat(copyToBuffer, "<th>\n");
		strcat(copyToBuffer, "<a class=\"text-light bg-dark\" href=\"..\\..\\"); 
		strcat(copyToBuffer, updatedFileName); // add the link to the sample page (recieved from the sample generation)
		strcat(copyToBuffer, "\"> Snapshot ");
		sprintf(converter, "%d", i); // add the number of the current snapshot
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</a>\n");
		strcat(copyToBuffer, "</th>\n");
		strcat(copyToBuffer, "<td>");
		sprintf(converter, "%d", processCnt); // add the number of processes in the current snapshot
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</td>\n");
		strcat(copyToBuffer, "<td>");
		sprintf(converter, "%d", dllCnt); // add the number of dlls in the current snapshot
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</td>\n");
		strcat(copyToBuffer, "<td>");
		sprintf(converter, "%d", totalMemoryUsage / processCnt); // add the average memory usage of the processes in the current snapshot
		strcat(copyToBuffer, converter);
		strcat(copyToBuffer, "</td>\n");
		strcat(copyToBuffer, "</tr>\n");
		currentS = currentS->nextSnap;
	}
	strcpy(buffer, copyToBuffer); // copy the data to the buffer to be written to the updated homepage
}

/// <summary>
///  This function generates the dynamic data of the unique dlls table
/// </summary>
void dllsDynamicTable() {
	int count = 1;
	dllDict* currentDll = dllDictHead;
	
	for (int i = 1; i <= DCounter; i++) { // loop through all the unique dlls
	
     // generate the dll page of the current dll
		generateDllPage(currentDll); 
		
		strcpy(buffer, ""); // clear buffer after generating the dll page
		strcat(buffer, "<tr>\n");
		strcat(buffer, "<td>");
		strcat(buffer, "<a class=\"text-light bg-dark ddt\" href=\"..\\..\\");
		strcat(buffer, updatedFileName); // add the link to the dll page (recieved from the dll page generation)
		strcat(buffer, "\">");
		strcat(buffer, currentDll->dllName); // add the name of the current dll
		strcat(buffer, "</a>\n");
		strcat(buffer, "</td>\n");
		strcat(buffer, "</tr>\n");
		
		// if it's the first dll - write the data(saved in buffer) to the dynamic data file
	if (count == 1) { 
		saveTempFile(dllTableData, buffer, "w");
		count++; // increase the counter to prevent deleting of the previous data
	}
	// if it's not the first dll - append the data(saved in buffer) to the dynamic data file
	else {
		saveTempFile(dllTableData, buffer, "a");
	}
	
	currentDll = currentDll->next;
	
	}
	strcpy(updatedFileName, "");
}

/// <summary>
///  This function generates the updated and ready home page
/// </summary>
void generateHomePage() {
	overallSamplesDynamicTable(); // generate the dynamic data of the overall samples table
	insertDynamicData(staticHomePage, "dynamic", updatedHomePage); // insert the dynamic data to the static home page
	snapshotsDynamicTable(); // generate the dynamic data of the snapshots details table
	insertDynamicData(updatedHomePage, "dynamicTable", updatedHomePage); // insert the dynamic data to the partly updated home page
	dllsDynamicTable(); // generate the dynamic data of the unique dlls table
	insertDataFromFile(updatedHomePage, dllTableData, "dynamicDllTable", updatedHomePage); // insert the dynamic data to the partly updated home page
}