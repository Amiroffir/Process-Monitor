int totalSnapshotDlls(struct processInformation* snapHead);
unsigned long memoryUsageAvg(struct snapshotsList* head);
unsigned long long totalMemoryUsageCount(struct processInformation* snapHead);
 processInformation* SortByLoadedDlls(struct processInformation* snapHead);
 unsigned long long highestMemoryUsage(struct processInformation* snapHead);