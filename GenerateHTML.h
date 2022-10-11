void generateHtml();
int SaveIntoFile(char* fileName, char* buff);
char* ReadAllFile(char* fileName);
void insertDynamicData(char* fileSrcPath, char Seperator[100], char* fileDestPath);
void insertDataFromFile(char* fileSrcPath, char* fileToInsert, char Seperator[100], char* fileDestPath);
char buffer[100000];
char converter[1000];