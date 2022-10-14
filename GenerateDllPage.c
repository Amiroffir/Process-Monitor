#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "HtmlDataProcessor.h"
#include "GenerateSamplePage.h"
#include "GenerateHTML.h"
#include "Dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

#define dllPageNameTemplate "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-HTML\\DLL-Pages\\dll"
#define staticDllPage "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-HTML\\Static-Src-Pages\\StaticDllPage.html"
#define usedByProcData "C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-HTML\\Dynamic-Data-Pages\\dynamicProcThatUsedDetails.html"
char sampleNum[50];
int counterInit = 1;
int dllPageCounter = 1;



void dynamicChosenDll(dllInfo* currD) {
	{
		strcpy(buffer, "");
		strcat(buffer, "<td>");
		strcat(buffer, currD->dllName);
		strcat(buffer, "</td>");
		strcat(buffer, "<td>");
		sprintf(converter, "%d", totalProcThatUsed(currD));
		strcat(buffer, converter);
		strcat(buffer, "</td>");
	}
}



void dynamicProcThatUsedDetails(dllInfo* currDllToCheck) {
	int count = 1;
	dllDict* currDll = searchDll(currDllToCheck, NULL);
	if (currDll == NULL) {
		return;
	}
	processDict* currP;
	currP = currDll->usedBy;
		while (currP != NULL) {
			strcpy(buffer, "");
				strcat(buffer, "<tr>\n");
				strcat(buffer, "<td class=\"border name\">");
				strcat(buffer, currP->processName);
				strcat(buffer, "</td>");
				strcat(buffer, "<td class=\"border\">");
				sprintf(converter, "%d", currP->processID);
				strcat(buffer, converter);
				strcat(buffer, "</td>");
				strcat(buffer, "</tr>");
				if (count == 1) {
					saveTempFile(usedByProcData, buffer, "w");
					count++;
				}
				else {
					saveTempFile(usedByProcData, buffer, "a");
				}
				currP = currP->next;
		    }
		}
		


 void generateDllPage(dllInfo* currentD) {
	if (snapshotCounter > counterInit) {
		counterInit = snapshotCounter;
		dllPageCounter = 1;
	}
	strcpy(updatedFileName, ""); // clear updatedFileName
	strcpy(updatedFileName, dllPageNameTemplate);	// copy the template to updatedFileName
	sprintf(sampleNum, "%d%s", dllPageCounter, ".html");	// convert the dllPageCounter to string
	strcat(updatedFileName, sampleNum);	// add the sample number to updatedFileName
	dllPageCounter++;	// increment the dllPageCounter
	
	dynamicChosenDll(currentD);
	insertDynamicData(staticDllPage, "dynamicChosenDll", updatedFileName);
	dynamicProcThatUsedDetails(currentD);
	insertDataFromFile(updatedFileName, usedByProcData, "dynamicProcThatUsedDetails", updatedFileName);
	
}