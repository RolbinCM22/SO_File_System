#pragma once
#include <variant>
#include <string>

/**
 * @brief Represents the type of instruction executed by a simulated CPU.
 */
enum class InstructionType {
  COMPUTE,     ///< Normal CPU work
  SYS_WRITE,   ///< System call: print text
  SYS_EXIT,    ///< System call: terminate process
  YIELD_CPU    ///< Voluntary yield to scheduler
};

/**
 * @brief Instruction payload for system calls.
 *
 * Some instructions (e.g., SYS_WRITE) need arguments.
 */
struct Instruction {
  InstructionType type;
  std::string argument;  ///< Optional argument for SYS_WRITE

  Instruction(InstructionType t, std::string arg = "")
    : type(t), argument(std::move(arg)) {}
};
