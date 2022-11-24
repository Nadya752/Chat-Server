# USYD CODE CITATION ACKNOWLEDGEMENT
# I declare that the following lines of code have been copied from the
# tutorial solutions of Week 8 with only minor changes and it is not my own work. 

# Tutorial solutions for week 8 from INFO1110 course
# https://edstem.org/courses/4000

echo "######################"
echo "### Running tests! ###"
echo "######################"
echo
count=0 # number of test cases run so far

for folder in `ls -d tests/*/ | sort -V`; do
    name=$(basename "$folder")
    
    echo Running test $name.
    expected_file=tests/$name/$name.out

     
    gcc -fsanitize=address -Wall -Werror -std=gnu11 -g tests/$name/$name.c client.c -o $name
    ./test &
    ./$name  | diff - $expected_file  && echo "Test $name: passed!"|| echo "Test $name: failed!" &
    echo
    sleep 10 # Tests are 10 seconds long each to wait for the programs to finish before killing server.
    ./test & apid=$!;
    kill -SIGKILL $apid;

    count=$((count+1))
    
    find . -type p -delete
	rm -rf Gryffindor
    rm -rf Ravenclaw
    rm -rf Slytherin
	rm -f gevent

done

echo "Finished running $count tests!"