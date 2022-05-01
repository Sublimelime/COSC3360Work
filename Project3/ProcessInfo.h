// PROCESSINFO_H
#ifndef PROCESSINFO_H
#define PROCESSINFO_H

struct ProcessInfo {
public:
  int id, pageFaultCount, pageTableSize;
  int *pageTable;

  ProcessInfo(int id, int pageTableSize) {
    this->id = id;
    pageFaultCount = 0;
    this->pageTableSize = pageTableSize;
    pageTable = new int[pageTableSize];
    // init page table to all empty vals
    for (int i = 0; i < pageTableSize; i++)
      pageTable[i] = -1;
  }

  ~ProcessInfo() { delete[] pageTable; }
};

#endif // PROCESSINFO_H
