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
  // holds max usages of each resource with a pair - <1,1> means max usage 1 of
  // resource 1
  std::vector<std::pair<int, int>> resources =
      std::vector<std::pair<int, int>>();

  // stats
  int deadline, computationTime;
};

#endif // PROCESSINFO_H
