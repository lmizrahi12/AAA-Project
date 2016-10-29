#!/bin/bash
	
	# $1 = <char> metric (E-numEmpty, D-numEmpty/maxFilled)

	./runTest.sh $1 > Output/data$1.csv	#Outputs into Output/data.csv
	./aggregateCSV_average.sh ../Output/data$1.csv > Output/aggregateData$1_average.csv		#Turns above output into aggregated (average) format and outputs to Output/aggregateData(X)_average.csv
	./aggregateCSV_max.sh ../Output/data$1.csv > Output/aggregateData$1_max.csv		#Turns above output into aggregated (average) format and outputs to Output/aggregateData(X)_max.csv
	./aggregateCSV_min.sh ../Output/data$1.csv > Output/aggregateData$1_min.csv		#Turns above output into aggregated (average) format and outputs to Output/aggregateData(X)_min.csv
