#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<iterator>
#include<sstream>
#include<string>
#include<cassert>

#ifndef _WIN32
#include<sys/stat.h>
#endif

using namespace std;

#include"enigma.h"


/*ENIGMA DEFINITIONS*/

/*minimalist constructor, declares machine components*/
Enigma::Enigma() : pb(this), rf(this) {
  rotor = NULL;
  errorCode = 0;
  nb_rotors = 0;
}

/*function for errors*/
void Enigma::errorDescription(int code) {
  switch(code) {
  case 0: return;
  case 3:
    cerr << "invalid index (a number in configuration file is not between 0 and 25)" << endl;
    errorCode = 3;  return;
  case 4:
    cerr << "non numeric character" << endl; 
    errorCode = 4;  return;
  case 1:
    cerr << "insufficient number of parameters (given in command line)" << endl; 
    errorCode = 1;  return;
  case 2:
    cerr << "invalid input character (a non capital letter was input)" << endl; 
    errorCode = 2;  return;
  case 5:
    cerr << "impossible plugboard configuration (file attempts to connect a contact with itself or with multiple contacts)" << endl; 
    errorCode = 5;  return;
  case 6:
    cerr << "incorrect number of plugboard parameters (should be an even number of them)" << endl;
    errorCode = 6;  return;
  case 7:
    cerr << "invalid rotor mapping (an input is not mapped or an input has multiple mappings)" << endl; 
    errorCode = 7;  return;
  case 8:
    cerr << "no rotor starting position (insufficient number of starting positions)" << endl;
    errorCode = 8;  return;
  case 9:
    cerr << "invalid reflector mapping (file attempts to connect a contact with itself or with multiple contacts)" << endl; 
    errorCode = 9;  return;
  case 10:
    cerr << "incorrect number of reflector parameters (file does not contain exactly 13 pairs of numbers)" << endl;
    errorCode = 10; return;
  case 11:
    cerr << "error opening configuration file" << endl; 
    errorCode = 11; return;
  default:
    cerr << "Unknown error" << endl; 
    errorCode = -1; return;
  }
}

int Enigma::getErrorCode() const {
  return errorCode;
}

/*checks for some errors in configuration files and configures machine components. configuration files are read through char-by-char to first make sure that only numeric and space/tab/NL characters are present. files will then be read int-by-int by component specific build methods to check for all other errors.*/
bool Enigma::build(int argc, char** argv) {

  /*check sufficient number of parameters*/
  if (argc<3) {
    errorDescription(1);        return false;
  }

  for (int i=1; i<argc && i<10; i++) {
    int fileStatus;
    struct stat fileInfo;
    char ch;
    ifstream file;

     /*use 'stat' system call to check that arg is a regular file. on linux, if a directory is given as command line arg, for some reason an ifstream can open it and 'file >>' endlessly reads in a char with ascii value 10. on windows, a directory cannot be opened by ifstream.*/
#ifndef _WIN32
    fileStatus = stat(argv[i], &fileInfo);
    if (fileStatus!=0) {
      cerr << "'"<< argv[i] << "' is a nonexistent pathname" << endl;
      errorDescription(11);     return false;
    }
    else {
      if (!S_ISREG(fileInfo.st_mode)) {
	cerr << "'" << argv[i] << "' does not point to a regular file" << endl;
	errorDescription(11);   return false;
      }
    }
#endif

    /*still need to check that the file can be opened.*/
    file.open(argv[i]);
    if (file.fail()) {
      errorDescription(11);     return false;
    }

    /*check that all characters are numeric, new line, carriage return, tab or space*/
    for (file >> ch; !file.eof(); file >> ch) {
      if ((int) ch < 48 || (int) ch > 57) {
	if (ch != 9 && ch != 10 && ch != 13 && ch != 32) {
	  cerr << "in file " << i << ": ";
	  errorDescription(4);  return false;
	}
      }
    }
    file.close();
  }

  /*reach here iif there are no errors. configuration can begin*/
  nb_rotors = argc - 4;
  if ( !pb.build(argv[1]) )     return false;
  if ( !rf.build(argv[2]) )     return false;
  if (nb_rotors>0) {
    rotor = new Rotor *[nb_rotors];
    for (int i=0; i<nb_rotors; i++) {
      rotor[i] = new Rotor(this);
      if( !rotor[i]->build(argv[i+3], argv[argc-1], i) ) //starting positions are set.
	return false;
    }  
  }
  return true;
}

bool Enigma::encrypt() {
  char outputLetter;

  /*encryption: while input file has valid letters to give, the process loops*/
  while (pb.getLetterFromInputFile())                  //loop ends if error input invalid or eof.
    {
      if (nb_rotors>0) {
	/*rightmost rotor rotates*/
	for (int i=nb_rotors-1; i>=0 && rotor[i]->rotate(); i--);

	/*plugboard sends letterIndex to rightmost rotor*/
	rotor[nb_rotors-1]->setLetterIndex(pb.scramble());

	/*each rotor with a left neighbour scrambles letterIndex & sends it to neighbour*/
	for (int i=nb_rotors-1; i>0; i--)
	  rotor[i-1]->setLetterIndex(rotor[i]->scramble());

	/*leftmost rotor sends letterIndex to reflector*/
	rf.setLetterIndex(rotor[0]->scramble());
      }
      /*if no rotors, plugboard leads straight to reflector*/
      else rf.setLetterIndex(pb.scramble());

      if (nb_rotors>0) {
	/*reflector sends letterIndex to leftmost rotor*/
	rotor[0]->setLetterIndex(rf.scramble());

	/*each rotor with a right neighbour inversely scrambles letterIndex & sends to neighbour*/
	for (int i=0; i<nb_rotors-1; i++)
	  rotor[i+1]->setLetterIndex(rotor[i]->inverseScramble());

	/*rightmost rotor sends letterIndex to plugboard*/
	pb.setLetterIndex(rotor[nb_rotors-1]->inverseScramble());
      }
      /*if no rotors, reflector leads straight to plugboard*/
      else pb.setLetterIndex(rf.scramble());

      /*plugboard inversely scrambles letterIndex*/
      pb.inverseScramble();

      /*plugboard outputs letter corresponding to letterIndex*/
      outputLetter = pb.getLetterIndex() + 65;
      cout << outputLetter;
    }

  if (errorCode!=0) 
    return false;                             //reach here iif invalid input.
  else return true;                           //reach here iif eof reached.
}

Enigma::~Enigma() {
  if (rotor) {
    for (int i=nb_rotors-1; i>=0; i--) {
      delete [] rotor[i];
      rotor[i] = NULL;
    }
    delete [] rotor;
    rotor = NULL;
  }
}
/*END OF ENIGMA DEFINITIONS*/


/*PIECEOFHARDWARE DEFINITIONS*/
PieceOfHardware::PieceOfHardware() {}

PieceOfHardware::PieceOfHardware(Enigma* _machine) {
  machine = _machine;
  letterIndex = 0;
  for (int i=0; i<27; i++)
    configArray[i] = 0;
}

bool PieceOfHardware::build(const char* configFilename, int type)
{
  int num, i=0;
  ifstream file;
  file.open(configFilename);

  /*set configArray*/
  for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25) { 
      machine->errorDescription(3);     return false;
    }
    configArray[i] = num;

    /*check no two identical entries in configArray*/
    for (int j=i-1; j>=0; j--) {
      if (configArray[i]==configArray[j]) {
	if (type==plugboard) {
	  machine->errorDescription(5); return false;
	}
	else {
	  machine->errorDescription(9); return false;
	}
      }
    }
  }

  if (type==plugboard && i%2!=0) {            //check for an even number of plugboard parameters.
    machine->errorDescription(6);       return false;
  }
  if (type==reflector && i != 26) {           //check for 13 pairs of reflector parameter numbers.
    machine->errorDescription(10);      return false;
  }

  configArray[i] = sintinel;                  //reach here iif config perfectly valid.
  file.close();
  return true;
}

/*storing the index value of the letter rather than the letter itself is great because it keeps this function very simple*/
void PieceOfHardware::setLetterIndex(int const &value) {
  assert(value<26 && value>=0);
  letterIndex = value;
}
/*END OF PIECEOFHARDWARE DEFINITIONS*/


/*PLUGBOARD DEFINITIONS*/
Plugboard::Plugboard() {}
Plugboard::Plugboard(Enigma *_machine) : PieceOfHardware::PieceOfHardware(_machine) {}

bool Plugboard::build(const char* configFilename) {
  return PieceOfHardware::build(configFilename, plugboard);
}

/*sets letter: reads in one character from inFile, performs checks, ignores if space etc if valid, assigns index value to letter and returns true; if fail (inc eof) returns false*/
bool Plugboard::getLetterFromInputFile() {
  int ascii;
  char input;

  for (cin >> ws, cin >> input, ascii = (int) input; 
       !cin.eof(); 
       cin >> ws, cin >> input, ascii = (int) input) {

    /*check that input is a capital letter, new line, carriage return, tab or space*/
    if (ascii>91 || ascii<64) {
      if (ascii!=10 && ascii!=13 && ascii!=9 && ascii!=32) {
	machine->errorDescription(2); return false;
      }                                 //else letter is new line, carriage return, tab or space
    }                                   //so do nothing.       
    else {
      letterIndex = ascii - 65;
      return true;
    }
  }
  return false;                         //reach here iif end of file has been reached.
}

int Plugboard::scramble(int number) const {
  assert(number>=0 && number<26);
  for (int i=0; configArray[i] != sintinel; i++) {
    if (configArray[i]==number) {
      if (i % 2 == 0) {                 //reach here iif i is even, so letter(configArray[i]) 
	return configArray[i+1];        //and letter(configArray[i+1]) are connected.
      }
      return configArray[i-1];          //reach here iif i is odd, so letter(configArray[i]) and 
    }                                   //letter(configArray[i-1]) are connected.
  }
  return number;                        //letter(number) is not scrambled by plugboard, so
}                                       //letterIndex stays the same.

int Plugboard::scramble() {
  return scramble(letterIndex);
}

void Plugboard::inverseScramble() {     //for any valid config file, scramble() is a
  for (int i=0; i<26; i++) {            //bijection on {0,.., 25}.
    if (scramble(i) == letterIndex) {   //so inverse function of scramble() exists; this is it.
      letterIndex = i;
      return;
    }
  }
  machine->errorDescription(-1);        //should not ever reach here.
  letterIndex = -1;
}

int Plugboard::getLetterIndex() const {
  return letterIndex;
}
/*END OF PLUGBOARD DEFINITIONS*/


/*REFLECTOR DEFINITIONS*/
Reflector::Reflector() {}
Reflector::Reflector(Enigma *_machine) : PieceOfHardware::PieceOfHardware(_machine) {}

bool Reflector::build(const char* configFilename) {
  return PieceOfHardware::build(configFilename, reflector);
}

int Reflector::scramble() {
  for (int i=0; configArray[i] != sintinel; i++) {
    if (configArray[i]==letterIndex) {
      if (i % 2 == 0) {                 //i is even, so letter(configArray[i]) and
	return configArray[i+1];        //letter(configArray[i+1]) are connected. 
      }                              
      return configArray[i-1];          //i is odd, so letter(configArray[i]) and
    }                                   //letter(configArray[i-1]) are connected.
  }
  machine->errorDescription(-1);         //all letters should have a mapping, so should
  return -1;                            //not ever reach here.
}
/*END OF REFLECTOR DEFINITIONS*/


/*ROTOR DEFINITIONS*/
Rotor::Rotor() {}
Rotor::Rotor(Enigma *_machine) : PieceOfHardware(_machine) {
  for (int i=0; i<50; i++)  
    notches[i] = 0;
  rotPos = 0;
}

bool Rotor::build(const char* configFilename, const char* posFilename, int rotor_nb)
{
  int num, i=0, k=0;
  ifstream file;
  file.open(configFilename);
  istream_iterator<int> begin(file), end;

  /*set configArray*/
  for (; begin!=end && i<26; ++begin, i++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25) {
      machine->errorDescription(3);   return false;
    }

    configArray[i] = num;

    /*check no two identical entries in configArray*/
    for (int j=i-1; j>=0; j--) {
      if (configArray[i]==configArray[j]) {
	machine->errorDescription(7); return false;
      }
    }
  }

  /*check that we have every input mapped. reach here iif no identical configArray entries, so merely need to check size)*/
  if (i != 26) {
    machine->errorDescription(7);     return false;
  }

  /*set notches, starting from where we left off*/
  for (; begin!=end; ++begin, k++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25) {
      machine->errorDescription(3);   return false;
    }

    notches[k] = num;
  }

  /*reach here iif rotor config perfectly valid*/
  configArray[i] = sintinel;
  file.close();
  notches[k] = sintinel;

  /*set rotPos*/
  int count;
  file.open(posFilename);
  istream_iterator<int> begin2(file), end2;
  for (count=0; count < rotor_nb && begin2!=end2; ++begin2, count++);

  /*check that there is a starting position for the rotor*/
  if (begin2==end2) {
    machine->errorDescription(8);     return false;
  }

  /*check that index is valid*/
  if (*begin2 < 0 || *begin2 > 25) {
    machine->errorDescription(3);     return false;
  }

  rotPos = *begin2;
  return true;
}

int Rotor::getRotPos() const {
  return rotPos;
}

bool Rotor::rotate() {
  rotPos = (rotPos + 1) % 26;
  for (int i=0; notches[i] != sintinel; i++) {
    if (rotPos == notches[i]) {
       return true;                       //if leftmost rotor does rotate, 'true' will be returned but
    }                                     //'i>0' condition in 'for' loop in main will fail anyway.
  }    
  return false;
}

int Rotor::scramble(int number) const {
  assert(number>=0 && number<26);
  return (configArray[(number + rotPos) % 26] + 26 - rotPos) % 26;
}

int Rotor::scramble() {
  return scramble(letterIndex);
}

int Rotor::inverseScramble() {
  for (int i=0; i<26; i++) { 
    if (scramble(i) == letterIndex)
      return i;
  }
  machine->errorDescription(-1);          //should not ever reach here.
  return -1;
}
/*END OF ROTOR DEFINITIONS*/
