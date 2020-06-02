#!/bin/bash

# this script shows why we need thread synchronisation

aantal=0
while ((1)) ; do
	x=$(./bin/mutex1)
	(( x!=0 )) && echo "$aantal: kuch kuch... $x was de uitvoer"
	(( aantal++ ))
done
