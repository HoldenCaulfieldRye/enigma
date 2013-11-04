/*this file contains the imperative part of the program, i.e. functions*/

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>

using namespace std;

#include"enigma.h"

/*function for errors*/
void error_description(int code) {
  switch(code) {
  case -10: 
    cout << "invalid index (file contains a number that is not between 0 and 25)";
    exit(-10);
  case -11:
    cout << "non numeric character (in file)";
    exit(-11);
  case -12:
    cout << "insufficient number of parameters (given in command line)";
    exit(-12);
  case -13:
    cout << "invalid input character (a non capital letter was input)";
    exit(-13);
  case -14:
    cout << "impossible plugboard configuration (file attempts to connect a contact with itself or with multiple contacts)";
    exit(-14);
  case -15:
    cout << "incorrect number of plugboard parameters (should be an even number of them)";
    exit(-15);
  case -16:
    cout << "invalid rotor mapping (an input is not mapped or an input has multiple mappings";
    exit(-16);
  case -17:
    cout << "no rotor starting position (insufficient number of starting positions)";
    exit(-17);
  case -18:
    cout << "invalid reflector mapping (file attempts to connect a contact with itself or with multiple contacts)";
    exit(-18);
  case -19:
    cout << "incorrect number of reflector parameters (file does not contain exactly 13 pairs of numbers)";
    exit(-19);
 default:
   cout << "Unknown error";
    exit(-1);
  }
}


/*checks that shell command (and its associated files) are valid*/
void check(int argc, char **argv)
{
  /*check sufficient number of parameters*/
  if (argc<3)
    error_description(-12);
  exit(-12);

  /*if rotors, check that there is a rotor starting positions file*/
  string lastArg = argv[argc-1];
  if (lastArg.compare(lastArg.size() - 4, 4, ".pos") != 0 && argc>3) {
    error_description(-17); //no rotor starting position
    exit(-17);
  }

  /*check that each configuration file is valid: valid index, numeric characters*/
  for (int i=0; i<argc; i++) {
    ifstream file;
    file.open(argv[i]);
    file.close();
  }

  cout << "I hope you entered config files in pb, ref, (rot) order!" << endl << endl;
}


int *getInput()
{
  int i = 0;
  char input;
  int message[500];

  do {
    cout << "Enter input (enter '.' to stop): ";
    cin >> input;


    if ((int)input<91 && (int)input>64) {
      if (input!='.' && input!=10 && input!=13 && input!=9 && input!=32) {
	/*if input is neither a capital letter or a new line, carriage return, tab, space, then ERROR*/
	error_description(-13);
	exit(-13);
      }
	/*if input is a new line, carriage return, tab, space, then do nothing*/
    }

    else { /*if input is a capital letter, add it to message*/
    message[i] = (int) input - 65;
    i++;
    }

  } while (input != '.');

  message[i] = sintinel;
  return message;
}


  
