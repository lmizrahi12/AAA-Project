# AAA-Project
Solving Sudoku's with Backtracking Algorithms

Running the tests:

1) runTests.sh <metric>
	<metric> E - Runs tests using the number of empty cells as x
	<metric> D - Runs tests using difficulty as x (difficulty=numEmpty/maxFilled)

2) aggregateCSV.sh <filepath> <index>
	<filepath> - Path to file which will be aggregated
	<index> - Index of value to aggregate (currently only 0 or 1, here we use only 0)

3) runTests_aggregateCSV.sh <metric>
	runTests_aggregateCSV.sh runs 1) and then 2) on the output of 1) to provide aggregated data in /Output folder
	<metric> E - Runs tests using the number of empty cells as x
	<metric> D - Runs tests using difficulty as x (difficulty=numEmpty/maxFilled)
