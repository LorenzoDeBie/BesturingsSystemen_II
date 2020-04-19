> Lorenzo De Bie
# 6 Shell Logic and Arithmetic
## Table of Contents


## 6.1 Doing Arithmetic in Your Shell Script
* *`RESULTVAR`*`=$((`*`calculations`*`))`
* `let `*`RESULTVAR`*`='`*`calculations`*`'`
  * Additional assingment operators: they do the same as C operators
    * `=`
    * `*=`
    * `/=`
    * `%=`
    * `+=`
    * `-=`
    * `<<=`
    * `>>=`
    * `&=`
    * `^=`
    * `\` (this means bitwise or: `a|=b` in )
  * these also work inside the `$(())` notation
* no spaces around the `=` character!
* no `$` in front of variables in these expressions except when you want to use the parameter variables like `$1`, `$2` ...
* Multiple assingments using the `,` operator:
  ```bash
  echo $(( X+=5 , Y*=3 )) #returns the value of Y
  ```
  * both assignments get done, last one gets returned`
  * same can be done using `let`, no `,` operator needed
    ```bash
    let X+=5 Y*=3
    ```
    * all operations have to be one word, no whitespace allowed, except when using quotes

## 6.2 Branching on Conditions
* `if, elif, else` syntax:
  ```bash
  # this example checks number parameters passed to script
  if (( $# < 3 ))
  then
    printf "%b" "Error. Not enough argumetns.\n"
    printf "%b" "Usage: myscript file1 op file2\n"
    exit 1
  elif (( $# > 3 ))
  then
    printf "%b" "Error. Too many argumetns.\n"
    printf "%b" "Usage: myscript file1 op file2\n"
    exit 2
  else
    printf "%b" "Argument count correct. Proceeding... \n"
  fi
  ```  
  or
  ```bash
  if [ $# -lt 3]
  then
    printf "%b" "Error. Not enough argumetns.\n"
    printf "%b" "Usage: myscript file1 op file2\n"
    exit 1
  elif [ $# -gt 3 ]
  then
    printf "%b" "Error. Too many argumetns.\n"
    printf "%b" "Usage: myscript file1 op file2\n"
    exit 2
  else
    printf "%b" "Argument count correct. Proceeding... \n"
  fi    
  ```
* `(())` is used for arithmetic evaluations
* `[]` invokes `test` builtin which checks the last return value `$?` for `0`
* multpiple commands can be listed inside an if  statement:  
  ```bash
  if ls; pwd; cd $1;
  then
    echo success
  else
    echo failed
  fi
  ```  
  `ls`, `pwd` and `cd $1` will be executed and the if looks at the return value of `cd $1` because that will be inside `$?`

## 6.3 Testing for File Characteristics
* Unary operators that check file characteristics  
  Option | Description
  --- | ---
  `-b` | Block kspecial device (*/dev/hda1* for example)
  `-c` | Character special (*/dev/tty* for example)
  `-d` | Directory
  `-e` | File exists
  `-f` | Regular file
  `-g` | *setgid* bit is set
  `-h` | Symbolic link (same as `-L`)
  `-G` | Owned by effective group ID
  `-k` | Sticky bit is set
  `-L` | Symbolic links (same as `-h`)
  `-N` | Modified since last read
  `-O` | Owned by the effective user ID
  `-p` | Named pipe
  `-r` | Readable
  `-s` | Size > 0
  `-S` | Socket
  `-u` | *setuid* bit is set
  `-w` | Writeable
  `-x` | Executable
  `-z` | String length is zero
  `-n` | String length is nonzero
For example:
```bash
if [ -e myfile ]
```
  ## 6.4 Testing for More than One Thing
  * operators for logical AND `-a` and OR `-o`
  * use parentheses for propper precedence (parentheses may have to be escaped)
```bash
if [ -r $FILE -a -w $FILE ]
# with parentheses
if [ -r "$FN" -a \( -f "$FN" -o -p "$FN" \)]
```

## 6.5 Testing for String Characteristics
> Test if string inut is empty
* Unary opterators `-z` or `-n` : [See 6.3](#63-testing-for-file-characteristics)
```bash
if [ -n "$VAR" ]
then
    echo has text
else
    echo zero length
fi

if [ -z "$VAR" ]
then
    echo zero length
else
    echo has text
fi
```

## 6.6 Testing for Equality
> `-eq` vs `==`
* `-eq` : numeric comparison
* `==` : string comparison
```bash
VAR1="05"
VAR2="5"

if [ "$VAR1" -eq "$VAR2" ]
then
  echo YES
else
  echo NO
fi
# YES

if [ "$VAR1" = "$VAR2" ]
#if [ "$VAR1" == "$VAR2" ] is also possible
then
  echo YES
else
  echo NO
fi
# NO
```
* other comparison operators:  
  Numeric | String
  --- | ---
  `-lt` | `<`
  `-le` | `<=`
  `-gt` | `>`
  `-ge` | `>=`
  `-eq` | `=` or `==`
  `-ne` | `!=`

## 6.7 Testing with Pattern Matches
> shell-style pattern matching in tests?
* use the double-bracket compound statement `[[ `*`test`*` ]]` :
  ```bash
  if [[ "${MYFILENAME}" == pattern ]]
  ```
* turn on `extglob` for unlock more powerful pattern-matching
  ```bash
  shopt -s extglob
  # check for any files ending in .jpg or .jpeg
  if [[ "$FN" == *.@(jpg|jpeg) ]]
  ```
    * Possible grouping symbols:
      Grouping | Meaning
      --- | ---
      `@(...)` | Only one occurence
      `*(...)` | Zero or more occurences
      `+(...)` | One or more occurences
      `?(...)` | Zero or one occurence
      `!(...)` | Not this, but anything else
    * `shopt -s nocasematch` disables case sensitivity

## 6.8 Testing with Regular Expressions
> you just need regex (no you don't, pls don't)
* `=~` operators uses regex matching, result is stored inside `$BASH_REMATCH`:
  ```bash
  if [[ "$FN" =~ pattern ]]

  # special character classes
  [:alpha:] # same as [0-9a-zA-Z]
  [:digit:] # same as [0-9]
  [:blank:] # spaces or tabs`
  ```
    * grouping is done by placing pattern inside `()`
    * repetitions are done by placing pattern inside `[]` followed by modifier  