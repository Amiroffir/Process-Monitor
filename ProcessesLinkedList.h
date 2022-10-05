struct processinformation* addProcess(struct processinformation* current);
struct dllInfo* addDllToList(struct dllInfo* current);
struct processinformation* addExtraProcess(struct processinformation* destList, struct processinformation* addToList);
struct dllInfo* extraDllsCheck(struct dllInfo* destDll, struct dllInfo* dllToAdd);
struct dllInfo* addExtraDlls(struct dllInfo* destDlls, struct dllInfo* dllToAdd);
void addSnapshot(struct processinformation* currentSnapshot);

void printProcessesList(struct processinformation* current);
void printSnapshots();

void resetSnapshotCollection();
void listInit();
void dllListInit();

struct SnapshotsList* snapshotListHead;
struct processinformation* tempSnapHead;
struct processinformation* oneSnapHead;
struct processinformation* twentySnapHead;
struct processinformation* longSnapHead;