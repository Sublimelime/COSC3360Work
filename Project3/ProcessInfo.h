// PROCESSINFO_H
#ifndef PROCESSINFO_H
#define PROCESSINFO_H

class ProcessInfo {
public:
  int id, pageFramesOnDisk;
  ProcessInfo(int id, int pageFramesOnDisk)
      : id(id), pageFramesOnDisk(pageFramesOnDisk) {}
};

#endif // PROCESSINFO_H
