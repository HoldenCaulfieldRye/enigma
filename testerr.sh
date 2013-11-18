#!/bin/bash

#echo "4 10 12 5 11 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0" > rotor_less.rot
#echo "4 10 12 5 11 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 8 1 17 2 9 17" > rotor1.rot
#0 9 3 10 18 8 17 20 23 1 11 7 22 19 12 2 16 6 25 13 15 24 5 21 14 4 5
#
#ref: 0 4 1 9 2 12 3 25 5 11 6 24 7 23 8 21 10 22 13 17 14 16 15 20 18 19
#
# 25 8
#echo "18 10" > startpos3.pos
#echo "11 25" > startpos4.pos
#echo "5 25 24" > startpos5.pos


echo "case 1)"
./enigma plugboards/null.pb > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 1 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""


echo "case 2)"
echo "HELLOwORLD" > test.txt
./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 2 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""
rm test.txt


echo "case 3)"
echo "HELLOWORLD" > test.txt
echo "4 10 12 5 11 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 81 1 17 2 9 17" > rotor1.rot
./enigma plugboards/I.pb reflectors/I.rf rotor1.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 3 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""

echo "case 4)"
echo "HELLOWORLD" > test.txt
echo "4 10 12 5 11x 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 81 1 17 2 9 17" > rotor1.rot
./enigma plugboards/I.pb reflectors/I.rf rotor1.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 4 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""


echo "case 5)"
echo "HELLOWORLD" > test.txt
echo "1 5 5 3" > pb1.pb
./enigma pb1.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 5 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""

echo "case 6)"
echo "HELLOWORLD" > test.txt
echo "1 5 3" > pb1.pb
./enigma pb1.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 6 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""


echo "case 7)"
echo "HELLOWORLD" > test.txt
echo "4 4 12 5 11 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 8 1 17 2 9 17" > rotor1.rot
./enigma plugboards/I.pb reflectors/I.rf rotor1.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 7 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""

echo "case 8)"
echo "HELLOWORLD" > test.txt
echo "1 2" > pos1.pos
./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot pos1.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 8 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""


echo "case 9)"
echo "HELLOWORLD" > test.txt
echo "0 0 1 9 2 12 3 25 5 11 6 24 7 23 8 21 10 22 13 17 14 16 15 20 18 19" > ref1.rf
./enigma plugboards/I.pb ref1.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 9 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""


echo "case 10)"
echo "HELLOWORLD" > test.txt
echo "1 2" > ref1.rf
./enigma plugboards/I.pb ref1.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 10 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""

echo "case 11)"
echo "HELLOWORLD" > test.txt
./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot steffen.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
if [ $exitstat == 11 ]
then
	echo "not wrong (possibly correct)"
else
	echo "absolutely wrong"
fi
echo ""

echo "=============================="
echo "More edge cases"
echo "=============================="
echo "case 12: non-numeric character in config file"
echo "HELLOWORLD" > test.txt
echo "4 a 12 5 11 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 81 1 17 2 9 17" > rotor1.rot
./enigma plugboards/I.pb reflectors/I.rf rotor1.rot rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 13: negative number in config file"
echo "HELLOWORLD" > test.txt
echo "4 -10 12 5 11 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 81 1 17 2 9 17" > negref.rf
./enigma plugboards/I.pb negref.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 14: too large number in config file"
echo "HELLOWORLD" > test.txt
echo "0 44 1 9 2 12 3 25 5 11 6 24 7 23 8 21 10 22 13 17 14 16 15 20 18 19" > toolargeref.rf
./enigma plugboards/I.pb toolargeref.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 15: no problem, many rotors!"
echo "HELLOWORLD" > test.txt
echo "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20" > manyrot.pos
./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot rotors/I.rot rotors/II.rot manyrot.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 16: does the input routine skip whitespace?"
echo "HELLO                     W  ORLD" > test.txt
echo "HELLOWORLD" > default.txt
./enigma plugboards/I.pb reflectors/I.rf rotors/II.rot rotors/I.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo "correct case:"
./enigma plugboards/I.pb reflectors/I.rf rotors/II.rot rotors/I.rot rotors/I.pos < default.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 17: invalid input value in input routine (lowercase)"
echo "HElLOWORLD" > test.txt
./enigma plugboards/I.pb reflectors/I.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 18: invalid input value in input routine (punctuation)"
echo "HE,LLOWORLD" > test.txt
./enigma plugboards/I.pb reflectors/I.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 19: odd number of plugboards params"
echo "HELLOWORLD" > test.txt
echo "1 2 3" > oddnum.pb
./enigma oddnum.pb reflectors/I.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 20: connect plug to itself"
echo "HELLOWORLD" > test.txt
echo "1 2 3 3" > connecttoself.pb
./enigma connecttoself.pb reflectors/I.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 21: repeated value in rotor mapping"
echo "HELLOWORLD" > test.txt
echo "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 24" > repeatedrotor.rot
./enigma plugboards/I.pb reflectors/I.rf rotors/II.rot repeatedrotor.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 22: too few values in rotor mapping"
echo "HELLOWORLD" > test.txt
echo "1 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 24" > toofew.rot
./enigma plugboards/I.pb reflectors/I.rf rotors/II.rot toofew.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 23: repeated number in reflector mapping"
echo "HELLOWORLD" > test.txt
echo "4 10 12 5 10 6 3 16 21 25 13 19 14 22 24 7 23 20 18 15 0 81 1 17 2 9 17" > repeatedrf.rf
./enigma plugboards/I.pb repeatedrf.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

echo "case 24: too few reflector mapping values"
echo "HELLOWORLD" > test.txt
echo "4 12 5 10 6 3 16 21 25 13 19 14 22 24 7" > toofew.rf
./enigma plugboards/I.pb toofew.rf rotors/II.rot rotors/III.rot rotors/I.pos < test.txt > output.txt 2> error.txt
exitstat=$?
output=$(cat output.txt)
error=$(cat error.txt)
echo "exit status: $exitstat"
echo "stdout: $output"
echo "stderr: $error"
echo ""

rm error.txt output.txt test.txt default.txt
rm *.rf *.pos *.rot *.pb