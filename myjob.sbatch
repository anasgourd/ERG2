#!/bin/bash

#SBATCH --partition=batch

#SBATCH --ntasks-per-node=8  # Adjust as needed

#SBATCH --nodes=4

#SBATCH --time=1:30:00       # Adjust as needed



module load gcc openmpi  # Load necessary modules

mpicc -o main ks.cpp -lm -lstdc++  # Compile the MPI program

srun -n 4 ./main 1024000 4 
