# drone-targeting

## Overview

This project implements the core calculation module of a drone-based payload release system.

The program processes multiple targets, considers available ammunition types and their costs, and determines a suitable ammunition–target combination based on the criteria defined in the assignment.

The project supports command-line arguments for specifying input and output files, automated batch processing using Bash, and containerized execution using Docker.

## Project Structure

```text
drone-targeting/
├── Dockerfile
├── drop-calc.cpp
├── run-all.sh
├── input/
│   ├── mission-a.txt
│   ├── mission-b.txt
│   └── mission-c.txt
├── output/
│   ├── mission-a-result.txt
│   ├── mission-b-result.txt
│   └── mission-c-result.txt
├── .gitignore
└── README.md
```

The `output/` directory and its contents are generated during execution and are excluded from Git.

## Input Format

The program reads scenario data from a text file specified using the `--input-file` command-line argument.

Each input file contains:

- Release altitude in meters.
- Wind velocity components along the X and Y axes.
- Number of targets.
- Target coordinates, health points (HP), and type.

### Example — `input/mission-a.txt`

```text
120.0
5.0 0.0
4
300.0 400.0 20 LOGISTICS
300.0 450.0 15 PERSONNEL
260.0 380.0 15 PERSONNEL
340.0 380.0 15 PERSONNEL
```

## Output Format

The program writes calculation results to the file specified using the `--output-file` argument.

The output contains:

1. Selected impact coordinates (X, Y).
2. Calculated release coordinates (X, Y), accounting for wind displacement.
3. Selected ammunition type.
4. Number of destroyed targets, total target value, ammunition cost, and net value.

### Example — `output/mission-a-result.txt`

```text
300.00 400.00
275.27 400.00
Frag
4 50 35 15
```

## Usage

### 1. Compile and Run Manually

Compile the program using a C++17-compatible compiler:

```bash
g++ -std=c++17 -Wall -Wextra drop-calc.cpp -o drop_calc
```

Run the executable with the required command-line arguments:

```bash
./drop_calc --input-file input/mission-a.txt --output-file output/mission-a-result.txt
```

The order of the arguments is interchangeable.

Both `--input-file` and `--output-file` are required. Missing or invalid arguments cause the program to terminate with a nonzero exit code.

The output directory must exist before running the executable.

### 2. Batch Processing with Bash

The `run-all.sh` script automatically processes all `.txt` files in the `input/` directory.

Run:

```bash
bash run-all.sh
```

For every input file, the script generates a corresponding output file:

```text
input/mission-a.txt → output/mission-a-result.txt
input/mission-b.txt → output/mission-b-result.txt
input/mission-c.txt → output/mission-c-result.txt
```

The script creates the `output/` directory if necessary.

### 3. Docker

The project includes a Dockerfile based on `debian:12-slim`.

The Docker image installs the required C++ compiler, copies the source code and input files, compiles the program, and executes `run-all.sh` when the container starts.

**Build the Docker image:**

```bash
sudo docker build -t drop-calc .
```

**Create the local output directory:**

```bash
mkdir -p output
```

**Run the Docker container:**

```bash
sudo docker run --rm -v "$PWD/output:/drone-targeting/output:Z" drop-calc
```

The bind mount makes the generated results available in the local `output/` directory.

The `:Z` suffix supports SELinux-enabled systems such as Fedora. On systems without SELinux, it can be omitted.

**Check the results:**

```bash
ls output/
```

## Project Scope

This implementation focuses on the computational core, command-line file handling, automated batch processing, and containerized execution.

Network integration, operator controls, and other components of a complete system remain outside the scope of this assignment.

