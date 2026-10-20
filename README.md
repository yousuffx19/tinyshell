# tinyshell

- Basic shell built in C for UNIX-like shell commands

## Features

- Execute variable path commands
- Piping
- Output Redirection
- In-built cd command
- Arrow key navigation
- History navigation

## How to run
- clone the repository to your system
- run ``make``
- run the generated executable ``./test``

## Limitations
- Minor syntax problems (such as not parsing properly when we didnt give space in redirections)
- Unexpected behaviour with more complicated inputs
- Limited command size and history space due to fixed buffer sizes
- Signals like Ctrl-C will cause the terminal itself to exit as signals are still enabled
