# Multi-Level Feedback Queue (MLFQ) Scheduler

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Operating Systems](https://img.shields.io/badge/Operating%20Systems-Concept-lightgrey?style=for-the-badge)

![Simulation Screenshot](https://github.com/user-attachments/assets/eeb8f625-0532-421a-8b77-9268830b1265)

## 📌 Overview
This project is a simulation of a **Multi-Level Feedback Queue (MLFQ)** CPU scheduling algorithm, implemented in **C**. 

It demonstrates how Operating Systems prioritize processes by dynamically adjusting their priority levels based on their CPU burst behavior and wait times. The goal is to optimize **Turnaround Time** and minimize **Response Time** while preventing process starvation.

## ⚙️ Key Features
* **Dynamic Priority Adjustment:** Processes move between queues based on their execution history (CPU-bound vs. I/O-bound).
* **Aging Mechanism:** Implemented to prevent starvation by gradually boosting the priority of low-level processes that have waited too long.
* **Context Switching Simulation:** Simulates the overhead and logic of saving/loading process states.
  
## 🛠️ Tech Stack
* **Language:** C
* **Core Concepts:** Process Control Blocks (PCB), Queues, CPU Scheduling Algorithms.
* **Libraries:** FreeRTOS
 
## 🧠 What I Learned * Deepened my understanding of how kernels manage process execution and resource allocation. * Gained experience with low-level data structures in C (handling queues). * learned how to implement "Aging" to solve the Starvation problem in priority-based scheduling. * Learned how to effectively use FreeRTOS libraries and implementations.  --- *This project was developed as part of the Operating Systems course at Sakarya University.*

## 🚀 How To Run

### 1. Compilation
If you need to recompile the source code, use `make` (if a Makefile is provided) or `gcc`:
```bash
gcc -o freertos_sim main.c  # Ensure you link necessary FreeRTOS libraries

