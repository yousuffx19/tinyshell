# tinyshell

- Basic shell built in C for UNIX-like shell commands

## Features

- Execute variable path commands
- Piping
- Output Redirection
- In-built cd command

## How to run
- clone the repository to your system
- run ``make``
- run the generated executable ``./test``

## Limitations
- Does not run in raw terminal mode. So no arrow keys usage, Ctrl-C Ctrl-D interrupts
- No built-in history command, no reverse-i-search and other functionalities.
- Minor syntax problems (such as not parsing properly when we didnt give space in redirections)

