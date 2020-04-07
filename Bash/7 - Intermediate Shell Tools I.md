> Lorenzo De Bie
# 7 Intermediate Shell Tools I
## Table of Contents
[7.1 Sifting Through Files for a String](#71-sifting-through-files-for-a-string)    
[7.2 Getting Just he Filename from a Search](#72-getting-just-he-filename-from-a-search)  
[7.3 Getting a Simple True/False from a Search](#73-getting-a-simple-truefalse-from-a-search)  
[7.4 Searching for Text While Ignoring Case](#74-searching-for-text-while-ignoring-case)  
[7.5 Doing a Search in a Pipeline](#75-doing-a-search-in-a-pipeline)  
[7.6 Paring Down What the Search Finds](#76-paring-down-what-the-search-finds)  
[7.7 Searching with More Complex Patterns](#77-searching-with-more-complex-patterns)  
[7.8 Searching for an SSN](#78-searching-for-an-ssn)  

## 7.1 Sifting Through Files for a String
> Find all occurrences of a string in one or more files
* `grep `*`pattern files`*
  * `-h` : don't show filenames of match
  * `-c` : don't show mathes, but count how many times for each file
  * when no files are given, `grep` takes its input from `STDIN`

## 7.2 Getting Just he Filename from a Search
> just get the filename when looking for a string in files [see 7.1](#71-sifting-through-files-for-a-string)
* `grep -l `*`pattern files`*: gives only the filenames which contain pattern
  * Use this output as arguments for another command:  
  `rm -i $(grep -l 'This file is obsolete' *)`

## 7.3 Getting a Simple True/False from a Search
* `if grep -q `*`pattern file`*` ; then echo yes ; else echo nope ; fi`
  * `grep -q` does not produce any output, but exit code gets placed into `$?`
* redirecting `STDOUT` to `/dev/null`

## 7.4 Searching for Text While Ignoring Case
* `grep -i `*`pattern file`* : `-i` ignores case

## 7.5 Doing a Search in a Pipeline
> input is not a file, but output of another command/pipe
* *`some pipeline`*` | `*`of commands`*` | grep`
  * grep will read from `STDIN` when no files are given

## 7.6 Paring Down What the Search Finds
* `grep -v `*`pattern`*: removes all lines with pattern from `STDIN`

## 7.7 Searching with More Complex Patterns
* learn regex
* `man regex`

## 7.8 Searching for an SSN
* `grep '[0-9]\{3\}-\{0,1\}[0-9]\{2\}-\{0,1\}[0-9]\{4\}' `*`file`*