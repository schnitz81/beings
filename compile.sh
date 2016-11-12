#!/bin/bash
echo
echo
echo
gcc -v -g -Wall -c main.c && gcc -g -Wall -c world.c && gcc -g -Wall -c being.c && gcc -g -Wall -c event.c && gcc -g -Wall -c ai.c && gcc -v -g -o beings main.c world.c being.c event.c ai.c -lcurses

