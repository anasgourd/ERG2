# parallel-kth-mpi

This repository contains a **parallel MPI implementation** to find the **k-th smallest element** in a large dataset. It demonstrates distributed computation using **weighted median selection** and MPI.

## Contents

- **`file_gen.cpp`** – Generates a large dataset of random integers (`output1mil.txt`).  
- **`ks.cpp`** – Parallel algorithm to find the k-th smallest element using MPI.  
- **`myjob.sbatch`** – SLURM batch script for running `ks.cpp` on multiple nodes/cores.

## Requirements

- GCC compiler  
- OpenMPI  
- (Optional) SLURM and access to an HPC cluster for full parallel execution

## Usage

Run on HPC with SLURM:
```bash
sbatch myjob.sbatch
```
