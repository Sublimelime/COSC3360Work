#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "ProcessInfo.h"

// Convert hex to decimal
// Pulled from https://stackoverflow.com/a/34795119
template <typename T> bool hexToDec(const std::string &hexValue, T &result) {
  std::stringstream ss;
  ss << std::hex << hexValue;
  ss >> result;
  return !ss.fail();
}

int main(int argc, char *args[]) {
  using namespace std;

  if (argc != 2) {
    printf("Started with too many/few arguments, exiting.\n");
    return 1;
  }

  fstream inFile;
  inFile.open(args[1], ios::in);
  if (!inFile.is_open()) {
    printf("Unable to open input file, exiting.\n");
    return 1;
  }

  // integers keeping track of stats
  int totalPageFrames, pageSize, pageFramesPerProc, lookaheadWindowSize,
      minFreePoolSize, maxFreePoolSize, totalProcs;

  // Read file into vector, then close
  auto inputFileStrings = vector<string>();
  string line;
  while (inFile) {
    getline(inFile, line);
    inputFileStrings.push_back(line);
  }
  inFile.close();

  // set statistic ints
  totalPageFrames = stoi(inputFileStrings.at(0));
  pageSize = stoi(inputFileStrings.at(1));
  pageFramesPerProc = stoi(inputFileStrings.at(2));
  lookaheadWindowSize = stoi(inputFileStrings.at(3));
  minFreePoolSize = stoi(inputFileStrings.at(4));
  maxFreePoolSize = stoi(inputFileStrings.at(5));
  totalProcs = stoi(inputFileStrings.at(6));

  // set up ids and processes' info
  auto processes = vector<ProcessInfo>(totalProcs);
  for (int i = 0; i < totalProcs; i++) {
    string line = inputFileStrings.at(7 + i);
    processes.at(i).id = stoi(line.substr(0, 4));
    processes.at(i).pageFramesOnDisk = stoi(line.substr(4));
  }

  //determine alg to use for page replacement
  enum class PageReplaceAlg { FIFO, LRU, LRU_X, LFU, OPT, WS };
  printf("What page replacement algorithm should be used?\nType a number:\n");
  printf("0 - FIFO\n1 - LRU\n2 - LRU-X\n3 - LFU\n4 - OPT\n5 - Working Set\n");
  int alg;
  cin >> alg;
  PageReplaceAlg replaceAlg = (PageReplaceAlg)alg;

  return 0;
}
