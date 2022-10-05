typedef struct snapshotList_Header {
	int version;
	int snapshotsCount;
	char reserve[100];
} snapList_Header; 
 snapList_Header header;

 void saveToFile();
 void loadFromFile();
