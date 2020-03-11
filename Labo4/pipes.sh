# command | command = unnamed pipe
mkfifo p # make first in first out (queue) pipe
echo hallo > p & # add something to pipe (best in background anders blijft pipe open in deze terminal)
# how to read from pipe
cat p

#Interprocess communicatie:
#Pipe is simplex verbrinding (éénrichtingsverkeer!)
int fds[2];
pipe(fds);
fork();
# altijd eerst pipe en dan fork (anders twee pipes!)
# omdat simplex dan één fd van child en één fd van parent sluiten


# 0 is stdin 1 is stdout
# 0 is lezen van pipe 1 is schrijven naar pipe