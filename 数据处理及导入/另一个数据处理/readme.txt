how to make data:
1. compile all
gcc -O2 -Wall -o all-stations all-stations.c
gcc -O2 -Wall -o trains trains.c
gcc -O2 -Wall -o tickets1 tickets1.c
gcc -O2 -Wall -o tickets2 tickets2.c


2. copy all-stations.txt to data1, run all-stations
3. delete all-stations.txt from data1

4. copy train scheduals (with proper format) to data1
5. in data1, ls > 1.txt
6. delete filenames we don't use (at the end of 1.txt) in 1.txt
7. copy trains to data1, run trains

8. run tickets1 to make a tickets info without date

9. please set date in tickets2.c, after set you have to compile it again
10. run tickets2 to make sql 

note:
1. can only deal with proper format files
2. if (tickets price == 0 || - || lost) then it's NULL
3. in traveltime and traveldistance, '-' will be 0
4. in stay '-' will be NULL and others are string  

 
