typedef struct snapshotList_Header {
	int version;
	int snapshotsCount;
	char reserve[100];
} snapList_Header; 
 snapList_Header header;
 typedef struct processesList_Header {
	 int loadedProcesses;
	 char reserve[100];
 } processesList_Header;
 processesList_Header process_header;
 typedef struct dllsList_Header {
	 int privateLoadedDlls;
	 char reserve[100];
 } dllsList_Header;
 dllsList_Header dlls_header;

 void saveToFile();
 void loadFromFile();
