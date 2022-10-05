#include <Windows.h>
#include <time.h>
typedef struct DllInfo {
	char dllName[MAX_PATH];
	struct dllInfo* next;
	struct dllInfo* prev;
}dllInfo;
typedef struct MemoryInfo {
	unsigned long long PageFaultCount;
	unsigned long long WorkingSetSize;
	unsigned long long QuotaPagedPoolUsage;
	unsigned long long QuotaPeakPagedPoolUsage;
	unsigned long long PagefileUsage;
} memoryinfo;
typedef struct ProcessInfo {
	unsigned int loadedProcesses;
	char processName[MAX_PATH];
	unsigned int processID;
	memoryinfo memoryinfo;
	dllInfo* dllInfo;
	unsigned int loadedDlls;
	char snapshotTime[100];
	unsigned int snapshotCounter;
	struct processinformation* next;
	struct  processinformation* prev;
}processinformation;
typedef struct SnapshotList {
	processinformation* snapshotData;
	struct snapshotsList* nextSnap;
	struct snapshotsList* prevSnap;
}snapshotsList;

struct processinformation* GetMemoryInfo(DWORD processID);
struct processinformation* GetProcessesInfo();
unsigned int snapshotCounter;



