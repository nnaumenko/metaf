#!/bin/bash
	tempfile=$(mktemp) 
	emrun --browser firefox bin/test/main.html --gtest_output="xml:test.xml" | tee $tempfile 
	sed -n '/<<<<\/test.xml:BEGIN>>>>/,/<<<<\/test.xml:END>>>>/{ /<<<<\/test.xml:BEGIN>>>>/d; /<<<<\/test.xml:END>>>>/d; p }' $tempfile > "bin/test/test.xml" 
	rm -f $tempfile 
