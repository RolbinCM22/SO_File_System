#pragma once
#include <iostream>
#include "Scheduler.h"
#include "SystemCalls.h"
#include "Instruction.h"

/**
 * @class CPU
 * @brief Simulates the CPU execution loop of a tiny OS.
 *
 * The CPU fetches processes from the scheduler and executes their
 * instructions one-by-one. Each instruction may trigger system calls
 * through SystemCalls.
 */
class CPU {
private:
  Scheduler& scheduler;
  SystemCalls& syscalls;

public:
  CPU(Scheduler& sch, SystemCalls& sc)
    : scheduler(sch), syscalls(sc) {}

  /**
   * @brief Runs one CPU cycle: fetch, decode, execute.
   */
  void step() {
    auto pcb = scheduler.scheduleNext();
    if (!pcb) {
      std::cout << "[CPU] No process to run.\n";
      return;
    }

    // Fetch next instruction
    uint64_t pc = pcb->getProgramCounter();
    auto program = pcb->getProgram();

    if (pc >= program.size()) {
      std::cout << "[CPU] Program finished. Terminating PID=" << pcb->getProcessID() << "\n";
      scheduler.terminateCurrent();
      syscalls.exitProcess(pcb->getProcessID());
      return;
    }

    const Instruction& inst = program[pc];
    pcb->setProgramCounter(pc + 1);

    // Decode + Execute
    executeInstruction(pcb, inst);
  }

private:
  /**
   * @brief Executes one instruction for a specific process.
   */
  void executeInstruction(const std::shared_ptr<PCB>& pcb,
                          const Instruction& inst) {
    switch (inst.type) {

      case InstructionType::COMPUTE:
        std::cout << "[CPU] PID " << pcb->getProcessID()
                  << " executed COMPUTE.\n";
        break;

      case InstructionType::SYS_WRITE:
        std::cout << "[SysCall] PID " << pcb->getProcessID()
                  << " says: " << inst.argument << "\n";
        break;

      case InstructionType::YIELD_CPU:
        std::cout << "[CPU] PID " << pcb->getProcessID()
                  << " yielded the CPU.\n";
        scheduler.terminateCurrent();
        break;

      case InstructionType::SYS_EXIT:
        std::cout << "[CPU] PID " << pcb->getProcessID()
                  << " called SYS_EXIT.\n";
        scheduler.terminateCurrent();
        syscalls.exitProcess(pcb->getProcessID());
        break;
    }
  }
};
