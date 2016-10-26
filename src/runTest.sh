#!/bin/bash

	# $1 = <char> metric (E-numEmpty, D-numEmpty/maxFilled)
	
	cd backtracking
	make > /dev/null
	make clean > /dev/null
	
	./backtracking.exe ../Input/17.txt $1
	./backtracking.exe ../Input/18.txt $1
	./backtracking.exe ../Input/19.txt $1
	./backtracking.exe ../Input/20.txt $1
	./backtracking.exe ../Input/21.txt $1
	./backtracking.exe ../Input/22.txt $1
	./backtracking.exe ../Input/23.txt $1
	./backtracking.exe ../Input/24.txt $1
	./backtracking.exe ../Input/25.txt $1
	./backtracking.exe ../Input/26.txt $1
