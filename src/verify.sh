#!/bin/bash
	
	# $1 - Number of completed cells to test (6 sudokus per number of completed cells)

	cd backtracking
	make > /dev/null
	make clean > /dev/null
	
	./backtracking.exe ../Input/$1.txt v
