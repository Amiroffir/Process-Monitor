struct processInformation* addProcess(struct processInformation* current);
struct dllInfo* addDllToList(struct dllInfo* current);
struct processInformation* addExtraProcess(struct processInformation* destList, struct processInformation* addToList);
struct dllInfo* extraDllsCheck(struct dllInfo* destDll, struct dllInfo* dllToAdd);
struct dllInfo* addExtraDlls(struct dllInfo* destDlls, struct dllInfo* dllToAdd);
void addSnapshot(struct processInformation* currentSnapshot);

// Utility function
void printSnapshots();

void resetSnapshotCollection();
void processesListInit();
void dllListInit();

struct SnapshotsList* snapshotListHead; // global head of the snapshots list
struct processInformation* tempSnapHead;
struct processInformation* oneSnapHead;
struct processInformation* twentySnapHead;
struct processInformation* longSnapHead;