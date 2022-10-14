int totalSnapshotDlls(struct processinformation* snapHead);
unsigned long memoryUsageAvg(struct snapshotsList* head);
unsigned long totalMemoryUsageCount(struct processinformation* snapHead);
 processinformation* SortByLoadedDlls(struct processinformation* snapHead);
 int highestMemoryUsage(struct processinformation* snapHead);