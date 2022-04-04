// PROCESSINFO_H
#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <algorithm>
#include <string>
#include <vector>

class ProcessInfo {
public:
  // holds commands the process must execute
  std::vector<std::string> commands = std::vector<std::string>();
  // holds max usages of each resource
  std::vector<int> resourceMaximums = std::vector<int>();

  // stats
  int deadline, computationTime;
};

#endif // PROCESSINFO_H
