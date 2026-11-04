# Distributed System Lab

This repository contains C programs and practical implementations developed for the Distributed System Lab using MPI (Message Passing Interface).

## 1. Environment Setup (Ubuntu)

Follow these steps on a fresh Ubuntu installation.

### Step 1: Update the System

```bash
sudo apt update
sudo apt upgrade -y
```

### Step 2: Install GCC Compiler and Build Tools

```bash
sudo apt install build-essential -y
```

### Step 3: Install OpenMPI

```bash
sudo apt install openmpi-bin libopenmpi-dev -y
```

### Step 4: Verify Installation

Check whether GCC and OpenMPI are installed correctly.

```bash
gcc --version
mpicc --version
mpirun --version
```

## 2. Compile and Run an MPI Program

Navigate to the directory containing your C source file.

### Step 1: Compile

Use `mpicc` to compile an MPI program.

```bash
mpicc Problem_1.c -o Problem_1
```

### Step 2: Run

Use `mpirun` to execute the program with the required number of processes.

```bash
mpirun -np 4 ./Problem_1
```

**Command Explanation:**

- `mpicc`: MPI C compiler.
- `Problem_1.c`: Source code file.
- `-o Problem_1`: Name of the executable file.
- `mpirun`: Executes the MPI program.
- `-np 4`: Runs the program using 4 processes.
- `./Problem_1`: Executable file to run.

**Note:** Change the source filename and number of processes according to the requirements of each problem.

## 3. Important Commands

| Command | Purpose |
|---|---|
| `sudo apt update` | Updates package lists |
| `sudo apt install build-essential -y` | Installs GCC and build tools |
| `sudo apt install openmpi-bin libopenmpi-dev -y` | Installs OpenMPI |
| `gcc --version` | Checks GCC installation |
| `mpicc --version` | Checks MPI compiler |
| `mpirun --version` | Checks MPI runtime |

**Important:** Always use `mpicc` instead of `gcc` to compile MPI programs.