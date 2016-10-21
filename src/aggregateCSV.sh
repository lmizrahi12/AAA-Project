#!/bin/bash
	
	# $1 = <char*> filepath to .csv to aggregate
	
	cd aggregate
	make makeaggregateCSV > /dev/null
	make clean > /dev/null

	./aggregateCSV $1 0;
