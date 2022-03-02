// VERTEXPROCESS_H
#ifndef VERTEXPROCESS_H
#define VERTEXPROCESS_H

#include <vector>

class VertexProcess {
public:
  int id;
  bool isInitialInput; // Determines whether the process is an initial input
                       // that will be inputting from file2
  bool isOutput; // determines whether the process is an output that will be
                 // printing

  // holds a list of pipe ids that the process should input from and output onto
  std::vector<int> inputs = std::vector<int>();
  std::vector<int> outputs = std::vector<int>();

  VertexProcess(int id, bool isInput, bool isOutput)
      : id(id), isInitialInput(isInput), isOutput(isOutput) {}

  // Configures `inputs` depending on provided matrix.
  void determineInputs(const std::vector<std::vector<int>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
      if (matrix[i][id] == 1)
        inputs.push_back(i);
    }
  }

  // Configures `outputs` depending on provided matrix.
  void determineOutputs(const std::vector<std::vector<int>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
      if (matrix[id][i] == 1)
        outputs.push_back(i);
    }
  }
};

#endif // VERTEXPROCESS_H
