#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "HtmlDataProcessor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

#define SEPERATOR1 "dynamic"
#define SEPERATOR2 "dynamicTable"

char buffer[1000] = { 0 };
char converter[1000] = { 0 };
int SaveIntoFile(char* fileName, char* buff)
{
	FILE* fi = fopen(fileName, "w");
	if (!fi)
	{
		return 0;
	}

	fputs(buff, fi);


	fclose(fi);
}
char* ReadAllFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}


	// Get the file size
	char* buff = (char*)malloc(1000);
	char* read;
	int fileSize = 0;
	while ((read = fgets(buff, 1000, f)))
	{
		fileSize += strlen(buff);
	}

	free(buff);
	fclose(f);
	fileSize++;

	// alloc space as file size
	buff = (char*)malloc(fileSize);

	f = fopen(fileName, "r");
	if (!f)
	{
		return NULL;
	}
	int readPosition = 0;
	char charToRead;
	while ((charToRead = fgetc(f)) != EOF)
	{
		buff[readPosition] = charToRead;
		readPosition++;
	}
	buff[readPosition] = NULL;

	fclose(f);

	return buff;
}
void insertDynamicData(char* filePath, char Seperator[100]) {

	char* homePageTemplate = ReadAllFile(filePath);

	// find the token
	char* found = strstr(homePageTemplate, Seperator);

	// get the position of the token
	int position = found - homePageTemplate;

	char* newFileSpace = (char*)malloc(strlen(homePageTemplate) + strlen(buffer));
	strncpy(newFileSpace, homePageTemplate, position);
	newFileSpace[position] = NULL;

	strcat(newFileSpace, buffer);
	newFileSpace[position + strlen(buffer)] = NULL;
	strcat(newFileSpace, found + strlen(Seperator));

	SaveIntoFile("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html", newFileSpace);
	free(newFileSpace);
	free(homePageTemplate);
}
void overallSamplesDynamicTable() {
	// overall samples dynamic table
	strcat(buffer, "<td>");
	sprintf(converter, "%d", 5);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", 5);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
	strcat(buffer, "<td>");
	sprintf(converter, "%d", 5);
	strcat(buffer, converter);
	strcat(buffer, "</td>");
}
void snapshotsDynamicTable() {
	// snapshots dynamic table
	strcpy(buffer, ""); // clear buffer
	snapshotsList* currentS = snapshotListHead;
	int processCnt = 0;
	int dllCnt = 0;
	for (int i = 1; i <= snapshotCounter; i++) {
		processCnt = processesCount(currentS->snapshotData);
		dllCnt = totalSnapshotDlls(currentS->snapshotData);
		strcat(buffer, "<tr>\n");
		strcat(buffer, "<th>\n");
		strcat(buffer, "<a class=\"text-light bg-dark\" href=\"#.css\"> Snapshot ");
		sprintf(converter, "%d", i);
		strcat(buffer, converter);
		strcat(buffer, "</a>\n");
		strcat(buffer, "</th>\n");
		strcat(buffer, "<td>");
		sprintf(converter, "%d", processCnt);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td>");
		sprintf(converter, "%d", dllCnt);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "<td>");
		sprintf(converter, "%d", dllCnt);
		strcat(buffer, converter);
		strcat(buffer, "</td>\n");
		strcat(buffer, "</tr>\n");
		//fputs(buffer, fp);
		currentS = currentS->nextSnap;
	}
}


void generateHtml() {
	
	overallSamplesDynamicTable();
	insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\StaticHomePage.html", "dynamic");
		snapshotsDynamicTable();
		insertDynamicData("C:\\Users\\Amir Offir\\VSC-workspace\\Process-Monitor\\Process-Monitor-Design\\HomePage.html", "dynamicTable");


	}


