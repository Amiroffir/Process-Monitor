#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
//#include "saveAndLoad.h"
#include "HtmlDataProcessor.h"
#include "GenerateSamplePage.h"
#include "GenerateHTML.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

#define sampleNameTemplate "Process-Monitor-HTML\\Sample-Pages\\Sample" // the template for the sample page name
#define staticSamplePage "Process-Monitor-HTML\\Static-Src-Pages\\StaticSamplePage.html" // the static sample page path
#define sampleProcsData "Process-Monitor-HTML\\Dynamic-Data-Pages\\dynamicProcessesDetails.html" // the dynamic data of the processes path
char sampleNum[50];
processInformation* sortedList; 

/// <summary>
	///  This function is  generating the dynamic data of the chosen sample
	/// </summary>
void dynamicChosenSample(snapshotsList* currS) {
	strcpy(buffer, "");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", currS->snapshotData->snapshotCounter); // get the snapshot number
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	strcat(buffer, currS->snapshotData->snapshotTime); // get the time of the snapshot
	strcat(buffer, "</td>");
}

/// <summary>
	///  This function is generating the dynamic data of the processes
	/// </summary>
	/// <param name="currProcList"></param>
void dynamicProcessesDetails(processInformation* currProcList) {
	unsigned long long addIcon = highestMemoryUsage(currProcList); // get the highest memory usage for future use
	int count = 1;
	while (currProcList != NULL) {
		strcpy(buffer, "");
		strcat(buffer, "<tr>\n");
		strcat(buffer, "<td class=\"border name\">");
		strcat(buffer, currProcList->processName); 
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%llu", currProcList->memoryinfo.PageFaultCount); 
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%llu", currProcList->memoryinfo.WorkingSetSize);
		strcat(buffer, converter);
		
		// if the current process is the highest memory usage process
		// add the icon to the table cell
		if (atoll(converter) == addIcon) { 
			strcat(buffer, "\t<img src=\"..\\images\\icons8-error-48.png\" width=\"22\" height=\"22\" class=\"d-inline-block align-bottom\" alt=\"\"/>");
		}
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%llu", currProcList->memoryinfo.QuotaPagedPoolUsage);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%llu", currProcList->memoryinfo.QuotaPeakPagedPoolUsage);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td class=\"border\">");
		sprintf(converter, "%llu", currProcList->memoryinfo.PagefileUsage);
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
		// add the dropdown button and menu
		strcat(buffer, "<div class=\"dropdown-left\">\n");  
		strcat(buffer, "<button type=\"button\" class=\"btn btn-secondary dropdown-toggle\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"false\"> DLL's List </button>"); 
		strcat(buffer, "<div class=\"dropdown dropdown-dlls\">"); 
		
		// loop over the process dlls and add them to the dropdown menu
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
		// if it's the first process, write the dynamic data to the file
		if (count == 1) {
			saveTempFile(sampleProcsData, buffer,"w");
			count++; 
		}
		// if it's the first process, append the dynamic data to the file
		else {
				saveTempFile(sampleProcsData, buffer, "a");
		}
		currProcList = currProcList->next;
	}
}

/// <summary>
	///  This function is generating updated and ready sample page
	/// </summary>
	/// <param name="currS"></param>
void generateSample(snapshotsList* currS) {
	// get the desired file name according to the sample number
	strcpy(updatedFileName, sampleNameTemplate);
	sprintf(sampleNum, "%d%s", currS->snapshotData->snapshotCounter,".html");
	strcat(updatedFileName, sampleNum);
	
	sortedList = SortByLoadedDlls(currS); // sort the processes by the number of loaded dlls
	
	dynamicChosenSample(currS); 
	insertDynamicData(staticSamplePage, "dynamicChosenSample", updatedFileName);
	dynamicProcessesDetails(sortedList); 
	insertDataFromFile(updatedFileName,sampleProcsData,"dynamicProcessesDetails", updatedFileName);
}