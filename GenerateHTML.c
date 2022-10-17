#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include "GenerateHTML.h"
#include "GenerateHomePage.h"
//#include "HtmlDataProcessor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)

/// <summary>
///  This function saves a char pointer to a file
/// </summary>
/// <param name="fileName"></param>
/// <param name="buff"></param>
int SaveIntoFile(char* fileName, char* buff)
{
	FILE* fi = fopen(fileName, "w");
	if (!fi)
	{
		printf("Error opening file");
		LogError(strcat(strerror(GetLastError),"\n"));
		return 0;
	}
	LogEvent("File opened successfully for writing\n");
	fputs(buff, fi);


	fclose(fi);
}

/// <summary>
	///  This function reads a file and returns a char pointer
	/// </summary>
	/// <param name="fileName"></param>
char* ReadAllFile(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		printf("Error opening file\n");
		LogError(strcat(strerror(GetLastError),"\n"));
		return NULL;
	}
	LogEvent("File opened successfully for reading\n");

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
		printf("Error opening file");
		LogError(strcat(strerror(GetLastError),"\n"));
		return NULL;
	}
	LogEvent("File opened successfully for reading\n");
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

/// <summary>
	///  This function saves a char pointer to a data file
	/// </summary>
int saveTempFile(char* fileName, char* buff, char fileAction[5])
{
	FILE* fi = fopen(fileName, fileAction); // fileAction - "w" for write, "a" for append
	if (!fi)
	{
		printf("Error opening file\n");
		LogError(strcat(strerror(GetLastError),"\n"));
		return 0;
	}
	LogEvent("File opened successfully for writing\n");
	fputs(buff, fi);


	fclose(fi);
}

/// <summary>
	///  This function inserts dynamic data into a html file
	/// </summary>
void insertDynamicData(char* fileSrcPath, char Seperator[100], char* fileDestPath) {
	
	char* homePageTemplate = ReadAllFile(fileSrcPath); // Read the template/partly updated file

	// find the token
	char* found = strstr(homePageTemplate, Seperator); 

	// get the position of the token
	int position = found - homePageTemplate;

	
	char* newFileSpace = (char*)malloc(strlen(homePageTemplate) + strlen(buffer));
	strncpy(newFileSpace, homePageTemplate, position);
	newFileSpace[position] = NULL;
	
	strcat(newFileSpace, buffer); // insert the dynamic data
	
	newFileSpace[position + strlen(buffer)] = NULL;
	strcat(newFileSpace, found + strlen(Seperator));

	SaveIntoFile(fileDestPath, newFileSpace); // save the new file with the dynamic data inserted
	free(newFileSpace);
	free(homePageTemplate);
}

/// <summary>
	///  This function inserts data from a file into a html file
	/// </summary>
	/// <param name="fileSrcPath"> template/partly updated html page </param>
	/// <param name="fileToInsert"> file with the desired data </param>
	/// <param name="Seperator"> token </param>
	/// <param name="fileDestPath"> desired path for the updated html page</param>
void insertDataFromFile(char* fileSrcPath, char* fileToInsert, char Seperator[100], char* fileDestPath) {
	
	char* homePageTemplate = ReadAllFile(fileSrcPath); // Read the template/partly updated file
	char* tempFileToAdd = ReadAllFile(fileToInsert); // Read the file with the desired data

	// find the token
	char* found = strstr(homePageTemplate, Seperator);

	// get the position of the token
	int position = found - homePageTemplate;
	// alloc space for the new file
	char* newFileSpace = (char*)malloc(strlen(homePageTemplate) + strlen(tempFileToAdd)); 

	strncpy(newFileSpace, homePageTemplate, position); // copy the first part of the template file
	newFileSpace[position] = NULL;
	strcat(newFileSpace, tempFileToAdd); // insert the desired data
	newFileSpace[position + strlen(tempFileToAdd)] = NULL;
	strcat(newFileSpace, found + strlen(Seperator)); // insert the rest of the template

	SaveIntoFile(fileDestPath, newFileSpace); // save the new file with the desired data inserted
	free(newFileSpace);
	free(homePageTemplate);
	free(tempFileToAdd);
}

/// <summary>
	///  This function generates the html pages	
	/// </summary>
void generateHtml() {
	
	generateHomePage(); 

	}


