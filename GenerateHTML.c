#include "MyUtilities.h"
#include "ProcessesLinkedList.h"
#include "GetProcessesInfo.h"
#include "saveAndLoad.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable:4996)


void generateHtml() {
	{
		FILE* fp;
		fp = fopen("HomePage.html", "w");
		if (fp == NULL) {
			printf("Error opening file");
			// write to log
			return;
		}
		/*fputs("<!DOCTYPE html>\n", fp);
		
		fputs("<html lang=\"en\">\n", fp);
		fputs("<head>\n", fp);
		fputs("<meta charset=\"UTF-8\">\n", fp);
		fputs("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n", fp);
		fputs("<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n", fp);
		fputs("<link rel=\"stylesheet\" href=\"C:\\Users\\Amir Offir\\.vscode\\Process-Monitor\\HomePage.css\">\n", fp);
		fputs("<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384 - Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF / et3URy9Bv1WTRi\"	crossorigin = \"anonymous\" >\n", fp);
		fputs("<title>Process Monitor</title>\n", fp);
		fputs("<nav class=\"navbar navbar - dark navbar - expand - lg bg - dark\">\n",fp);
		fputs("<a class=\"navbar-brand\" id=\"p-name\" href=\"#\">", fp);
		fputs("Process Monitor</a>\n", fp);
		fputs("<a class=\"navbar-brand border-bottom\" href=\"#\">", fp);
		fputs("<img src=\"https://www.flaticon.com/svg/static/icons/svg/2919/29190.svg\"width=\"30\" height=\"30\" class=\"d-inline-block align-top\" alt=\"Home Icon\">", fp);
		fputs("Home</a>\n", fp);
		fputs("<a class=\"navbar-brand border-bottom\" href=\"#\">", fp);
		fputs("<img src=\"https://www.flaticon.com/svg/static/icons/svg/2919/29190.svg\"width=\"30\" height=\"30\" class=\"d-inline-block align-top\" alt=\"3pass Icon\">", fp);
		fputs("About</a>\n", fp);
		fputs("</nav>\n", fp);
		fputs("<h1 class=\"border-bottom\">", fp);
		fputs("Processes</h1>\n", fp);
		fputs("</head>\n", fp);
		fputs("<body>\n", fp);
		fputs("<h1>Processes</h1>\n", fp);
		fputs("<table border=\"1\">\n", fp);
		fputs("<tr>\n", fp);
		*/
		snapshotsList* currentS = snapshotListHead;
		char buffer[64] = { 0 };
		for (int i = 0; i <= snapshotCounter; i++) {
			if (i != 0) {
				fputs("<th>Snapshot ", fp);
				sprintf(buffer,"%d",i);
				fputs(buffer, fp);
				fputs("</th>\n", fp);
			}
			else {
				fputs("<th>		", fp);
				fputs("</th>\n", fp);

			}
		}
		fputs("</tr>\n", fp);
		fputs("<tr>\n", fp);
		fputs("<td>Total processes</td>\n", fp);
		while (currentS != NULL) {
			fputs("<td>", fp);
			sprintf(buffer, "%d",processesCount(currentS->snapshotData));
			fputs(buffer, fp);
			fputs("</td>\n", fp);
			currentS = currentS->nextSnap;
		}
		fputs("</tr>\n", fp);
		fputs("</table>\n", fp);
		fputs("</body>\n", fp);
		fputs("</html>\n", fp);
		fclose(fp);
	
	}
}

