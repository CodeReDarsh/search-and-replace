# Search and Replace

A program that takes command-line arguments that represent text-manipulation commands to search for text in an input file and generate a copy of the input file with the search-text replaced with the provided replacement-text. In other words, I am implementing a simple search/replace feature found in basically every text editor available.

The options (-l, -s, -r, -w) and other arguments given at the command line include:

- `-s <search_text>` : the text to search for in the input file. This option is required.
- `-r <replace_text>` : the text that will replace the search text in the output file. This option is required.
- `-w` : enable wildcard searching of words, explained later. This option is optional.
- `-l start_line,end_line` : which lines of the input file to process. The range is inclusive. Lines outside this range are simply copied from the input file to the output file unmodified. If end_line is greater than the number of lines in the file, simply read to the end of the file. Note that line numbers start at 1, not 0. This option is optional.
- `infile` : pathname of the input file. This argument is required.
- `outfile` : pathname of the output file. This argument is required.

For example, consider the command-line arguments 

`-s hello -r wonder -l 4,10 dir1/input1.txt dir2/output1.txt`

These arguments indicate that the program should search for occurrences of "hello" anywhere in the lines 4 through 10 (inclusive) in the file input1.txt (located in the directory named "dir1") and replace the occurrences of "hello" with "wonder" in the file output1.txt (which we must create in the directory "dir2"). All other text is simply copied from input1.txt to output1.txt. The search text may appear anywhere in the file and need not begin or end a word. With the exception of infile and outfile, which must appear in that order at the end of the command-line arguments, the command-line arguments may appear in any order.

>**Important**: for the sake of simplicity I have assumed that at least 7 command line arguments have been provided.
Make sure that the second last argument provides the `infile` and the last argument provides the `outfile`. Also make sure that `infile` and `outfile` refer to different file names.

For example, consider the command-line arguments 


`-s hello -r wonder -l 4,10 dir1/input1.txt dir2/output1.txt`


These arguments indicate that we should search for occurrences of "hello" anywhere in the lines 4 through 10 (inclusive) in the file `input1.txt` (located in the directory named `dir1`) and replace the occurrences of "hello" with "wonder" in the file `output1.txt` (which we must create in the directory `dir2`). All other text is simply copied from `input1.txt` to `output1.txt`.

The search text may appear anywhere in the file and need not begin or end a word. With the exception of `infile` and `outfile`, which must appear in that order at the end of the command-line arguments, *the command-line arguments may appear in any order.*

## Error checking

The program checks for the error situations given below, which have been provided in decreasing order of priority. That is, the first error case has the highest priority.

| Error Code (see hw6.h) | Explanation                                                                                                                                                                  |
|------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `MISSING_ARGUMENT`       | Fewer than 7 command-line arguments (including the executable’s name) have been provided.                                                                                    |
| `DUPLICATE_ARGUMENT`     | A recognized option (`-s, -r, -l, -w`) has been provided more than once.                                                                                                       |
| `INPUT_FILE_MISSING`     | The named input file is missing or cannot be opened for reading for some reason.                                                                                             |
| `OUTPUT_FILE_UNWRITABLE` | Cannot create the output file for writing for some reason.                                                                                                                |
| `S_ARGUMENT_MISSING`     | The `-s` option is missing, or the argument to `-s` (i.e., the search-text) is missing.                                                                                          |
| `R_ARGUMENT_MISSING`     | The `-r` option is missing, or the argument to `-r` (i.e., the replacement-text) is missing.                                                                                     |
| `L_ARGUMENT_INVALID`     | One or both of the line numbers given to `-l` is missing or unparseable, or `start_line` > `end_line`. |
| `WILDCARD_INVALID`       | The argument to `-s` is not a properly-formed wildcard search-term, but wildcard matching has been requested. Refer to wildcard search patterns for an explanation  |

> **Note**: For the `-s`, `-r` and `-l` options, the argument that immediately follows the `-s/-r/-l` option on the command line must not begin with a "`-`" character. If it does, we will assume that the `-s/-r/-l` option is missing its search/replacement string and we should return the appropriate error code. Any extra unknown arguments and options don't hinder with the text text replacement.

>**Also Note:** I have assumed that `search_text` and `replacement_text` are at most `MAX_SEARCH_LEN` characters in length as defined in the header file `hw6.h`. You may change this length at your convenience.

> **CAUTION** : In Windows, newlines in files are represented by the [two-character combination](https://dev.to/pieter/why-windows-uses-rn-newlines-instead-of-n-126l#:~:text=A%20newline%20on%20Windows%20is,Linux%20simply%20use%20a%20%5Cn%20.) `\r\n`. Under Linux and MacOS, newlines are simply stored as \n. During testing, which will be under Linux, newlines in files will be stored using the Linux/MacOS style. You have been warned.

## Wildcard Search and Replacement

Unlike a simple replacement, wildcard replacement replaces "words" with new text, where a "word" is defined as a sequence of alphanumeric characters terminating just before, and/or beginning just after, a whitespace character, a punctuation mark, or the end of the file.

Two kinds of wildcards are supported searching:

1. Searching for a prefix of a string 
2. Searching for a suffix of a string 

These possibilities are indicated by a `*` that _ends_ the search string (when searching for a prefix) or _begins_ the search string (when searching for a suffix).


For example, if `search_text` = "`foo*`", this indicates we are searching for words that begin with "foo". Conversely, `search_text` = "`*foo`", this indicates we are searching for wrods that end with "`foo`". In the list of examples given at the end, if our command line arguments included `-s "St*" -w -r "Wolfie"`, then all the instances of "`Stony`" shown would be replaced with "`Wolfie`".


Here’s another example: for the arguments `-s aga* -r lamp -w`, the string "`Off again! On again! In again! Out again!`" would become "`Off lamp! On lamp! In lamp! Out lamp!`"


For the arguments `-s "*n" -r "SBU" -w`, the string "`Off again! On again! In again! Out again!`" would become "`Off SBU! SBU SBU! SBU SBU! Out SBU!`"

In the examples below, assume that `input.txt` exists and is readable, and that `output.txt` is writable.

|  # | Arguments                                                      | Outcome / Error Code                                                                                                                                                                                                    |
|:--:|----------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|  1 | `-s the -r end -l 10,15 input.txt output.`txt                    | Successful run                                                                                                                                                                                                          |
|  2 | `-s the -r end -h -Z moo -l 10,15 -q hello input.txt output.txt` | Successful run. We ignore unrecognized options and arguments for those options. Ignored options are highlighted.                                                                                                        |
|  3 | `-l 10,15 -s the -r end input.txt output.txt`                    | Successful run. The order of valid arguments is irrelevant.                                                                                                                                                             |
|  4 | `-l 10,15 -r the -s input.txt output.txt`                        | Successful run. Based on our assumptions about the location of the input and output filenames in argv[], these arguments are actually valid. The program will search for the text "input.txt" in the file of that name. |
|  5 | `-r end -s *the* -l xyz10,15 -w input.txt`                       | INPUT_FILE_MISSING. Other errors in the arguments have lower priority. (Your code should treat "-w" as the input file name and then fail to open it.)                                                                   |
|  6 | `-s the -r end input.txt /output.txt`                            | OUTPUT_FILE_UNWRITABLE. We cannot write to the root directory.                                                                                                                                                          |
|  7 | `-s the -r end input.txt readonly.txt `                          | OUTPUT_FILE_UNWRITABLE. Assume readonly.txt is a file that has been made read-only via a command to the OS. (e.g., chmod)                                                                                               |
|  8 | `(none)`                                                         | MISSING_ARGUMENT                                                                                                                                                                                                        |
|  9 | `-s the -r end /trash.txt`                                       | MISSING_ARGUMENT                                                                                                                                                                                                        |
| 10 | `-s stony input.txt output.txt`                                  | MISSING_ARGUMENT                                                                                                                                                                                                        |
| 11 | `input.txt output.txt`                                           | MISSING_ARGUMENT                                                                                                                                                                                                        |
| 12 | `-s -r end -l 10,15 -w input.txt output.txt`                     | S_ARGUMENT_MISSING                                                                                                                                                                                                      |
| 13 | `-s the -r -l 10,15 input.txt output.txt`                        | R_ARGUMENT_MISSING                                                                                                                                                                                                      |
| 14 | `-s the -r end -l input.txt output.txt`                          | L_ARGUMENT_INVALID                                                                                                                                                                                                      |
| 15 | `-s the -r -l input.txt output.txt`                              | R_ARGUMENT_MISSING                                                                                                                                                                                                      |
| 16 | `-s the -r end -l 10sbu,15 input.txt output.txt`                 | Successful run                                                                                                                                                                                                          |
| 17 | `-s the -r end -l 10,sbu15 input.txt output.txt`                 | L_ARGUMENT_INVALID (sbu15 is parsed as 0)                                                                                                                                                                               |
| 18 | `-s the -r end -l 10 input.txt output.txt`                       | L_ARGUMENT_INVALID                                                                                                                                                                                                      |
| 19 | `-s the* -w -r end -l 10,15 input.txt output.txt`                | Successful run                                                                                                                                                                                                          |
| 20 | `-s the* -w -r end -l 10,15bonk input.txt output.txt`            | Successful run                                                                                                                                                                                                          |
| 21 | `-w -r end -s *the -l 10,15bonk input.txt output.txt`            | Successful run                                                                                                                                                                                                          |
| 22 | `-s the -w -r end -l 10,15 input.txt output.txt`                 | WILDCARD_INVALID. Missing *.                                                                                                                                                                                            |
| 23 | `-s *the* -w -r end -l 10,15 input.txt output.txt`               | WILDCARD_INVALID. Too many *s.                                                                                                                                                                                          |
| 24 | `-s the -w -r end -r end -s hello -l 10,15 input.txt output.txt` | DUPLICATED_ARGUMENTS. Duplicated arguments have higher priority than an invalid wildcard argument.                                                                                                                      |
| 25 | `-s the -r end -l 10,15 -s hello input.txt output.txt`           | DUPLICATED_ARGUMENTS. -s appears more than once.                                                                                                                                                                        |
| 26 | `-s the -r end -l 10,15 -r what input.txt output.txt`            | DUPLICATED_ARGUMENTS. -r appears more than once.                                                                                                                                                                        |
| 27 | `-w -r end -s *the -l 10,15 -w input.txt output.txt`             | DUPLICATED_ARGUMENTS. -w appears more than once.                                                                                                                                                                        |
| 28 | `-s the -r end* -l 10,15 input.txt output.txt`                   | Successful run                                                                                                                                                                                                          |


# Notes

Make sure you have [Valgrind](https://valgrind.org/) and [Criterion](https://criterion.readthedocs.io/en/master/setup.html) with it's required dependencies installed in order to run the unit tests.

- To install valgrind enter the following command:

    `sudo apt-get install -y valgrind`

- To install Criterion and it's dependencies enter the following command:
    
    `sudo apt-get install libcriterion-dev && sudo apt-get install -y ninja-build meson cmake pkg-config libffi-dev libgit2-dev`

- To run the unit tests enter the following:

    `make && make test`
