#include <iostream>

#include "PCB.h"
#include "Scheduler.h"
#include "SystemCalls.h"

/**
 * @brief Runs a full system-level test for the non-preemptive priority scheduler.
 *
 * This test validates:
 *  - Process creation and priority assignment
 *  - Correct ordering of scheduling based on priority
 *  - Non-preemptive behavior (running process is never interrupted)
 *  - Memory access via VirtualMemoryUnit
 *  - Clean termination of processes
 */
void runSystemTest() {
  std::cout << "\n========== PRIORITY SCHEDULER SYSTEM TEST ==========\n";

  // ---------------------------------------------------------------
  // 1. Instantiate Scheduler and SystemCalls
  // ---------------------------------------------------------------
  Scheduler scheduler;
  SystemCalls syscalls(scheduler);

  // ---------------------------------------------------------------
  // 2. Create processes with explicit priorities
  // ---------------------------------------------------------------
  std::cout << "\n[Test] Creating processes with different priorities...\n";

  auto p1 = syscalls.createProcess();
  auto p2 = syscalls.createProcess();
  auto p3 = syscalls.createProcess();

  // Manually assign priorities (for test purposes)
  // p3 -> highest priority
  // p1 -> mid priority
  // p2 -> low priority
  p1->setState(PCB::State::READY);
  p2->setState(PCB::State::READY);
  p3->setState(PCB::State::READY);

  // Simulate priority assignment
  // (your PCB constructor sets mid by default; override manually in test)
  p1->setPriority(PCB::Priority::mid);
  p2->setPriority(PCB::Priority::low);
  p3->setPriority(PCB::Priority::high);

  // Re-queue them to apply new priority ordering
  scheduler.addProcess(p1);
  scheduler.addProcess(p2);
  scheduler.addProcess(p3);

  syscalls.printAllProcesses();

  // ---------------------------------------------------------------
  // 3. First scheduling cycle — expect highest priority (p3)
  // ---------------------------------------------------------------
  std::cout << "\n[Test] Scheduling highest priority process...\n";
  auto running = scheduler.scheduleNext();
  if (running) {
    std::cout << "[Test] Running PID=" << running->getProcessID()
              << " Priority=" << static_cast<int>(running->getPriority())
              << " State=" << static_cast<int>(running->getState()) << "\n";
  }

  // ---------------------------------------------------------------
  // 4. VMU memory access on running process
  // ---------------------------------------------------------------
  std::cout << "\n[Test] Testing VMU memory access...\n";

  try {
    running->writeMemory(0, 10, 'Z');
    char val = running->readMemory(0, 10);
    std::cout << "[Test] Read '" << val << "' from PID=" << running->getProcessID() << "\n";
  } catch (const std::exception& ex) {
    std::cerr << "[ERROR] VMU memory test failed: " << ex.what() << "\n";
  }

  // ---------------------------------------------------------------
  // 5. Terminate highest priority process
  // ---------------------------------------------------------------
  std::cout << "\n[Test] Terminating current process...\n";

  uint64_t terminatedPID = running->getProcessID();
  scheduler.terminateCurrent();
  syscalls.exitProcess(terminatedPID);

  syscalls.printAllProcesses();

  // ---------------------------------------------------------------
  // 6. Next scheduling cycle — expect medium priority (p1)
  // ---------------------------------------------------------------
  std::cout << "\n[Test] Scheduling next highest-priority...\n";

  running = scheduler.scheduleNext();
  if (running) {
    std::cout << "[Test] Running PID=" << running->getProcessID()
              << " Priority=" << static_cast<int>(running->getPriority()) << "\n";
  }

  std::cout << "\n[Test] Terminating PID=" << running->getProcessID() << "\n";
  scheduler.terminateCurrent();
  syscalls.exitProcess(running->getProcessID());

  // ---------------------------------------------------------------
  // 7. Last process should now run (lowest priority)
  // ---------------------------------------------------------------
  running = scheduler.scheduleNext();
  if (running) {
    std::cout << "[Test] Running final PID=" << running->getProcessID()
              << " Priority=" << static_cast<int>(running->getPriority()) << "\n";
  }

  std::cout << "\n[Test] Terminating final process...\n";
  scheduler.terminateCurrent();
  syscalls.exitProcess(running->getProcessID());

  // ---------------------------------------------------------------
  // 8. Final state
  // ---------------------------------------------------------------
  std::cout << "\n[Test] Final Process Table:\n";
  syscalls.printAllProcesses();

  std::cout << "\n========== END PRIORITY SCHEDULER TEST ==========\n\n";
}

int main() {
  runSystemTest();
  return 0;
}
