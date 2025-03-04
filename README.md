# minishell

This project is a simple shell implementation written in C.

## Description

Welcome to my Minishell project! As someone passionate about understanding the intricacies of Unix-like systems, I embarked on this journey to create a simple yet functional shell from scratch. This project, written in C, adheres to the POSIX standard and aims to provide a hands-on experience with command parsing, execution, and environment variable management. By developing this shell, I hope to deepen my knowledge and share my learnings with others who are also interested in the fascinating world of operating systems.

## Features

- Execution of basic shell commands.
- Handling of built-in commands like `cd`, `echo`, etc.
- Support for multiple pipe commands.
- Environment variable handling.
- Error handling and reporting.

## Installation

To install and run minishell, follow these steps:

1. Clone the repository:
   ```sh
   git clone https://github.com/hatchiman21/minishell.git

2. Navigate to the project directory:
   ```sh
   cd minishell

3. Compile the source code:
    ```sh
    make

## Usage

After compiling the project, you can start the shell by running:

    ./minishell

## Examples

Here are a few examples of how to use the minishell:

Running a basic command:

    ls -l

Using built-in commands:

    cd /path/to/directory
    echo "Hello, world!"

Chaining commands with pipes:

    ```sh
    ls -l | grep minishell
