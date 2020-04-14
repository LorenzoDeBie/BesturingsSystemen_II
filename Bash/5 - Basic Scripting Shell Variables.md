>Lorenzo De Bie
# 5 Basic Scripting: Shell Variables
## Table of Contents
[5.1 Documenting Your Script](#51-documenting-your-script)  
[5.4 Seperating Variable Names from Surrounding Text](#54-seperating-variable-names-from-surrounding-text)  
[5.5 Exporting Variables](#55-exporting-variables)  
[5.6 Seeing All Variable Values](#56-seeing-all-variable-values)  
[5.7 Using Parameters in a Shell Script](#57-using-parameters-in-a-shell-script)  
[5.8 Looping Over Argument Pased to a Script](#58-looping-over-argument-pased-to-a-script)  
[5.9 Handling Parameters with Spaces](#59-handling-parameters-with-spaces)  
[5.10 Handling Lists of Parameters with Spaces](#510-handling-lists-of-parameters-with-spaces)  
[5.11 Counting Arguments](#511-counting-arguments)  
[5.12 Consuming Arguments](#512-consuming-arguments)  
[5.13 Getting Default Values](#513-getting-default-values)  
[5.14 Setting Default Values](#514-setting-default-values)  
[5.15 Using null as a Valid Default Value](#515-using-null-as-a-valid-default-value)  
[5.16 Using More than Just a Constant String for Default](#516-using-more-than-just-a-constant-string-for-default)  
[5.17 Giving an Error Message for Unset Parameters](#517-giving-an-error-message-for-unset-parameters)  
[5.18 Changing Pieces of a String](#518-changing-pieces-of-a-string)  
[5.19 Getting the Absolute Value of a Number](#519-getting-the-absolute-value-of-a-number)  

## 5.1 Documenting Your Script
* `# this is a comment`

## 5.4 Seperating Variable Names from Surrounding Text
* *`somescript starttext`*`${`*`somevar`*`}`*`somemoretext`*

## 5.5 Exporting Variables
* `export `*`myvar`*
* `export `*`name`*=*`value`*
* variables are *call by value* : variables changed in a called script are not changed in the calling script

## 5.6 Seeing All Variable Values
* `set`
* `env`
* `export -p`
* `declare -p`

## 5.7 Using Parameters in a Shell Script
* `${`*`number`*`}` : each parameter passed to a script is available using this syntax

## 5.8 Looping Over Argument Pased to a Script
* `$*` : a list containing all script parameters
  * looping over this list:  
    ``` bash
        for var in $*
        do
            echo $var;
            # YOUR COMMANDS GO HERE
        done
    ```

## 5.9 Handling Parameters with Spaces
> Consider the following script:
> ``` bash
> ls -l $1
> ```
> Call this script: `./myscript.sh "File with spaces in name.txt`  
> first line of output will be:  
> `ls: File: No such file or directory`  
> the spaces are interpreted as file seperators by scripts or programs called by our script
* enclose your variables in double quotes:  
```bash
ls -l "$1"
```

## 5.10 Handling Lists of Parameters with Spaces
* use the same technique as [5.8](#58-looping-over-argument-pased-to-a-script) but with `"$@"` instead of `"$*"`

## 5.11 Counting Arguments
* `$#` contains the number of arguments supplied
* `${#`*`varname`*`}` : gives length of var

## 5.12 Consuming Arguments
* use `shift` after processing the parameter:  
  ```bash
  if [[ $1 = -v]]
  then
    VERBOSE = 1
    shift
  fi
  # $* and $@ now don't have '-v' in them anymore
  ```

## 5.13 Getting Default Values
* use the `:-` operator in a variable
  ```bash
  #VAR1 = $1 except when $1 is not set or null
  VAR1 = ${1:-default_value}
  ```

## 5.14 Setting Default Values
* use the `:=` operator in a variable
  ```bash
  echo ${HOME:=/tmp}
  ```
  * Nothing will be assigned on positional variables like `$1` or `$*`, it will just return the right value. It behaves like the [`:-` operator](#513-getting-default-values)

## 5.15 Using null as a Valid Default Value
* use the `=` operator in a variable
  ```bash
  # HOME will only be set to /tmp when it was unset, not when it was empty
  echo ${HOME=/tmp}
  ```

## 5.16 Using More than Just a Constant String for Default
The following things can be on the right side of the `=` or `:=` operator in a variable:
* Parameter expansion: `${BASE:=${HOME}}`
* Tilde expansion: `${BASE:=~`*`username`*`}` : `BASE` gets set to home directory of user with given username
* Command substitution: `${BASE:="$(`*`some commands`*`)"}` : `BASE` gets set to the output of the given commans
* Arithmetic expansion : `${BASE:=$((`*`SOMEVAR`*`+1))}`

## 5.17 Giving an Error Message for Unset Parameters
> Force the user of your script to set certain parameters
* use the `:?` operator in a variable. Example:
  ```bash
  USAGE="SAMPLE ERROR MESSAGE FOR USAGE"
  VAR1=${1:?"ERROR MESSAGE FOR VAR1"}
  VAR2=${2:?"${USAGE}"}
  ```
* `set -u` at the beginning of your script
  * don't forget `set +u` at the end of your script if needed

## 5.18 Changing Pieces of a String
> change file suffixes
```bash
for FN in *.bad
do
    mv "${FN}" "${FN%bad}good"
done
```
Possible String manipulation operators (inside `${}`):
* *`name`*`:`*`number1`*`:`*`number2`* : substring of `name` starting at index `number1` and size `number2`
* `#`*`name`* : length of string
* *`name`*`#`*`pattern`* : Remove (shortest) front-anchored pattern
* *`name`*`##`*`pattern`* : Remove (longest) front-anchored pattern
* *`name`*`%`*`pattern`* : Remove (shortest) rear-anchored pattern
* *`name`*`%%`*`pattern`* : Remove (longest) rear-anchored pattern
* *`name`*`/`*`pattern`*`/`*`string`* : replace first occurence
* *`name`*`//`*`pattern`*`/`*`string`* : replace all occurences

## 5.19 Getting the Absolute Value of a Number
* `${`*`MYVAR`*`#-}` : [string manipulation, see 5.18](#518-changing-pieces-of-a-string)