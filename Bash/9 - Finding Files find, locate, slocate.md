 Lorenzo De Bie
# 9 Finding Files: find, locate, slocate
## Table of Contents
[9.1 Finding All Your MP3 Files](#91-finding-all-your-mp3-files)  
[9.2 Handling Filename Containing Odd Characters](#92-handling-filename-containing-odd-characters)  
[9.3 Speeding Up Operations on Found Files](#93-speeding-up-operations-on-found-files)  
[9.4 Finding Files Across Symbolic Links](#94-finding-files-across-symbolic-links)  
[9.5 Finding Files Irrespective of Case](#95-finding-files-irrespective-of-case)  
[9.6 Finding Files by Date](#96-finding-files-by-date)  
[9.7 Finding Files by Type](#97-finding-files-by-type)  
[9.8 Finding Files by Size](#98-finding-files-by-size)  
[9.9 Finding Files by Content](#99-finding-files-by-content)  
[9.10 Finding Existing Files and Content Fast](#910-finding-existing-files-and-content-fast) 
[9.11 Finding a File Using a List of Possible Locations](#911-finding-a-file-using-a-list-of-possible-locations) 

## 9.1 Finding All Your MP3 Files
* `find `*`path`*` -name '`*`pattern`*`'`
  * Options are executed sequentially, they are like filters. `find` climbs through the filesystem and passes each file to the options: when the option returns true it is passed to the next one.
  * `-print` will print the name to `STDOUT`
  * `-exec` : executes the command given, command needs to end with `\;`. `{}` gets replaced with the filename
    * `find . -name '*.mp3' -print -exec mv '{}' ~/songs` : moves all mp3 files in this or sub directories to `~/songs`

## 9.2 Handling Filename Containing Odd Characters
> filenames containing whitespaces or other 'odd' characters won't always play nice with the `-exec` part of find
* `-print0` : uses a `\0` character as seperator between found files
* `xargs` : applies a given command to all given parameters
  * `-0` : use a `\0` character as seperator for parameters
  * `-i` :  eliminates argument grouping: command will be executed for each parameter, not one command for all of them (**not all versions port this option**)

## 9.3 Speeding Up Operations on Found Files
* `xargs` : [see 9.2](#92-handling-filename-containing-odd-characters)

## 9.4 Finding Files Across Symbolic Links
* `find -L` or `find `*`path`*` -follow` : `find` doesn't follow symbolic links by default, `-L` or `-follow` enables this

## 9.5 Finding Files Irrespective of Case
* `-iname` option with `find` : **not all versions of find support `-iname`**
* use a sick regex when `-iname` is not available (pls don't)

## 9.6 Finding Files by Date
* `-mtime [+-]`*`days`* option with `find` : 
  * `+` : look for *`days`* ago or older
  * `-` : look for *`days`* ago or newer
* combine multiple options with `-a` and `-o`
  * logical `and` and `or` respectively
  * use `\(` and `\)` or `'('` and `')'` to group multiple things and set precedence

## 9.7 Finding Files by Type
* `find `*`path`*` -type `*`[bcdpflsD]`*
  * `b` : Block special file
  * `c` : Character special file
  * `d` : Directory
  * `p` : pipe
  * `f` : plain ol' file
  * `l` : Symbolic link
  * `s` : Socket
  * `D` : Door (Solaris only)

## 9.8 Finding Files by Size
* `-size [+-]`*`number[unit]`* :  option with `find`
  * works same way as `-mtime`, [see 9.6](#96-finding-files-by-date)
* Possible *`units`*:
  * `k` : kilobytes
  * `c` : bytes (or characters, same thing)
  * `b` : blocks (=512 bytes) **= default**
* `-delete` is better than `-exec rm '{}' \;` when `find` supports it 

## 9.9 Finding Files by Content
* `find` with `-exec grep -Hi `*`pattern`*` {} \;`
* `find` with `-exec grep -i `*`pattern`*` {}  /dev/null \;` : use this when `grep` doen't support the `-H` option

## 9.10 Finding Existing Files and Content Fast
> [gotta go fast](https://www.youtube.com/watch?v=VTHsOSGJHN0)
* `locate` or `slocate`: [see CH1.5](1%20-%20Beginning%20Bash.md#15-finding-and-running-commands)

## 9.11 Finding a File Using a List of Possible Locations
```bash
for file in list; do
  [ -f "$file" ] && YOUR_COMMANDS_HERE
done
```
* search in `PATH` : use `${VAR/PATTERN/REPLACEMENT}` substitution operator
```bash
for path in ${PATH//:/ }; do
  YOUR COMMANDS_HERE
done
```