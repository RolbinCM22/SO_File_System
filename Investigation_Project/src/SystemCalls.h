// SystemCalls.h

#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>
#include "Scheduler.h"
#include "PCB.h"
#include "VirtualMemoryManager/VirtualMemoryUnit.h"
#include "ProgramLoader.h"


/**
 * @class SystemCalls
 * @brief Provides high-level system call operations for process management.
 *
 * This class encapsulates OS-level actions such as creating processes,
 * terminating them, and printing the process table. It interacts directly with
 * the Scheduler to enqueue new processes and maintains an internal process table.
 */
class SystemCalls {
private:
	Scheduler& scheduler;   ///< Reference to the system scheduler.
	PhysicalMemoryManager physMgr;
	BackingStoreManager bsm;
	PageFaultHandler pfh;

	uint64_t nextProcessID = 1;   ///< Auto-incremented PID counter.
	std::unordered_map<uint64_t, std::shared_ptr<PCB>> processTable;  ///< Active processes table.

public:
  /**
   * @brief Constructs a SystemCalls interface bound to a scheduler.
   * @param sch Reference to the scheduler responsible for process execution.
   */
	explicit SystemCalls(Scheduler& sch) : scheduler(sch), pfh(physMgr, bsm){}

	/**
	 * @brief Creates a new process and registers it in the system.
	 * @return std::shared_ptr<PCB> Pointer to the newly created process.
	 */
	std::shared_ptr<PCB> createProcess() {
		// auto ptm = PageTableManager(this->pfh);
		// Creates a unique vmu for the process.
		auto vmu = std::make_unique<VirtualMemoryUnit>(pfh, physMgr);
		// PCB creation.
		auto pcb = std::make_shared<PCB>(
			nextProcessID++, PCB::State::NEW,
			0, PCB::Priority::mid, std::move(vmu)
		);
		pcb->setState(PCB::State::READY);
		// Saves the pcb on the process table
		processTable[pcb->getProcessID()] = pcb;
		scheduler.addProcess(pcb);
		pcb->setProgram(ProgramLoader::buildDemoProgram());
		std::cout << "[SysCall] Created process (PID=" << pcb->getProcessID() << ")\n";
		return pcb;
	}

	  /**
   * @brief Terminates a process given its PID and removes it from the system.
   * @param pid Identifier of the process to terminate.
   */
	void exitProcess(uint64_t pid) {
		auto it = processTable.find(pid);
		if (it == processTable.end()) {
			std::cerr << "[SysCall] Exit: Process PID=" << pid << " not found\n";
			return;
		}
		it->second->setState(PCB::State::TERMINATED);
		std::cout << "[SysCall] Process terminated (PID=" << pid << ")\n";
		processTable.erase(it);
	}

	/**
   * @brief Prints all currently registered processes and their states.
   *
   */
	void printAllProcesses() const {
		std::cout << "\n[SysCall] Current system processes:\n";
		for (const auto& [pid, pcb] : processTable) {
			std::cout << " PID=" << pid << " State=" << static_cast<int>(pcb->getState()) << "\n";
		}
	}
};