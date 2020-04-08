> Lorenzo De Bie
# 8 Intermediate Shell Tools II
## Table of Contents
[8.1 Sorting Your Output](#81-sorting-your-output)  
[8.2 Sorting Numbers](#82-sorting-numbers)  
[8.3 Sorting IP Addresses](#83-sorting-ip-addresses)  
[8.4 Cutting Out Parts of Your Output](#84-cutting-out-parts-of-your-output)  
[8.5 Removing Duplicate Lines](#85-removing-duplicate-lines)  
[8.9 Translating Characters](#89-translating-characters)  
[8.10 Converting Uppercse to Lowercase](#810-converting-uppercse-to-lowercase)  
[8.11 Converting DOS Files to Linux Format](#811-converting-dos-files-to-linux-format)  
[8.12 Removing Smart Quotes](#812-removing-smart-quotes)  
[8.13 Counting Lines, Words, or Characters in a File](#813-counting-lines-words-or-characters-in-a-file)  

## 8.1 Sorting Your Output
* `sort `*`file(s)`*
  * `-r` : sort in reverse order
  * `-f` : ignore case when sorting (aa & aA will be together)

## 8.2 Sorting Numbers
* `sort -n`
  * `-rn` to sort in descending order ([see 8.1](#81-sorting-your-output))

## 8.3 Sorting IP Addresses
* `sort -t. -n +3.0 `*`ipaddr.list`* : sorts by the last octet
  * `-t` : sets a seperator (`.` in the case of an ip address)
  * `+`*`n`*`.`*`m`* : works with `-t`: begin sorting from field `n`, and starting at character `m` in that field. Both start counting at 0  
  For ip addresses sorted by the last octet we sort the 3rd field starting at the 0th character
* `sort -t . -k 1,1n -k 2,2n -k 3,3n -k 4,4n `*`ipaddr.list`* : sorts by entire address
  * `-k `*`x`*`,`*`y`*`n` : start sorting at the beginning of field *`x`*, stop at the ending of field *`y`* and sort nummericaly `n`
    * fields start at 1 instead of 0
    * default `y` = end of line
    * remove `n` to sort alphabetically
* `-S` : turns of *stable* sorting in some versions of linux (NetBSD for example)

## 8.4 Cutting Out Parts of Your Output
> You have fixed width or column-based data, and only want to see a specific part or column from that data
* `cut`
  *  `-c`*`n`*`-`*`m`* : only shows char `n` to char `m` of each line
     * `m` defaults to end of line
     * Columns start at 1
  * `-d` : set a delimiter
    * you can use multiple `cut` commands in a row for nonmatching delimiters  
    `cut -d'[' -f2 delimited_data | cut -d']' -f1` : gets data between '[' and ']'

## 8.5 Removing Duplicate Lines
* `sort -u` : use this when you were sorting the output, you can just add `-u` to your command
* `uniq` : pipe output to uniq
  * **output needs to be sorted!**
  * `-c` : puts the number of occurences before each line
  * `-d` : only see duplicates

## 8.9 Translating Characters
> convert one char into another in the whole file
* `tr '`*`char1`*`' '`*`char2`*`' <`*`inputfile`*` >`*`outputfile`*
  * `char1` & `char2` can be multiple chars, `tr` will do a *one-for-one* substitution:  
  first char of `char1` will be replaced with first char of `char2`, second char of `char1` will be replaced with the second char of `char2` and so forth
    * when length of `char2` < length of `char1`, last char of `char2` will be repeated untill the lengths match

## 8.10 Converting Uppercse to Lowercase
* `tr '[:upper:]' '[:lower:]' <`*`inputfile`*` >`*`outputfile`*
* `tr 'A-Z' 'a-z' <`*`inputfile`*` >`*`outputfile`*
  * use this when you want to translate subranges
* ROT13 with `tr`:  
`tr 'A-Za-z' 'N-ZA-Mn-za-m'`
    * ROT13 uses the same algorithm to encode and decode text

## 8.11 Converting DOS Files to Linux Format
> DOS line ends with `\r\n`, Linux line ends with `\n`
* `tr -d '\r' <`*`file.dos`*` >`*`file.linux`*
  * `-d` will delete all specified chars
  * **This will also delete all `\r` characters which are not at the end of a line**
* Some more escape sequences:
  * `\`*`xyz`* : character with octal value *`xyz`*
  * `\\` : backslash
  * `\a` : bell
  * `\b` : backspace
  * `\f` : form feed
  * `\n` : newline
  * `\r` : return
  * `\t` : horizontal tab
  * `\v` : vertical tab

## 8.12 Removing Smart Quotes
> MS Word uses characters from the Windows-1252 character set, smart quotes are one of those characters. We want to change them back to regular quotes
* `tr '\221\222\223\224\226\227' '\047\047""--' <`*`input`*` >`*`output`*

## 8.13 Counting Lines, Words, or Characters in a File
* `wc `*`data_file`* : displays lines, words and characters of *`data_file`*
  * `-l` : lines only
  * `-w` : words only
  * `-c` : characters only
* `wc <`*`data_file`* : filename will not be shown in output