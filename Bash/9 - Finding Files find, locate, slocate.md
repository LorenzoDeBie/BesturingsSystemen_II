> Lorenzo De Bie
# 9 Finding Files: find, locate, slocate
## Table of Contents
[9.1 Finding All Your MP3 Files](#91-finding-all-your-mp3-files)  

## 9.1 Finding All Your MP3 Files
* `find `*`path`*` -name '`*`pattern`*`'`
  * Options are executed sequentially, they are like filters. `find` climbs through the filesystem and passes each file to the options: when the option returns true it is passed to the next one.
  * `-print` will print the name to `STDOUT`
  * `-exec` : executes the command given, command needs to end with `\;`. `{}` gets replaced with the filename
    * `find . -name '*.mp3' -print -exec mv '{}' ~/songs` : moves all mp3 files in this or sub directories to `~/songs`