> Lorenzo De Bie 

# 1. Beginning Bash
## Table of Contents
[1.5 Finding and Running Commands](#15-finding-and-running-commands)  
[1.6 Getting Information About Files](#16-getting-information-about-files)  
[1.7 Showing All Hidden (Dot) Files in the Current Directory](#17-showing-all-hidden-dot-files-in-the-current-directory)  
[1.8 Using Shell Quoting](#18-using-shell-quoting)  
[1.9 Using or Replacing Builtins and External Commands](#19-using-or-replacing-builtins-and-external-commands)  
[1.10 Determining if You Are Running Interactively](#110-determining-if-you-are-running-interactively)  


## 1.5 Finding and Running Commands
>You need to find and run a particular command.  
1. You know the name of the command:
   * `type` : gives the location of a command
     * `-a` : prints all mathces
   * `which` : same as type, but only searches PATH
     * `-a` : prints all matches
2. You don't know or can't remember the name of a command
   * `apropos` : searches manpages names and descirptions for regex arguments
   * `man ⁻k` : does the same as apropos
3. You need to find a **file** or command:
   * `locate` : uses databse files about the system
   * `slocate` : also stores permissions and does not show files or command you do not have permissions for
4. `find` : see chapter 9 [will change this when we get there](https://www.youtube.com/watch?v=KpXsfimrkFo)
5. `ls` : list all files in this direcotry

## 1.6 Getting Information About Files
>You need more information about a file.
1. `ls`
   * `-l` : gives extra information about the files
   * `-a` : don't hide files & dirs starting with . (dot).
   * `-A` : don't hide files & dirs starting with . (dot) except . and ..
   * `-R` : recurse through subdirectories
2. `stat`
   * has most of the same options as ls, see man page for more info
3. `file`
   * has most of the same options as ls, see man page for more info

## 1.7 Showing All Hidden (Dot) Files in the Current Directory
>See only the hidden files in current directory.
* `ls -d` : the `-d` options does not show contents of the directories listed
  * `.[!.]*` displays all hidden files and dirs without `.` and `..` dirs
  * `.[!.]*` gets expanded by the shell, not `ls`.

## 1.8 Using Shell Quoting
>When will bash expand text?
* when text is not inclosed in single quotes (**''**), it will be expanded
* escaping characters only works with strings enclosed in double quotes (**""**) or not enclosed.

## 1.9 Using or Replacing Builtins and External Commands
>You need to know exactly what functions is executed `/bin/echo` or builtin `echo`.
>You have created a new command and it conflicts with an existing or builtin.
* use `type` or `which` to find which command this shell will use
* use `enable` to enable or disable `-n` buitlin commands of the shell

## 1.10 Determining if You Are Running Interactively
>How do I know if my script is running interactively
* the variable `$-` contains all the shell variables, this will contain `i` when running interactively
* ```Bash
    case "$-" in
      *i*) #Code for interactive shell here
      ;;
      *) #Code for noninteractive shell here
      ;;
    esac
  ```