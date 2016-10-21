#!/bin/bash
	
	# $1 = <char> metric (E-numEmpty, D-numEmpty/maxFilled)
	
	./runTest.sh $1 > Output/data$1.csv	#Outputs into Output/data.csv
	./aggregateCSV.sh ../Output/data$1.csv > Output/aggregateData$1.csv		#Turns above output into aggregated format and outputs to Output/aggregateData.csv
