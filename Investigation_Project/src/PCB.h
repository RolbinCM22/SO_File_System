#pragma once
#include <cstdint>
#include <memory>
#include "VirtualMemoryManager/VirtualMemoryUnit.h"
#include "Instruction.h"

/**
 * @class PCB
 * @brief Represents a Process Control Block (PCB) in the operating system.
 *
 * Stores metadata required for scheduling, execution, and memory operations.
 * Each PCB owns a VirtualMemoryUnit for process-specific memory access.
 */
class PCB {
  public:
    /**
		 * @enum State
		 * @brief Execution states for a process.
		 */
		enum class State {NEW = 0, READY = 1, RUNNING = 2, WAITING = 3, TERMINATED = 4};
		/**
		 * @enum Priority
		 * @brief Priority levels for scheduling.
		 */
		enum class Priority {low = 0, mid = 1, high = 2};
		/**
		 * @struct Registers
		 * @brief CPU register snapshot used for context switching.
		 */
		struct Registers {
			uint64_t rbs = 0;
			uint64_t rsp = 0;
			uint64_t rax = 0;
			uint64_t rbx = 0;
			uint64_t rcx = 0;
			uint64_t rdx = 0;
			uint64_t rsi = 0;
			uint64_t rdi = 0;
			uint64_t r8 = 0;
			uint64_t r9 = 0;
			uint64_t r10 = 0;
			uint64_t r11 = 0;
			uint64_t r12 = 0;
			uint64_t r13 = 0;
			uint64_t r14 = 0;
			uint64_t r15 = 0;
		};

	private:
  uint64_t processID;          ///< Unique process identifier.
  State state;                 ///< Current scheduling state.
  uint64_t programCounter;     ///< Simulated instruction pointer.
  Registers registers;         ///< Saved CPU registers.
  Priority priority;           ///< Scheduling priority.
  std::unique_ptr<VirtualMemoryUnit> vmu; ///< Associated virtual memory unit.
	std::vector<Instruction> program;

	public:
	  PCB(uint64_t pid,
      State st,
      uint64_t pc,
      Priority prio,
      std::unique_ptr<VirtualMemoryUnit> vmu_ptr)
    : processID(pid)
		, state(st)
		, programCounter(pc)
		, priority(prio)
		, vmu(std::move(vmu_ptr)) {}

   	// Getters
    [[nodiscard]] uint64_t getProcessID() const { return this->processID; }
    [[nodiscard]] State getState() const { return this->state; }
		[[nodiscard]] Priority getPriority() const { return this->priority; }
    [[nodiscard]] uint64_t getProgramCounter() const { return this->programCounter; }
		[[nodiscard]] std::vector<Instruction> getProgram() const { return this->program; }

    // Setters
		void setState(State st) { this->state = st; }
		void setProgramCounter(uint64_t pc) { this->programCounter = pc; }
		void setPriority(Priority prio) { this->priority = prio; }
		void setProgram(std::vector<Instruction> v) { this->program = v; }

    // Memory access via VMU (delegación)
    char readMemory(size_t vpn, size_t offset) { return this->vmu->read_memory(vpn, offset); }
    void writeMemory(size_t vpn, size_t offset, char value) { this->vmu->write_memory(vpn, offset, value); }
};