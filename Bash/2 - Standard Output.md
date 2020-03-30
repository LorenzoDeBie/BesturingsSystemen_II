> Lorenzo De Bie 

# 2. Standard Output
## Table of Contents
[2.1 Writing Output to the Terminal/Window](#21-writing-output-to-the-terminalwindow)  
[2.2 Writing Output but Preserving Spacing](#22-writing-output-but-preserving-spacing)  
[2.3 Writing Output with More Formatting Control](#23-writing-output-with-more-formatting-control)  
[2.4 Writing Output Without the Newline](#24-writing-output-without-the-newline)  
[2.5 Saving Output from a Command](#25-saving-output-from-a-command)  
[2.6 Saving Output to Other Files](#26-saving-output-to-other-files)  
[2.7 Saving Output from the ls Command](#27-saving-output-from-the-ls-command)  
[2.8 Sending Output and Error Messages to Different Files](#28-sending-output-and-error-messages-to-different-files)  
[2.9 Sending Output and Error Messages to the Same File](#29-sending-output-and-error-messages-to-the-same-file)  
[2.10 Appending Rather than Clobbering Output](#210-appending-rather-than-clobbering-output)  
[2.11 Using Just the Beginning or end of a File](#211-using-just-the-beginning-or-end-of-a-file)  
[2.12 Skipping a Header in a File](#212-skipping-a-header-in-a-file)  
[2.13 Throwing Output Away](#213-throwing-output-away)  
[2.14 Saving or Grouping Output from Several Commands](#214-saving-or-grouping-output-from-several-commands)  
[2.15 Connecting Two Programs by Using Output as Input](#215-Connecting-two-programs-by-using-output-as-input)  
[2.16 Saving a Copy of Output Even While Using It as Input](#216-saving-a-copy-of-output-even-while-using-it-as-input)  
[2.17 Connecting Two Programs by Using Output as Arguments](#217-connecting-two-programs-by-using-output-as-arguments)  
[2.18 Using Mulitple Redirects on One Line](#218-using-mulitple-redirects-on-one-line)  
[2.19 Saving Output When Redirect Doesn't Seem to Work](#219-saving-output-when-redirect-doesnt-seem-to-work)  
[2.20 Swapping STDERR and STDOUT](#220-swapping-stderr-and-stdout)  
[2.21 Keeping Files Safe from Accidental Overwriting](#221-keeping-files-safe-from-accidental-overwriting)  
[2.22 Clobbering a File on Purpose](#222-clobbering-a-file-on-purpose)  


## 2.1 Writing Output to the Terminal/Window
* Use `echo`
  * Spacing will be ignored
  * Bash will do subsitutions, wildcard matching...
  
## 2.2 Writing Output but Preserving Spacing
* Enclose the string in quotes **""** or **''**

## 2.3 Writing Output with More Formatting Control
* `printf` : same syntax as in C

## 2.4 Writing Output Without the Newline
* `printf` : same syntax as in C, best solution
* `echo -n` : the option `-n` will disable the trailing endline
* `echo -e 'hi\c'` : the option `-e` enables escape sequences, the sequence `\c` disables the trailing endline

## 2.5 Saving Output from a Command
* Use the `>` symbol to redirect the output to a file  
   `echo fill it up > file.txt`

## 2.6 Saving Output to Other Files
* See [2.5 Saving Output from a Command](#25-saving-output-from-a-command)  

## 2.7 Saving Output from the ls Command
* `-C` option enables output in columns
* `-1` option makes the output only have one file per line
* A program can add code to detect wether the output is a terminal: `isatty`

## 2.8 Sending Output and Error Messages to Different Files
* `myprogram 1> messages.out 2> message.err`
  * Pass a file descriptor to redirect only the output from it to a specific file
  * 1 = STDOUT, when no descriptor is supplied, STDOUT is assumed
    * `myprogram > messages.out 2> message.err` : same as above
  * 2 = STDERR

## 2.9 Sending Output and Error Messages to the Same File
* use `&>` or `>&` which are shortcuts to send STDOUT and STDERR to the same place
  * `myprogram &> outfile`
  * `myprogram >& outfile`
* `myprogram > outfile 2>&1`
  * `2>&1` : redirect the output of 2 (STDERR) to the filedescriptor with number 1 (STDOUT), the `&` means the 1 will be interpretedas a filedescriptor instead of a file

## 2.10 Appending Rather than Clobbering Output
* use `>>` instead of `>` to redirect

## 2.11 Using Just the Beginning or end of a File
* `head` or `tail`
  * `-n `*`number`* : replace with number of lines you need
    * `tail` has the `+`*`number`* option: this will skip the first *number* lines
  * `tail` also has `-f` and `-F` which will follow a file and keep writing new additions

## 2.12 Skipping a Header in a File
* `tail -n +`*`number`* : see [2.11](#211-using-just-the-beginning-or-end-of-a-file)

## 2.13 Throwing Output Away
* redirect to `/dev/null`

## 2.14 Saving or Grouping Output from Several Commands
* group the commands in `{}`
  * `{ pwd; ls; cd ../elsewhere; pwd; ls; } > /tmp/all.out`
    * spaces before first command and after last command are important!
    * last command must end with `;`
* group the commands in `()`
  * this will execute the commands in a subshell and redirect all the output of this subshell to a given file
  * if you use cd in subshell the current shell will not change directory, only the subshell will.

## 2.15 Connecting Two Programs by Using Output as Input
* use the pipe `|` sympol to connect two programs (preferred)
  * Can also link multiple programs together
* redirect the output to a temp file & use this as input
  * `cat file otherfile > /tmp/cat.out; sort < /tmp/cat.out`

## 2.16 Saving a Copy of Output Even While Using It as Input
> Usefull when piping multiple commands together.  
> We want to save output of a command in the middle as well as piping it further  
> `cat myfile | tr 'a-z' 'A-Z' | uniq | awk -f transform.awk | wc`  
> We want to save the output of uniq and send it too awk.
* `tee` duplicates and sends input from STDIN to all given files and to STDOUT
  * `cat myfile | tr 'a-z' 'A-Z' | uniq | ` ***`tee /tmp/uniq.out |`***` awk -f transform.awk | wc`
  * `-a` : append to given files

## 2.17 Connecting Two Programs by Using Output as Arguments
> use output of a program as arguments when another program doesn't listen to STDIN, but expects arguments  
> `find . -name '*.c' | rm` will not work becaues rm doesn't listen to STDIN, it expects arguments
* `rm $(find . -name '*.c')` : command substition feature of bash
* everything in `$()` is run in a subshell. Changes to the environment made in this subshell are not made in the original shell
  * arguments are seperated with newlines inside `$()`: each output line is an argument

## 2.18 Using Mulitple Redirects on One Line
* `divert 3> file.three 4> file.four 5> file.five 6>else.where`
  * divert needs to use the FDs 3, 4, 5, and 6 for anything to be written to the file
  * not recommended as it makes things more complicated than need be
  * A C program doesn't need to call `open()` to use these FDs

## 2.19 Saving Output When Redirect Doesn't Seem to Work
> `>` is not working to redirect output
* You are a dummy because maybe the output you see on screen comes from STDERR
  * `2>` will redirect STDERR as well
  * `2>&1` will redirect STDERR to STDOUT
* STDOUT is *buffered* while STDERR is **not**
  * everything from STDERR
* Order of redirections is important
  * `somecmd > myfile 2>&1`: STDOUT is redirected to myfile, then STDERR is redirecter to STDOUT(=myfile)
  * `somecmd 2>&1 > myfile`: STDERR is redirected to STDOUT(=screen), then STDOUT = redirected to myfile, STDERR is still redirected to screen.
  * when using pipes: `somecmd 2>&1 | othercmd`: bash will now also pipe STDERR instead of showing it to the screen
    * `|&` is shorthand for `2>&1` since bash 4.0

## 2.20 Swapping STDERR and STDOUT
> We want to send STDOUT to a logfile and STDERR to the screen and to another logfile. Since pipes `|` only work with STDOUT this is not possible unless we swap STDERR and STDOUT
* `somecommand 3>&1 1>stdout.logfile 2>&3- | tee -a stderr.logfile`
  * `3>&1` copies (STDOUT) to FD 3
  * `1>stdout.logfile` redirects STDOUT to logfile
  * `2>&3-` copies STDERR to FD 3 which is set to STDOUT (original STDOUT, not the logfile)
    * the trailing `-` closes FD 3. `somecommand` then doesn't have an open FD it doesn't use
* `tee -a` : see [2.16](#216-saving-a-copy-of-output-even-while-using-it-as-input)

## 2.21 Keeping Files Safe from Accidental Overwriting
* `set noclobber -o` : bash won't overwrite any existing files when this option is set
  * only the shell won't overwrite existing files, other commands/programs are still able to overwrite existing files:
  `cp some.file other.file` will still overwrite other.file
* `set noclobber +o` : disable noclobber, files will be overwritten

## 2.22 Clobbering a File on Purpose
> clobber a file even when `noclobber` is set
* use `>|` to redirect output