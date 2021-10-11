# VE482 Operating System Project1

# Linux shell written in C

mumsh.c includes the main() function.

global.h declares all necessary struct and global variables.

readInput.c reads the input from stdin.

parse.c parse the input into command argv.

execute.c runs the command.

pipe.c is a special version of execute.c handling commands with pipe.

redirect.c handles input and output redirection.

built-in.c includes built in commands like cd and pwd.

Some extra functions are merged into these files.