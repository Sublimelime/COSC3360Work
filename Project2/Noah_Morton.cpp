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

#include "HelperFunctions.h"
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
      int resource = commandFileLines.at(i)[10] - '0';
      int maximum = commandFileLines.at(i)[13] - '0';
      resourcesAvailable.at(resource) = maximum;
    } else if (commandFileLines.at(i).substr(0, 2) == "max") {
      // going to just assume no more than 9, one digit
      int process = commandFileLines.at(i)[4] - '0';
      int resource = commandFileLines.at(i)[6] - '0';
      int resourceMax = commandFileLines.at(i)[9] - '0';
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
    if (line.substr(0, 7) == "process") {
      processBeingRead = line[8] - '0';
      processBeingRead--; // starts 1 indexed, so reduce to 0
      cout << "Switching to new process " << processBeingRead << endl;
    } else if (lineStartsWithNum(line)) {
      // got deadline + comp time, read next line too
      auto &currentProcess = processes.at(processBeingRead);
      currentProcess.deadline = stoi(line);
      line = commandFileLines.at(i + 1); // next line
      currentProcess.computationTime = stoi(line);
      i++; // skip over

      // sanity check
      if (currentProcess.deadline < currentProcess.computationTime) {
        printf(
            "Process %d's computation time exceeds deadline. Impossible input.",
            processBeingRead);
        return 0;
      }

    } else { // got a command, add it to list
      processes.at(processBeingRead).commands.push_back(line);
    }
  }

  return 0;

  int pnum = -1;
  int pid;
  for (int k = 0; k < processes.size(); k++) {
    pid = fork();
    if (pid == 0) {
      pnum = k;
      break;
    }
  }

  // clean up stuff TODO
}
