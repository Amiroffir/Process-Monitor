#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "GenerateHTML.h"
#include "GenerateHomePage.h"
#include "HtmlDataProcessor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

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

void insertDynamicData(char* fileSrcPath, char Seperator[100], char* fileDestPath) {

	char* homePageTemplate = ReadAllFile(fileSrcPath);

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

	SaveIntoFile(fileDestPath, newFileSpace);
	free(newFileSpace);
	free(homePageTemplate);
}

void insertDataFromFile(char* fileSrcPath, char* fileToInsert, char Seperator[100], char* fileDestPath) {

	char* homePageTemplate = ReadAllFile(fileSrcPath);
	char* tempFileToAdd = ReadAllFile(fileToInsert);

	// find the token
	char* found = strstr(homePageTemplate, Seperator);

	// get the position of the token
	int position = found - homePageTemplate;

	char* newFileSpace = (char*)malloc(strlen(homePageTemplate) + strlen(tempFileToAdd));

	strncpy(newFileSpace, homePageTemplate, position);
	newFileSpace[position] = NULL;
	strcat(newFileSpace, tempFileToAdd);//
	newFileSpace[position + strlen(tempFileToAdd)] = NULL;
	strcat(newFileSpace, found + strlen(Seperator));

	SaveIntoFile(fileDestPath, newFileSpace);
	free(newFileSpace);
	free(homePageTemplate);
	free(tempFileToAdd);
}

void generateHtml() {
	
	generateHomePage();

	}


