#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "GenerateSamplePage.h"
#include "GenerateHTML.h"
#include "Dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

#define dllPageNameTemplate "Process-Monitor-HTML\\DLL-Pages\\dll"  // template of the dll page name
#define staticDllPage "Process-Monitor-HTML\\Static-Src-Pages\\StaticDllPage.html" // path to the static html page
#define usedByProcData "Process-Monitor-HTML\\Dynamic-Data-Pages\\dynamicProcThatUsedDetails.html" // path to the dynamic data file
char sampleNum[50]; 
int counterInit = 1;
int dllPageCounter = 1; // counter for the dll pages for future using in the dll page name

/// <summary>
/// This function generates the dynamic data of the chosen dll table
/// </summary>
/// <param name="currD"></param>
void dynamicChosenDll(dllInfo* currD) { 
	{
		strcpy(buffer, "");
		strcat(buffer, "<td>");
		strcat(buffer, currD->dllName); 
		strcat(buffer, "</td>");
		strcat(buffer, "<td>");
		sprintf(converter, "%d", totalProcThatUsed(currD)); // get the total number of processes that used the dll
		strcat(buffer, converter);
		strcat(buffer, "</td>");
	}
} 

/// <summary>
	///  This function generates the dynamic data of the processes that used the chosen dll table
	/// </summary>
	/// <param name="currDllToCheck"></param>
void dynamicProcThatUsedDetails(dllInfo* currDllToCheck) {
	int count = 1;
	dllDict* currDll = searchDll(currDllToCheck, NULL); // search the identical dll name in the dll dictionary
	if (currDll == NULL) {
		return;
	}
	processDict* currP;
	currP = currDll->usedBy; // get the first process that used the dll
		while (currP != NULL) {
			strcpy(buffer, ""); // clear the buffer
				strcat(buffer, "<tr>\n");
				strcat(buffer, "<td class=\"border name\">");
				strcat(buffer, currP->processName);
				strcat(buffer, "</td>");
				strcat(buffer, "<td class=\"border\">");
				sprintf(converter, "%d", currP->processID); 
				strcat(buffer, converter);
				strcat(buffer, "</td>");
				strcat(buffer, "</tr>");
				// if its the first process that used the dll - write the data(saved in buffer) to the dynamic data file
				if (count == 1) {  
					saveTempFile(usedByProcData, buffer, "w"); 
					count++; // increase the counter to prevent deleting of the previous data
				}
				// if its not the first process that used the dll - append the data(saved in buffer) to the dynamic data file
				else { 
					saveTempFile(usedByProcData, buffer, "a");
				}
				currP = currP->next; 
		    }
		}
		
 /// <summary>
 ///  This function generates the updated and ready dll page
 /// </summary>
 /// <param name="currentD"></param>
 void generateDllPage(dllInfo* currentD) {
	
	 // make sure that the dllPage name will be generated correctly 
	 //in case of additional snapshots after report is generated once or more at the same running time
	 if (snapshotCounter > counterInit) { 
		counterInit = snapshotCounter;
		dllPageCounter = 1;
	}
	 // generate the dll page name
	strcpy(updatedFileName, ""); 
	strcat(updatedFileName, dllPageNameTemplate);	
	sprintf(sampleNum, "%d%s", dllPageCounter, ".html");	
	strcat(updatedFileName, sampleNum);	
	dllPageCounter++;	// increment the dllPageCounter
	
	dynamicChosenDll(currentD); // generate the dynamic data of the chosen dll table
	insertDynamicData(staticDllPage, "dynamicChosenDll", updatedFileName); // insert the dynamic data to the static html page	
	dynamicProcThatUsedDetails(currentD); // generate the dynamic data of the processes that used the chosen dll table
	insertDataFromFile(updatedFileName, usedByProcData, "dynamicProcThatUsedDetails", updatedFileName); // insert the dynamic data to the partly-updated html page
	
}