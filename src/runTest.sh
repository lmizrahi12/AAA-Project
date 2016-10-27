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
	./backtracking.exe ../Input/27.txt $1
	./backtracking.exe ../Input/28.txt $1
	./backtracking.exe ../Input/29.txt $1
	./backtracking.exe ../Input/30.txt $1
	./backtracking.exe ../Input/31.txt $1
	./backtracking.exe ../Input/32.txt $1
	./backtracking.exe ../Input/33.txt $1
	./backtracking.exe ../Input/34.txt $1
	./backtracking.exe ../Input/35.txt $1
