# -Version-Control-System-AKNUR-
Development of a Simple Version Control System in C++

## Overview
This project is a custom-built Version Control System (VCS) developed in C++, the name of the project is "AKNUR". It's designed to provide simple functionalities such as tracking file changes, committing changes, viewing commit logs, comparing different versions of files, and reverting them to previous states. AKNUR VCS is suitable for small projects and individual use.

## Features
- **Initialize Repository**: Set up a new repository for tracking file changes.
  
  Example:
./AKNUR init
- **Track Files**: Add files to the repository to keep track of changes.
  
  Example:
  ./AKNUR track <filename>
- **Commit Changes**: Save the current state of tracked files.
  
  Example:
  ./AKNUR commit
- **View Commit Logs**: Display a history of all commits made.
  
  Example:
  ./AKNUR log
- **Compare Commits**: Compare the current state of files with a previous commit.
  
  Example:
  ./AKNUR compare <hash>
- **Revert Changes**: Revert files back to a previous commit state.
  
  Example:
  ./AKNUR reverse <hash>
- **Integrity Check**: Verify the integrity of stored commits.
  
  Example:
  ./AKNUR check-integrity
- **Help message**: Display the help information.
  
  Example:
  ./AKNUR help

## Installation
Before installing the VCS, ensure you have the following requirements installed on your system:

-C++ Compiler (e.g., GCC, Clang)

-OpenSSL (for SHA-256 hashing)

-Git (if cloning the repository)

### Cloning the Repository
To get the source code, clone the repository from GitHub.

Example:
  
  git clone [https://github.com/NURnurNURnurNUR/-Version-Control-System-]
  
  cd [AKNUR]
  
### Compiling the Source Code
Compile the source code using a C++ compiler. For example, using Clang.

Example:

  clang++ -std=c++17 -o AKNUR -I/usr/local/opt/openssl@3.0/include -L/usr/local/opt/openssl@3.0/lib -lcrypto -lssl main.cpp

### Running the VCS
After compilation, you can run AKNUR.

Example:
  ./AKNUR [command]
  
Replace [command] with any of these like init, track, commit, etc.

## Usage
This section explains how to use the various features of the AKNUR VCS.

### Initializing a Repository

To start using AKNUR, you first need to initialize a repository. This sets up the necessary directory structure.

Example:
  ./AKNUR init

### Tracking Files
Add files to the  tracking list. Once a file is tracked, any changes to it will be monitored by the VCS.

Example:
  ./AKNUR track <filename>
  
Replace <filename> with the actual name of the file you want to track.

### Committing Changes
To save the current state of your tracked files, commit the changes. This will create a snapshot of the current state of these files.

Example:
  ./AKNUR commit
  
### Viewing Commit Log
You can view the history of all commits made in the repository, which can be useful for tracking changes over time.

Example:
  ./AKNUR log
  
### Comparing with a Commit
To compare the current state of files with those in a specific commit, use the compare command followed by the commit hash.

Example:
  ./AKNUR compare <hash>
  
Replace <hash> with the actual hash of the commit you want to compare against.

### Reverting to a Commit
If you need to revert your project back to the state of a specific commit, use the reverse command.

Example:
  ./AKNUR reverse <hash>
  
Again, replace <hash> with the hash of the commit you want to revert to.

### Checking File Integrity
Ensure the integrity of your files with the check-integrity command. This verifies that the contents of your committed files have not been altered.

Example:
  ./AKNUR check-integrity

### Getting Help
For more information about available commands and their usage.

Example:
  ./AKNUR help


