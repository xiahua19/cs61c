## Compile C code
`gcc -o exe_name file_name.c`

## Debug program using `GDB`
> sudo apt-get install gdb
- `gcc -g -o exe_name file_name.c`: compile a c program
- `gdb exe_name`: debug the program compiled before
- `break line_num`: create a breakpoint on a line
- `run`
- `run < input.txt`: redirection of io
> The author of the post is following a buffer overflow exploitation course. Instead of helping him, everyone jumped with off topic information. We all know that the code is wrong, but then, how you are supposed to learn buffer overflow exploitation if not on a bad code ? In this case debugging didn't work properly because location of the debug file is somewhere else.
- `(gdb) show debug-file-directory`: The directory where separate debug symbols are searched for is "/usr/lib/debug"
- `(gdb) set debug-file-directory`
- `s`: run next line
- `print var`: print variable
- `display var`: display variable after every run
- `kill`

## Check Memory using `valgrind`
> sudo apt-get install valgrind
- `gcc -o exe_name file_name.c`: compile the c program
- `valgrind exe_name`: check the memory used by the program