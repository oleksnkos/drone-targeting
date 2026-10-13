# drone-targeting

## Overview

This project implements the core calculation module of a drone-based payload release system.
The program processes multiple targets, considers available ammunition types and their costs, and determines a suitable ammunition–target combination based on the criteria defined in the assignment.
It is designed as part of a larger targeting and payload release system, which may later include batch processing, network services, and an operator interface.

## Input Format

The program reads its input from a text file named `scenario.txt`.
The file contains the scenario parameters, including target information required for the calculations.

## Output Format

The program writes the calculation results to `result.txt`.
The output contains the selected target, ammunition type, and the corresponding calculation results.

## Example

**Input — `scenario.txt`**

```text
120.0
5.0 0.0
4
300.0 400.0 20 LOGISTICS
300.0 450.0 15 PERSONNEL
260.0 380.0 15 PERSONNEL
340.0 380.0 15 PERSONNEL
```

**Output — `result.txt`**

```text
300.00 400.00
275.27 400.00
Frag
4 50 35 15
```

## Usage

Compile the program using a C++ compiler and run it with `scenario.txt` available in the working directory.

```bash
g++ main.cpp -o drone_targ
./drone_targ
```

The program generates `result.txt` containing the calculated results.

## Project Scope

This implementation focuses on the computational core and file-based input/output interface. Network integration, operator controls, and other components of the complete system are outside the scope of this assignment.
