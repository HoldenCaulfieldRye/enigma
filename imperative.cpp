/*this file contains the imperative part of the program, i.e. functions*/

#include<iostream>
#include<string>
#include<cstring>

using namespace std;


/*function for errors*/
const char *error_description(int code) {
  switch(code) {
  case -10: 
    return "invalid index (file contains a number that is not between 0 and 25)";
  case -11:
    return "non numeric character (in file)";
  case -12:
    return "insufficient number of parameters (given in command line)";
  case -13:
    return "invalid input character (a non capital letter was input)";
  case -14:
    return "impossible plugboard configuration (file attempts to connect a contact with itself or with multiple contacts)";
  case -15:
    return "incorrect number of plugboard parameters (should be an even number of them)";
  case -16:
    return "invalid rotor mapping (an input is not mapped or an input has multiple mappings";
  case -17:
    return "no rotor starting position (insufficient number of starting positions)";
  case -18:
    return "invalid reflector mapping (file attempts to connect a contact with itself or with multiple contacts)";
  case -19:
    return "incorrect number of reflector parameters (file does not contain exactly 13 pairs of numbers)";
  }
 default:
   return "Unknown error";
}


/*checks that shell command (and its associated files) are valid*/
void check(int argc, char argv**)
{
  /*check sufficient number of parameters*/
  if (argc<3)
    error_description(-12);
  exit(-12);

  /*if rotors, check that there is a rotor starting positions file*/
  //string lastArg = argv[argc-1];
  if (argv[argc-1].compare(argv[argc-1].length-4, 4, ".pos") != 0 && argc>3) //lastArg.compare(lastArg.length - 4, 4, ".pos")
    error_description(-17); //no rotor starting position
  exit(-17);

  /*check that each configuration file is valid: valid index, numeric characters*/

  cout << "I hope you entered config files in pb, ref, (rot) order!" << endl << endl;
}
