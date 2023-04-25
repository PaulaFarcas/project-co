
file="$1"

if test ! -f $1 
	then echo "not regular file"
else
#	echo $file
	output=$(gcc -Wall -Wextra "$file" 2>&1)
	nr_errors=$(echo "$output" | grep -c 'error')
	nr_warnings=$(echo "$output" | grep -c 'warning')

	echo "Error: $nr_error"
	echo "Warnings: $nr_warnings"

fi

