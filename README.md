# parallel-kth-mpi

This repository contains a **parallel MPI implementation** to find the **k-th smallest element** in a large dataset. It demonstrates distributed computation using **weighted median selection** and MPI.This project was developed for the Parallel and Distributed Systems course at ECE AUTH (2023–2024).

## Contents

- **`file_gen.cpp`** – Generates a large dataset of random integers (`output1mil.txt`).  
- **`ks.cpp`** – Parallel algorithm to find the k-th smallest element using MPI.  
- **`myjob.sbatch`** – SLURM batch script for running `ks.cpp` on multiple nodes/cores.

## Requirements

- GCC compiler  
- OpenMPI  
- SLURM and access to an HPC cluster for full parallel execution

## Results

All experiment results, including execution times, number of MPI processes, and k-th smallest elements found, are available in a separate repository: [Results Repository](https://github.com/anasgourd/results).  

- Organized by dataset size and experiment configuration.  
- Useful to evaluate parallel performance, scalability and correctness of the algorithm.

## Usage

Run on HPC with SLURM:
```bash
sbatch myjob.sbatch
```
