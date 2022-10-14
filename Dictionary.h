typedef struct process {
	char processName[MAX_PATH];
	unsigned int processID;
	struct process* next;
	struct process* prev;
}processDict;
typedef struct dll {
	char dllName[MAX_PATH];
	int usedByCount;
	processDict* usedBy;	//pointer to the processes that uses this dll
	struct dll* next;
	struct dll* prev;
}dllDict;

int PCounter;
int DCounter;

void addProcToDict(char processName[MAX_PATH], unsigned int processID);
void printProcDictionary();
struct processDict* searchProcess(char processName[MAX_PATH], unsigned int processID);
struct processDict* addProcessToUsedBy(struct processDict* usedByHead, struct processinformation* processToAdd);

void addDllToDict(char dllName[MAX_PATH], struct processinformation* proc);
void printDllDictionary();
struct dllDict* searchDll(char dllName[MAX_PATH], struct processinformation* processToAdd);

struct processDict* procDictHead;
struct dllDict* dllDictHead;