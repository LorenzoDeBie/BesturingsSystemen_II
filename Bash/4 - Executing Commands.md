> Lorenzo De Bie
# 4. Executing Commands
## Table of Contents
[4.1 Running Any Executable](#41-running-any-executable)  
[4.2 Running Several Commands in Sequence](#42-running-several-commands-in-sequence)  
[4.3 Running Several Commands All at Once](#43-running-several-commands-all-at-once)  
[4.4 Telling Whether a Command Succeeded or Not](#44-telling-whether-a-command-succeeded-or-not)  
[4.5 Running a Command Only if Another Command Succeeded](#45-running-a-command-only-if-another-command-succeeded)  
[4.6 Using Fewer if Statements](#46-using-fewer-if-statements)  
[4.7 Running Long Jobs Unattended](#47-running-long-jobs-unattended)  
[4.8 Displaying Error Messages When Failures Occur](#48-displaying-error-messages-when-failures-occur)  
[4.9 Running Commands from a Variable](#49-running-commands-from-a-variable)  
[4.10 Running All Scripts in a Directory](#410-running-all-scripts-in-a-directory)  

## 4.1 Running Any Executable
* type in the thame of the command and hit enter
* `bash` looks in `$PATH` to find the commands you want to run
  * Security risk: adding `.` to `$PATH` (especially at the beginning of `$PATH`)
    * Malicious version of `ls` would be run instead of your intended version (when `.` is at the beginning of `$PATH`)

## 4.2 Running Several Commands in Sequence
* Type other commands while first one is running, UNIX/Linux will do it
* put the commands in a script and run it with `bash`
  * `bash ./script/sh`
* put a `;` between the commands, all commands will execute regardless if they fail or not
* put a `&&` between the commands to only execute them if the previous one succeeded (return code = 0)

## 4.3 Running Several Commands All at Once
* put a `&` after each command
  * `long & medium & short`
  * `bash` will show the job numbers of `long` and `medium`
  * send a running program to the background by pressing `ctrl+Z` and then entering `bg`, this is basicly adding a `&` after the fact.

## 4.4 Telling Whether a Command Succeeded or Not
* `$?` contains the exit status of the last run command
  * 0 = success, nonzero = fail (**convention**)
  * try only using exit codes 0 to 127: `bash` uses 128 + N to denote killed by signal N
  * exit statusses range from 0 to 255 (return bigger or smaller, and it wraps aroudn)

## 4.5 Running a Command Only if Another Command Succeeded
* use the exit status `$?` in an if statement
* use the `&&` operator: [see 4.2](#42-running-several-commands-in-sequence) 

## 4.6 Using Fewer if Statements
* use the `&&` operator: [see 4.2](#42-running-several-commands-in-sequence) 
* `set -e` : `bash` will now exit anytime it encounters a nonzero return code. Particulary good for scripts.
  * shell window will dissapear when using this in interactive shell

## 4.7 Running Long Jobs Unattended
> running a long job in the background wiht `&` will stop when you exit the shell
* `nohup `*`long`*` &`: makes *`long`* ignore `SIGHUP` signals
  * `SIGHUP` signals are used to hangup child processes. When the shell exits it sends this to all child processes
  * `nohup` will redirect `STDOUT` & `STDERR` to a file `nohup.out` in the current dir (or home dir if creating it in this dir fails).
    * Will not happen if you manually redirect output

## 4.8 Displaying Error Messages When Failures Occur
* use the `||` operator: `bash` will evaluate the second expression only if the first one failed
* don't forget to use braces to group expressions
  * `cmd || echo error ; exit 1` will always execute exit 1!

## 4.9 Running Commands from a Variable
* `bash` lets you assign program names to variables. It will execute the program if the variable gets used in a place where a program name is expected
  * Watch out what variable names you use: some programs use environment variables when executing

## 4.10 Running All Scripts in a Directory
```bash
for SCRIPT in /path/to/scripts/dir/*
do
    if [ -f "$SCRIPT" -a -x "$SCRIPT" ]
    then
        $SCRIPT
    fi
done
```
* loops over everything in the specified directory, test if it is a file and if it is executable. If both are true, bash executes the file.
* No mechanism for parameters for each script