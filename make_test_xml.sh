#!/bin/bash
	tempfile=$(mktemp)
	emrun --browser firefox bin/test/main.html --gtest_output="xml:test.xml" > $tempfile
	sed -n '/<<<<BEGIN>>>>/,/<<<<END>>>>/{ /<<<<BEGIN>>>>/d; /<<<<END>>>>/d; p }' $tempfile > bin/test/test.xml
	rm -f $tempfile