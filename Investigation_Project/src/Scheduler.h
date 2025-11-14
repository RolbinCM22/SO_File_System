#pragma once
#include <memory>
#include <queue>
#include <vector>
#include "PCB.h"

/**
 * @class Scheduler
 * @brief Implements a non-preemptive priority-based CPU scheduler.
 *
 * Ready processes are stored in a priority queue ordered by PCB::Priority.
 * Higher priority processes are selected first. The scheduler never preempts a
 * running process; the currently running process continues execution until it
 * terminates or yields voluntarily.
 */
class Scheduler {
private:

  /**
   * @brief Comparator used to order PCBs by priority.
   *
   * A process with higher PCB::Priority value has higher scheduling priority.
   */
  struct PriorityCompare {
    bool operator()(const std::shared_ptr<PCB>& a,
                    const std::shared_ptr<PCB>& b) const {
      return static_cast<int>(a->getPriority()) <
             static_cast<int>(b->getPriority());
    }
  };

  std::priority_queue<
    std::shared_ptr<PCB>,
    std::vector<std::shared_ptr<PCB>>,
    PriorityCompare
  > readyQueue; ///< Priority-ordered ready queue.

  std::shared_ptr<PCB> currentProcess; ///< Currently running process.

public:

  /** @brief Constructs an empty scheduler. */
  Scheduler() : currentProcess(nullptr) {}

  /**
   * @brief Adds a process into the ready queue.
   * @param pcb Process to enqueue.
   */
  void addProcess(const std::shared_ptr<PCB>& pcb) {
    // Sets the initial process state
    pcb->setState(PCB::State::READY);
    this->readyQueue.push(pcb);
  }

  /**
   * @brief Selects the next process to run based on priority.
   *
   * If a process is already running, it is returned (non-preemptive behavior).
   *
   * @return The next scheduled process or nullptr if the system is idle.
   */
  std::shared_ptr<PCB> scheduleNext() {
    if (this->currentProcess != nullptr) {
      return this->currentProcess;
    }
    if (this->readyQueue.empty()) {
      return nullptr;
    }
    auto next = this->readyQueue.top();
    this->readyQueue.pop();
    next->setState(PCB::State::RUNNING);
    this->currentProcess = next;
    return next;
  }

  /**
   * @brief Terminates the currently running process.
   *
   * Sets its state to TERMINATED and frees the CPU.
   */
  void terminateCurrent() {
    if (!this->currentProcess) {
      return;
    }
    this->currentProcess->setState(PCB::State::TERMINATED);
    this->currentProcess = nullptr;
  }

  /**
   * @brief Determines whether all runnable processes are finished.
   * @return True if both ready queue and CPU are empty.
   */
  [[nodiscard]] bool empty() const {
    return this->readyQueue.empty() && this->currentProcess == nullptr;
  }
};
