# MultiThreadedCPrograms

A collection of multithreaded c programs. Each folder has its own Makefile and semi-automatic check program included.
Create PRs if you find bugs.

N.B: there are warnings about casts from pointer to integer. This doesn't break the execution but it will be fixed in a future update.

## MTInvert

Multi threded C program which inverts the content of a file and saves it into a new file.

To quickly generate the test file and run the program, use this command in your shell:

```bash
    make && ./gen_data && ./invert dataset.bin inverted.bin 4 && ./check
```
