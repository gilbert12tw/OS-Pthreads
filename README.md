# Pthread Assignment Guide

This guide will help you set up your development environment for the pthread assignment using Docker. Since you've already completed the xv6 assignment, this guide focuses primarily on Docker-specific setup and workflows for this assignment.

## Pthread Overview

This assignment involves implementing multi-threaded programs using POSIX threads (pthreads) in C++. Unlike xv6, which required QEMU, this assignment runs natively in a containerized Linux environment using Docker.

## Pthread Directory Structure

```
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

## Setting up your environment

1. Clone the shared pthread repository:

```bash
git clone https://git.lsalab.cs.nthu.edu.tw/os25/os25_shared_pthread.git pthread
```

2. Navigate to the directory:

```bash
cd pthread
```

3. (Important! For Windows users) Configure Git to handle line endings correctly:

```bash
git config core.autocrlf false
git rm --cached -r .
git reset --hard
```

If you don't do this, you will probably encounter the error `/usr/bin/env: 'python3\r': No such file or directory` when running a script inside the Docker container.

4. Now you can start working on the assignment. The template will provide you with the necessary files and structure to begin

- Spec is in file `Pthread-spec.md`
- Grading script is in `grade-mp1-public`(TBD)

## How to run pthread

### Run with Docker (Recommended)

This repository includes a `docker-compose.yml` file that defines a pre-configured build environment. This ensures everyone has the same compilation environment regardless of their host operating system.

1. Clone the repository and navigate to the directory:

```bash
cd <your-cloned-repo>
```

2. Pull the built image:

```bash
docker pull gcc:latest
```

**Note**: The repository is configured to use `gcc:latest` by default. If you prefer, you can modify `docker-compose.yml` to use other similar images like `dasbd72/xv6:amd64` or `dasbd72/xv6:arm64v8`.

3. Using `docker-compose`:

```bash
# Build code
docker-compose run --rm build

# Run interactive shell
docker-compose run --rm build /bin/bash

# Run a specific command inside the container
docker-compose run --rm build ./main 200 tests/00.in output.txt

# Verify output
docker-compose run --rm build python3 scripts/verify.py --output output.txt --answer tests/00.ans
```

### Building Locally (Optional)

If you prefer to build directly on your machine without Docker, ensure you have:

- A g++ compiler supporting C++11
- pthread library

Then simply run:

```bash
make
```

**Note**: Building locally may produce different results due to environment differences. Docker is recommended for consistency with the grading environment.

## How to run the grading scripts

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

For test case 01(docker-compose):

// TODO add script here

// and docker-compose command for test case

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

## Plagiarism Policy

- Never show your code to others.
- If your code is found to be similar to others, including sources from the internet, and you cannot answer questions properly about your code during the demo, you will be considered as plagiarizing and will receive a score of 0 for the assignment.
