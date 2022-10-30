#include <windows.h>
typedef struct DllInfo {
	char dllName[MAX_PATH];
	struct DllInfo* next;
	struct DllInfo* prev;
}dllInfo;
typedef struct MemoryInfo {
	unsigned long long PageFaultCount;
	unsigned long long WorkingSetSize;
	unsigned long long QuotaPagedPoolUsage;
	unsigned long long QuotaPeakPagedPoolUsage;
	unsigned long long PagefileUsage;
} memoryinfo;
typedef struct processInfo {
	char processName[MAX_PATH];
	unsigned int processID;
	memoryinfo memoryinfo;
	dllInfo* dllInfo;
	unsigned int totalLoadedDlls;
	char snapshotTime[100];
	unsigned int snapshotCounter;
	struct processInfo* next;
	struct  processInfo* prev;
}processInformation;

typedef struct SnapshotList {
	processInformation* snapshotData;
	struct SnapshotList* nextSnap;
	struct SnapshotList* prevSnap;
}snapshotsList;

struct processInformation* GetMemoryInfo(DWORD processID);
struct processInformation* GetProcessesInfo();
unsigned int snapshotCounter;



