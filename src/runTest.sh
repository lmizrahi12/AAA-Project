#!/bin/bash

	# $1 = <char> metric (E-numEmpty, D-numEmpty/maxFilled)
	
	cd backtracking
	make > /dev/null
	make clean > /dev/null
	
	./backtracking.exe ../Input/One.txt $1
	./backtracking.exe ../Input/Two.txt $1
	./backtracking.exe ../Input/Three.txt $1
	./backtracking.exe ../Input/Four.txt $1
	./backtracking.exe ../Input/Five.txt $1
