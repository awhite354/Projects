# Split

Split is a program that takes a delimiter character and a list of filenames as arguments. The program "splits" each file into lines by replacing the delimiter character with a newline and prints the output to stdout.

## Usage

```bash
./split <delimiter> <file1> <file2> ...
```

## Design

The design for this program started by validating the input arguments. Then the program entered a while loop which reads each file given as an argument. Inside the while loop, the program checks if the argument is a "-", which sets the input file to stdin. If not, the program tries to open a file given the argument name, and throws an error if the file is not accessible, or not a file. I then go into a function called handleFile() which reads, processes, and writes the file. 

The handleFile() function starts by creating a buffer of size 4096 bytes. It then enters a while loop that reads from a file into the buffer. Then the buffer replaces each delimiter character with a newline. The buffer is then written to stdout using the write() function. This function loops until the end of the file is reached, writing to the buffer in each loop in 4096-byte increments.

### Error Handling

The main errors I needed to explicitly handle were the error for too few arguments and invalid delimiter arguments.   These errors needed to be handwritten with the return values set to EINVAL. All other errors were handled using the warn() function along with errno, which was automatically set within the program.