# Small Shell

- [Small Shell](#small-shell)
  - [Main Structure](#main-structure)
  - [Methods](#methods)
    - [input](#input)
      - [userin](#userin)
      - [gettok](#gettok)
    - [smallsh](#smallsh)
      - [procline](#procline)
      - [runcommand](#runcommand)

## Main Structure

The project has two main files:

- `smallsh.c` which contains the main shell code
- `input.c` which contains all the methods used for reading inputs

## Methods

### input

File containing all the methods that take care of the user input. Defines all the following variables:

- `inpbuf`: represents the user input and it's maxium lenght of 512 characters (`MAXBUF`).
- `tokbuf`: represents the buffer "tokenized", with a maximum lenght of 512*2 characters (`2 * MAXBUF`).

#### userin

Method used to print the prompt and read the user's input. Takes a pointer to string `*p` as parameter.

After creating `c`, to store the character from `getChar()`, and `count`, which tracks the amount of character within the buffer, the programs initializes the string pointers `ptr` and `tok` with their respective buffers.

The program now runs an infinite loop, where:

- The first if statements reads from input the next character. If this character is EOF, then return EOF.  
    Note that `c` is an integer because **it wants** to read EOF, which is an integer constant set to `-1`.
- The second if statements simply checks whether buffer count is lower than `MAXBUF`. If it isn't, adds to count 1 and inserts in the input buffer at index `count++` the currently read character.
- The third if statement checks whether the current character is a newline and if `count` is lower than `MAXBUF` again. It also adds in the input buffer at index `count` the newline character. Then, returns `count`.
- The fourth and last if statement checks if the current character is a newline. It then prints that the input is too big, resetting `count` to `0` and the reprinting the prompt `*p`.

>**So what's going on?**  
>The program is essentially reading an input from the user. Using the stdin buffer, the program loops until multiple conditions are met:
>
>- If the input is EOF, return EOF;
>- If, while iterating through the characters, the program finds `\n` before reaching the limit, set the buffer contents accordingly;
>- If the input is too long, simply ask for another input after telling that the input is too big.

#### gettok

Method which returns a token using `outptr` to read it.

After defining `type` which will hold the type of token, it passes to `*outptr` the pointer to `tokbuf` (note that `outptr` is indeed still empty) so that all the process starts .

With the first while loop, it skips any kind of space.
`*ptr` value, which points to a character of the input buffer gets copied to the token.

Through `*ptr`, the program returns the type of symbol, similarly to an ENUM. If it's not a `\n` or a semicolon, it sets the type to `ARG` and through `inarg` iterates through the special symbol array, checking whether there is a special character or not.

### smallsh

#### procline

Starts by defining:

- `*arg`: array which will contain the user inputs arguments, with a maximum of 512 arguments.
- `toktype`: type of the command's symbol.
- `narg`: number of arguments. Set to `0`.

The line processing starts with a switch where it grabs the token type through `gettok` and selects the respective case.

- If the argument is `ARG`, increments the `narg` counter
- If the argument is `EOL`, simply skip
- If the argument is `SEMICOLON`, after setting the background flag to 1, it sets in the argument array at `narg` to NULL, and runs the command. If the token type ends up being `EOL`, it sets the number of arguments to 0, resetting everything. It also checks whether the token is `BACKGROUND`, setting the flag back to 0.

#### runcommand

Method that indeed runs a command. Takes `**ccline`, which is the actual command, taken from input, and takes `background`, flag which determines whether the process is in the background or not.

The program sets up the whole fork process, creating a PID var to track the children process. This simply takes the command and executes it using the `execvp` command.

If the process is not running in the background, throws an error. Otheriwse, waits for the exitstatus of the process.
