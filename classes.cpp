#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<iterator>
#include<sstream>
#include<string>
#include<cassert>

using namespace std;

#include"enigma.h"


/*PIECEOFHARDWARE DEFINITIONS*/
PieceOfHardware::PieceOfHardware() {
}


PieceOfHardware::PieceOfHardware(const char* configFilename, int type)
{
  int num, i=0;
  ifstream file;
  file.open(configFilename);

  /*set configArray*/
  for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25) { 
      error(-10);
    }
    configArray[i] = num;

    /*check no two identical entries in configArray*/
    for (int j=i-1; j>=0; j--) {
      if (configArray[i]==configArray[j]) {
	if (type==plugboard)
	  error(-14);
	if (type==reflector)
	  error(-18);
      }
    }
  }

  if (type==plugboard) { //check that we have an even number of plugboard parameters
    if (i % 2 != 00) 
      error(-15);
  }
  else if(type==reflector) { //check that we have 13 pairs of reflector parameter numbers
    if (i != 26)
      error(-19);
  }

  /*reach here iif config perfectly valid*/
  configArray[i] = sintinel;
  configArraySize = i;
  file.close();
}


/*function for errors*/
void PieceOfHardware::error(int code) {
  switch(code) {
  case -12:
    cerr << "insufficient number of parameters (given in command line)" << endl;
    exit(-12);
  case -13:
    cerr << "invalid input character (a non capital letter was input)" << endl;
    exit(-13);
  case -14:
    cerr << "impossible plugboard configuration (file attempts to connect a contact with itself or with multiple contacts)" << endl;
    exit(-14);
  case -15:
    cerr << "incorrect number of plugboard parameters (should be an even number of them)" << endl;
    exit(-15);
  case -16:
    cerr << "invalid rotor mapping (an input is not mapped or an input has multiple mappings" << endl;
    exit(-16);
  case -17:
    cerr << "no rotor starting position (insufficient number of starting positions)" << endl;
    exit(-17);
  case -18:
    cerr << "invalid reflector mapping (file attempts to connect a contact with itself or with multiple contacts)" << endl;
    exit(-18);
  case -19:
    cerr << "incorrect number of reflector parameters (file does not contain exactly 13 pairs of numbers)" << endl;
    exit(-19);
  default:
    cerr << "Unknown error" << endl;
    exit(-1);
  }
}


int PieceOfHardware::showLetterIndex() const {
  return letterIndex;
}


/* storing the index value of the letter rather than the letter itself is great because it keeps this function very simple */
void PieceOfHardware::setLetterIndex(int value) {
  letterIndex = value;
}
/*END OF PIECEOFHARDWARE DEFINITIONS*/


/*PLUGBOARD DEFINITIONS*/
Plugboard::Plugboard(const char* configFilename) : PieceOfHardware::PieceOfHardware(configFilename, plugboard) {
}


/*set letter: read in one character from inFile, perform checks, ignore if space etc*/
/*if valid, assign index value to letter and return true; if fail (inc eof) return false*/
bool Plugboard::getLetterFromInputFile()
{
  int ascii;
  char input;

  for (cin >> ws, cin >> input; !cin.eof(); cin >> ws, cin >> input) {

    /*check that input is a capital letter, new line, carriage return, tab or space*/
    ascii = (int) input;
    if (ascii>91 || ascii<64) {
      if (ascii!=10 && ascii!=13 && ascii!=9 && ascii!=32)
	error(-13);
      //else letter is new line, carriage return, tab or space, so do nothing
    }                         
    else {
      letterIndex = ascii - 65;
      return true;
    }
  }
  return false;        //reach here iif end of file has been reached
}


void Plugboard::scramble() {
  for (int i=0; i<configArraySize; i++) {
    if (configArray[i]==letterIndex) {
      if (i % 2 == 0) {
	/*i is even, so letter(configArray[i]) and letter(configArray[i+1]) are connected*/
	letterIndex = configArray[i+1]; 
	return;
      }
      /*i is odd, so letter(configArray[i]) and letter(configArray[i-1]) are connected*/
      letterIndex = configArray[i-1];
      return;
    }
  }
  return; //reach here iif letter(letterIndex) is not scrambled by plugboard 
}


int Plugboard::scramble(int number) {
  assert(number>=0 && number<26);
  for (int i=0; i<configArraySize; i++) {
    if (configArray[i]==number) {
      if (i % 2 == 0) {
	return configArray[i+1];
      }
      return configArray[i-1];
    }
  }
  return number; //letter(number) is not scrambled by plugboard, so letterIndex stays the same
}


void Plugboard::inverseScramble() { //for any valid config file, scramble() is a bijection on {0,.., 25}
  for (int i=0; i<26; i++) { //this inverse function of scramble() exists, and this is it
    if (scramble(i) == letterIndex) {
      letterIndex = i;
      return;
    }
  }
  error(-1);                 //should not ever reach here
}
/*END OF PLUGBOARD DEFINITIONS*/


/*REFLECTOR DEFINITIONS*/
Reflector::Reflector(const char* configFilename) : PieceOfHardware::PieceOfHardware(configFilename, reflector) {
}


void Reflector::scramble() {
  for (int i=0; i<configArraySize; i++) {
    if (configArray[i]==letterIndex) {
      if (i % 2 == 0) {
	/*i is even, so letter(configArray[i]) and letter(configArray[i+1]) are connected*/
	letterIndex = configArray[i+1]; 
	return;
      }
      /*i is odd, so letter(configArray[i]) and letter(configArray[i-1]) are connected*/
      letterIndex = configArray[i-1];
      return;
    }
  }
  error(-1); //all letters should have a mapping, so should not ever reach here
}
/*END OF REFLECTOR DEFINITIONS*/


/*ROTOR DEFINITIONS*/
Rotor::Rotor(const char* configFilename, const char* posFilename, int rotor_nb) : PieceOfHardware()
{
  int num, i=0, k=0;
  ifstream file;
  file.open(configFilename);
  istream_iterator<int> begin(file), end;

  /*set configArray*/
  for (; begin!=end && i<26; ++begin, i++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25)
      error(-10);

    configArray[i] = num;

    /*check no two identical entries in configArray*/
    for (int j=i-1; j>=0; j--) {
      if (configArray[i]==configArray[j])
	error(-16);
    }
  }

  /*check that we have every input mapped*/
  /*reach here iif no identical configArray entries, so merely need to check size)*/
  if (i != 26)
    error(-16);

  /*set notches, starting from where we left off*/
  for (; begin!=end; ++begin, k++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25)
      error(-10);

    notches[k] = num;
  }

  /*reach here iif rotor config perfectly valid*/
  configArray[i] = sintinel;
  configArraySize = i;
  file.close();
  notches[k] = sintinel;

  /*set rotPos*/
  file.open(posFilename);
  istream_iterator<int> begin2(file), end2;
  for (int count=0; count < rotor_nb && begin2!=end2; ++begin2, count++);

  /*check that index is valid*/
  if (*begin2 < 0 || *begin2 > 25)
    error(-10);
  rotPos = *begin2;
}


int Rotor::showRotPos() const {
  return rotPos;
}


void Rotor::showNotches() {
  for (int i=0; notches[i]!=sintinel; i++)
    cerr << notches[i] << ", ";
  cerr << endl;
}


bool Rotor::rotate() {
  rotPos = (rotPos + 1) % 26;
  for (int i=0; notches[i] != sintinel; i++) {
    if (rotPos == notches[i]) {
      cerr << "notch met!" << endl;
      return true;   //if leftmost rotor does rotate, 'true' will be returned but...
    }
  }                  //...'i>0' condition in 'for' loop in main will fail anyway
  return false;
}


void Rotor::scramble() {
  letterIndex = (configArray[(letterIndex + rotPos) % 26] + 26 - rotPos) % 26;
}


int Rotor::scramble(int number)  {
  assert(number>=0 && number<26);
  return (configArray[(number + rotPos) % 26] + 26 - rotPos) % 26;
}


void Rotor::inverseScramble() { //for any valid config file, scramble() is a bijection on {0,.., 25}
  for (int i=0; i<26; i++) {    //this inverse function of scramble() exists, and this is it
    if (scramble(i) == letterIndex) {
      letterIndex = i;
      return;
    }
  }
  error(-1);                 //should not ever reach here
}
/*END OF ROTOR DEFINITIONS*/

/*make sure that you're picking up errors correctly with the error config files you created!*/
