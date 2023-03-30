# TAI_P1

## In order to compile the programs, we have to go to src folder and execute the two commands below:
## (note that the programs are already compiled in bin folder)

### Command to compile the program cpm.cpp

g++ cpm.cpp -o cpm

### Command to compile the program cpm_gen.cpp

g++ cpm_gen.cpp -o cpm_gen

## Then, to run the programs we have to go to bin folder and execute the two commands below:

### Command to run the program cpm

 ./cpm filename word_length threshold min_prob max_num_fails
 
### Command to run the program cpm_gen

 ./cpm filename word_length threshold min_prob max_num_fails max_length_gen_text
