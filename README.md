# drone-targeting

## Overview

This project implements the core calculation module of a drone-based payload release system.

The program processes multiple targets, considers available ammunition types and their costs, and determines a suitable ammunition–target combination based on the criteria defined in the assignment.

The release point is computed by a selectable trajectory model (`hover` or `forward`) hidden behind a common abstract interface. New models can be added without changing the rest of the program.

The project supports command-line arguments for specifying input and output files and the trajectory model, automated batch processing using Bash, and containerized execution using Docker.

## Project Structure

```text
drone-targeting/
├── Dockerfile
├── run-all.sh
├── include/
│   ├── Vec2.h            # 2D vector type with overloaded operators
│   ├── Trajectory.h      # ITrajectorySource interface + Hover / ForwardThrow models
│   ├── Mission.h         # Target, Ammo, Mission (scenario loading)
│   └── StrikePlanner.h   # ammo + impact point selection
├── src/
│   ├── Vec2.cpp
│   ├── Trajectory.cpp
│   ├── Mission.cpp
│   ├── StrikePlanner.cpp
│   └── main.cpp          # argument parsing, model selection, output
├── input/
│   └── ...
├── output/
│   └── ...
├── .gitignore
└── README.md
```

Contents of `output/` are generated during execution and are excluded from Git.

## Trajectory Models

Fall time from altitude: `t = sqrt(2 · altitude / g)`, `g = 9.81 m/s²`.

| Model | Description | Formula |
|---|---|---|
| `hover` (default) | Drone hovers; the payload is drifted only by the wind | `release = impact − wind · t` |
| `forward` | Drone drops while moving at cruise velocity `v_drone = (12.0, 0.0)` m/s; the payload inherits the drone's velocity | `release = impact − (wind + v_drone) · t` |

The model only affects the release point (second output line).

## Input Format

The program reads scenario data from a text file specified using the `--input-file` command-line argument.

Each input file contains:

- Release altitude in meters.
- Wind velocity components along the X and Y axes.
- Number of targets.
- Target coordinates, health points (HP), and type (`PERSONNEL`, `LOGISTICS`, `APC`, `TANK`, `RADAR`).

Invalid input (altitude ≤ 0, target count outside 1–64, HP < 1, unknown target type) causes the program to terminate with a nonzero exit code.

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
2. Calculated release coordinates (X, Y), according to the selected trajectory model.
3. Selected ammunition type.
4. Number of destroyed targets, total target value, ammunition cost, and net value.

### Example — `hover` (default)

```text
300.00 400.00
275.27 400.00
Frag
4 50 35 15
```

### Example — `forward`, same input

```text
300.00 400.00
215.91 400.00
Frag
4 50 35 15
```

## Usage

### 1. Compile and Run Manually

Compile the program using a C++17-compatible compiler:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o drop-calc
```

Run the executable with the required command-line arguments:

```bash
./drop-calc --input-file input/mission-a.txt --output-file output/mission-a-result.txt
```

Select the trajectory model with `--trajectory`:

```bash
./drop-calc --input-file input/mission-a.txt --output-file output/mission-a-forward.txt --trajectory forward
```

The order of the arguments is interchangeable. Missing, unknown, or invalid arguments cause the program to terminate with a nonzero exit code. The output directory must exist before running the executable.

### 2. Batch Processing with Bash

The `run-all.sh` script automatically processes all `.txt` files in the `input/` directory with both trajectory models.

Run:

```bash
bash run-all.sh
```

For every input file, the script generates the corresponding output files:

```text
input/mission-a.txt → output/mission-a-result.txt          (hover)
                    → output/mission-a-forward-result.txt  (forward)
```

The script creates the `output/` directory if necessary.

### 3. Docker

The project includes a Dockerfile based on `debian:12-slim`.

The Docker image installs the required C++ compiler, copies the headers, source files and input files, compiles the program, and executes `run-all.sh` when the container starts.

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