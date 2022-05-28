all: 
    gcc -o uart uart2.c -l WiringPi 

run: ./uart

clean: 
    rm -fr uart
