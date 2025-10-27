# Pthread Assignment Setup Guide

This guide will help you set up your development environment for the pthread assignment using Docker. Since you've already completed the xv6 assignment, this guide focuses primarily on Docker-specific setup and workflows for this assignment.

## Overview

This assignment involves implementing multi-threaded programs using POSIX threads (pthreads) in C++. Unlike xv6, which required QEMU, this assignment runs natively in a containerized Linux environment using Docker.

## Prerequisites

### Installing Docker Desktop

Download and install Docker Desktop from [https://www.docker.com/products/docker-desktop/](https://www.docker.com/products/docker-desktop/).

**Important notes**:
- **Windows**: Docker Desktop requires WSL 2 (Windows Subsystem for Linux). The installer will guide you through enabling it.
- **Linux**: You may need to add your user to the docker group: `sudo usermod -aG docker $USER`, then log out and log back in.

After installation, verify it works by running:
```bash
docker --version
docker-compose --version
```

## Repository Setup

### 1. Clone the Repository

Clone the shared pthread repository:

```bash
git clone https://git.lsalab.cs.nthu.edu.tw/os25/os25_shared_pthread.git pthread
cd pthread
```

### 2. Configure Git Line Endings (Critical for Windows Users!)

**Windows users MUST do this step** to avoid the error `/usr/bin/env: 'python3\r': No such file or directory`:

```bash
# For PowerShell/CMD
git config core.autocrlf false
git rm --cached -r .
git reset --hard
```

macOS and Linux users should also run this to ensure consistency:

```bash
git config core.autocrlf false
```

### 3. Repository Structure

```plain
pthread/
├── Makefile                      # Build configuration
├── docker-compose.yml            # Docker setup
├── main.cpp                      # Main program (to be completed)
├── ts_queue.hpp                  # Thread-safe queue (to be completed)
├── producer.hpp                  # Producer thread (to be completed)
├── consumer.hpp                  # Consumer thread (to be completed)
├── reader.hpp                    # Reader thread (to be completed)
├── writer.hpp                    # Writer thread (to be completed)
├── transformer.cpp               # Transformer logic
├── *_test.cpp                    # Unit test files
├── scripts/                      # Testing and utility scripts
│   ├── verify                    # Verify output correctness
│   └── auto_gen_transformer      # Generate transformer code
└── tests/                        # Test cases and expected outputs
    ├── 00_spec.json             # Test case 0 specification
    ├── 00.in                    # Test case 0 input
    ├── 00.ans                   # Test case 0 expected output
    ├── 01_spec.json             # Test case 1 specification
    ├── 01.in                    # Test case 1 input
    └── 01.ans                   # Test case 1 expected output
```

## Working with Docker

### Understanding Docker Compose

This repository includes a `docker-compose.yml` file that defines a pre-configured build environment. This ensures everyone has the same compilation environment regardless of their host operating system.

### Using Docker Compose

#### Pull the Docker Image

First, pull the required Docker image:

```bash
docker pull gcc:latest
```

**Note**: The repository is configured to use `gcc:latest` by default. If you prefer, you can modify `docker-compose.yml` to use other similar images like `dasbd72/xv6:amd64` or `dasbd72/xv6:arm64v8`.

#### Build Your Code

The Makefile includes a convenient `docker-build` target:

```bash
make docker-build
```

This command:
1. Starts a Docker container with the correct build environment
2. Mounts your current directory into the container
3. Compiles your code using `make`
4. Exits the container (built binaries remain in your directory)

### Using Docker Compose Run (Alternative)

You can also use `docker-compose` for running commands:

```bash
# Build code
docker-compose run --rm build

# Run interactive shell
docker-compose run --rm build /bin/bash

# Run a specific command inside the container
docker-compose run --rm build ./main 200 tests/00.in output.txt
```

### Building Locally (Optional)

If you prefer to build directly on your machine without Docker, ensure you have:

- A C++ compiler supporting C++11 (g++ or clang++)
- pthread library (usually included with your compiler)
- Python 3 with the `click` library (`pip install click`)

Then simply run:

```bash
make
```

**Note**: Building locally may produce different results due to environment differences. Docker is recommended for consistency with the grading environment.

## Running Test Cases

We provide two public test cases (00 and 01) and a verification script for you to test your implementation.

To run a test case:

```bash
# First, generate the transformer for the specific test
python3 scripts/auto_gen_transformer.py --input tests/00_spec.json --output transformer.cpp

# Rebuild with the new transformer
make docker-build

# Run the main program with test parameters (check the spec file for the correct parameters)
./main 200 tests/00.in output.txt

# Verify the output
python3 scripts/verify.py --output output.txt --answer tests/00.ans
```

For test case 01:

```bash
python3 scripts/auto_gen_transformer.py --input tests/01_spec.json --output transformer.cpp
make docker-build
./main 4000 tests/01.in output.txt
python3 scripts/verify.py --output output.txt --answer tests/01.ans
```

## Submission

### Setting Up the Submission Repository

Your team has been assigned a submission repository named `os<year>_team<team-id>_pthread` (e.g., `os25_team01_pthread`).

1. Add the submission remote to your local repository:
   ```bash
   # Replace <team-id> with your actual team ID
   git remote add submit git@git.lsalab.cs.nthu.edu.tw:os25/os25_team<team-id>_pthread.git
   
   # Verify the remote was added
   git remote -v
   ```

2. Commit and push your changes:
   ```bash
   # Stage your changes
   git add .
   
   # Commit with a descriptive message
   git commit -m "Implement pthread assignment"
   
   # Push to the submission branch
   git push submit HEAD:os25-pthread
   ```

**Important Submission Notes**:
- Push to the branch `os25-pthread` (not `master` or other branches)
- You can push multiple commits before the deadline
- The final commit before the deadline will be graded
- Follow the general submission rules outlined in the xv6 guide

## Troubleshooting

### Common Issues

#### 1. "docker: command not found"

**Solution**: Docker is not installed or not in your PATH. Reinstall Docker Desktop and ensure it's running.

#### 2. "Cannot connect to the Docker daemon"

**Solution**:
- Windows/macOS: Start Docker Desktop application
- Linux: Start the Docker service: `sudo systemctl start docker`

#### 3. "/usr/bin/env: 'python3\r': No such file or directory"

**Solution**: This is a line ending issue on Windows. Run:
```bash
git config core.autocrlf false
git rm --cached -r .
git reset --hard
```

## Additional Resources

- Docker Documentation: [https://docs.docker.com/](https://docs.docker.com/)
- Docker Compose Documentation: [https://docs.docker.com/compose/](https://docs.docker.com/compose/)
- POSIX Threads Tutorial: [https://hpc-tutorials.llnl.gov/posix/](https://hpc-tutorials.llnl.gov/posix/)
- C++ Threading: [https://en.cppreference.com/w/cpp/thread](https://en.cppreference.com/w/cpp/thread)

Good luck with your assignment!
