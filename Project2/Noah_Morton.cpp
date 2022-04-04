#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "ProcessInfo.h"

int main(int argc, char **args) {
  using namespace std;

  if (argc != 3) {
    printf("Started with too many/few arguments, exiting.\n");
    return 1;
  }

  fstream inFile1, inFile2;
  inFile1.open(args[1], ios::in);
  if (!inFile1.is_open()) {
    printf("Unable to open matrix input file, exiting.\n");
    return 1;
  }

  // holds lines from `many` file, used to setup program
  auto commandFileLines = vector<string>();
  string line;

  // read in all lines from file
  while (inFile1) {
    getline(inFile1, line);
    if (line != "") // ignore empty lines
      commandFileLines.push_back(line);
  }
  inFile1.close();

  // Keeps track of max resources available for each resource
  auto resourcesAvailable = vector<int>(stoi(commandFileLines.at(0)), 0);
  // holds ProcessInfos of each process
  auto processes = vector<ProcessInfo>(stoi(commandFileLines.at(1)));

  // read available and max usage of resources per process, store this info
  int processInstructionBeginPoint;
  for (int i = 2; i < commandFileLines.size(); i++) {
    if (commandFileLines.at(i).substr(0, 8) == "available") {
      // going to just assume no more than 9
      int resource = commandFileLines.at(i)[10];
      int maximum = commandFileLines.at(i)[13];
      resourcesAvailable.at(resource) = maximum;
    } else if (commandFileLines.at(i).substr(0, 2) == "max") {
      // going to just assume no more than 9, one digit
      int process = commandFileLines.at(i)[4];
      int resource = commandFileLines.at(i)[6];
      int resourceMax = commandFileLines.at(i)[9];
      processes.at(process).resourceMaximums.at(resource) = resourceMax;
    } else { // found where the process instructions begin
      processInstructionBeginPoint = i;
      break;
    }
  }

  // read each process's commands, deadline and processing time into its
  // ProcessInfo
  int processBeingRead = 0;
  for (int i = processInstructionBeginPoint; i < commandFileLines.size(); i++) {
    string line = commandFileLines.at(i);
    if (line.substr(0, 6) == "process") {
      processBeingRead = line[8];
    } else if (line.substr(0, 7) == "deadline") {
      processes.at(processBeingRead).deadline = line[9];
    } else { //got a command
      processes.at(processBeingRead).commands.push_back(line);
    }
  }

  int pnum = -1;
  int pid;
  for (int k = 0; k < processes.size(); k++) {
    pid = fork();
    if (pid == 0) {
      pnum = k;
      break;
    }
  }
}
