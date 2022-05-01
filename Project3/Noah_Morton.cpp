#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

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

  return 0;
}
