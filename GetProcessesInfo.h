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
typedef struct ProcessInfo {
	char processName[MAX_PATH];
	unsigned int processID;
	memoryinfo memoryinfo;
	dllInfo* dllInfo;
	unsigned int totalLoadedDlls;
	char snapshotTime[100];
	unsigned int snapshotCounter;
	struct ProcessInfo* next;
	struct  ProcessInfo* prev;
}processinformation;

typedef struct SnapshotList {
	processinformation* snapshotData;
	struct SnapshotList* nextSnap;
	struct SnapshotList* prevSnap;
}snapshotsList;

struct processinformation* GetMemoryInfo(DWORD processID);
struct processinformation* GetProcessesInfo();
unsigned int snapshotCounter;



