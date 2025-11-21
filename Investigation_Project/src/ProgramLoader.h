#pragma once
#include <vector>
#include "Instruction.h"

/**
 * @brief Provides utilities to build simple "programs" for PCBs.
 *
 * Each program is a sequence of instructions that a CPU will execute.
 */
class ProgramLoader {
public:
  /**
   * @brief Builds a simple test program.
   *
   * @return Vector of simulated instructions.
   */
  static std::vector<Instruction> buildDemoProgram() {
    return {
      Instruction(InstructionType::COMPUTE),
      Instruction(InstructionType::COMPUTE),
      Instruction(InstructionType::SYS_WRITE, "Hola desde el proceso"),
      Instruction(InstructionType::COMPUTE),
      Instruction(InstructionType::YIELD_CPU),
      Instruction(InstructionType::SYS_EXIT)
    };
  }
};
