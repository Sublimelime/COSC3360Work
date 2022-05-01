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


// Convert hex letter to decimal
int hexLetterToDec(char letter) {
  switch (letter) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  case 'A':
    return 10;
  case 'B':
    return 11;
  case 'C':
    return 12;
  case 'D':
    return 13;
  case 'E':
    return 14;
  case 'F':
    return 15;
  }
  return -1;
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

  // determine alg to use for page replacement
  enum class PageReplaceAlg { FIFO, LRU, LRU_X, LFU, OPT, WS };
  printf("What page replacement algorithm should be used?\nType a number:\n");
  printf("0 - FIFO\n1 - LRU\n2 - LRU-X\n3 - LFU\n4 - OPT\n5 - Working Set\n");
  int alg;
  cin >> alg;
  PageReplaceAlg replaceAlg = (PageReplaceAlg)alg;

  // forking
  int pnum = -1;
  for (int k = 0; k < 3; k++) {
    if (fork() == 0) {
      pnum = k;
      break;
    }
  }

  switch (pnum) {
  case -1: // Parent
    break;
  case 0: // Page fault replacer
    break;
  case 1: // Hard drive
    break;
  case 2: // page fault handler
    break;
  }
  return 0;
}
