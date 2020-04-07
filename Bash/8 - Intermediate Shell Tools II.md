> Lorenzo De Bie
# 8 Intermediate Shell Tools II
## Table of Contents
[](#)  

## 8.1 Sorting Your Output
* `sort `*`file(s)`*
  * `-r` : sort in reverse order
  * `-f` : ignore case when sorting (aa & aA will be together)

## 8.2 Sorting Numbers
* `sort -n`
  * `-rn` to sort in descending order ([see 8.1](#81-sorting-your-output))

## 8.5 Sorting IP Addresses
* `sort -t. -n +3.0 `*`ipaddr.list`* : sorts by the last octet
  * `-t` : sets a seperator (`.` in the case of an ip address)
  * `+`*`n`*`.`*`m`* : works with `-t`: begin sorting from field `n`, and starting at character `m` in that field. Both start counting at 0  
  For ip addresses sorted by the last octet we sort the 3rd field starting at the 0th character
* `sort -t . -k 1,1n -k 2,2n -k 3,3n -k 4,4n `*`ipaddr.list`* : sorts by entire address
  * ``