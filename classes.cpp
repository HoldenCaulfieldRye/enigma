#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
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
  switch (code) {
    case 1:
      cerr << "insufficient number of parameters (given in command line)" << endl; 
      errorCode=1;  return;
    case 2:
      cerr << "invalid input character (a non capital letter was input)" << endl; 
      errorCode=2;  return;
    default:
      cerr << "Unknown error" << endl; 
      errorCode=-1; return;
    }
}

/*overload: also specifies filename in which error occurred and closes ifstream*/
void Enigma::errorDescription(int code, const char* fileName, ifstream &file) {
  if (code!=0) {
    file.close();
    cerr << "in " << fileName << ": ";
  }
  switch(code) {
  case 0:           return;
  case 3:
    cerr << "invalid index (a number in configuration file is not between 0 and 25)" << endl;
    errorCode = 3;  return;
  case 4:
    cerr << "non numeric character" << endl; 
    errorCode = 4;  return;
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

/*checks for some errors in configuration files and configures machine components. configuration files are read through char-by-char to first make sure that only numeric and space/tab/NL characters are present. files will then be read int-by-int by component specific build methods to check for all other errors.*/
bool Enigma::build(int argc, char** argv) {

  /*check sufficient number of parameters*/
  if (argc<3) {
    errorDescription(INSUFFICIENT_NUMBER_OF_PARAMETERS);
    return false;
  }

  /*set number of rotors*/
  nb_rotors = argc - 4;

  /*call build on each component (includes error checks on each component)*/
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

int PieceOfHardware::setArray(ifstream &file, const char* fileName, int *array, int &offset) { 
  int num=0;
  offset=0;
  if (!validFilename(file, fileName))        //check for error opening config file
    return 1;
  for (file >> ws, file >> num;
       !file.eof() && offset<26; 
       file >> ws, file >> num, offset++) {
    if (!validEntry(file, fileName, num))    //check that entry is numeric and valid
      return 1;
    if (!setArrayEntry(array, offset, num))  //check no two identical entries
      return 2;
  }
  return 0;
}

bool PieceOfHardware::validFilename(ifstream &file, const char* fileName) {
  /*use 'stat' system call to check that arg is a regular file. on linux, if a directory is given as command line arg, for some reason an ifstream can open it and 'file >>' endlessly reads in a char with ascii value 10. on windows, a directory cannot be opened by ifstream.*/
  int fileStatus;
  struct stat fileInfo;

#ifndef _WIN32
  fileStatus = stat(fileName, &fileInfo);
  if (fileStatus!=0 || !S_ISREG(fileInfo.st_mode)) {
    machine->errorDescription(ERROR_OPENING_CONFIGURATION_FILE, fileName, file);     
    return false;
  }
#endif

  /*still need to check that the file can be opened.*/
  file.open(fileName);
  if (file.fail()) {
    machine->errorDescription(ERROR_OPENING_CONFIGURATION_FILE, fileName, file);
    return false;
  }
  return true;
}

/*this function used to iterate throughout intput file, as can be seen in commented region. it can't do that anymore, because setArray has to be separate for local build to send different error codes according to type, and setArray acts on one entry at a time. so iteration throughout input file will have to be done in body of local build, and validEntry will be called multiple times*/
bool PieceOfHardware::validEntry(ifstream &file, const char* configFilename, int& num) {
  /*check that entries are valid*/

  // for (file >> ws, file >> num;          HAVE THIS IN LOCAL BUILD !! read comments above for instructions
  //      !file.eof() && i<26; 
  //      file >> ws, file >> num, i++) {

  if (!num && file.fail() && !file.eof()) {
    machine->errorDescription(NON_NUMERIC_CHARACTER, configFilename, file);    
    return false;
  }

  if (num < 0 || num > 25) { 
    machine->errorDescription(INVALID_INDEX, configFilename, file);     
    return false;
  }
  return true;
}

/*this block used to send appropriate error code according to type as seen in commented region. now it only returns false. local build will know what error code to set. it used to set num to configArray, but it has towork for notches and rotpos too. so now array has to be specified as parameter when called in local builds*/
bool PieceOfHardware::setArrayEntry(int *array, int &offset, int const &num) {
  array[offset] = num;
  for (int j=offset-1; j>=0; j--) { //check no two identical entries
    if (array[offset]==array[j])
      return false;
  }
}
// {                           HAVE THIS IN LOCAL BUILD !!! read comments above for instructions
      // 	if (type==plu) {
      // 	  machine->errorDescription(IMPOSSIBLE_PLUGBOARD_CONFIGURATION, configFilename, file); 
      // 	  return false;
      // 	}
      // 	else if (type==ref) {
      // 	  machine->errorDescription(INVALID_REFLECTOR_MAPPING, configFilename, file); 
      // 	  return false;
      // 	}
      // 	else if (type==rot) {
      // 	  machine->errorDescription(INVALID_ROTOR_MAPPING, configFilename, file); 
      // 	  return false;
      // 	}
      // 	else {
      // 	  machine->errorDescription(REPEATED_ENTRIES_UNKNOWN_TYPE, configFilename, file); 
      // 	  return false;
      // 	} } }

  // if (type==plu && i%2!=0) {            //check for an even number of plugboard parameters.
  //   machine->errorDescription(INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS, configFilename, file);
  //   return false;
  // }

  // else if (i != 26) {           //check for 13 pairs of reflector parameters or valid rotor mapping.
  //   if (type==ref) {
  //     machine->errorDescription(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS, configFilename, file);
  //     return false;
  //   }
  //   if (type==rot) {
  //     machine->errorDescription(INVALID_ROTOR_MAPPING, configFilename, file);
  //     return false;
  //   }
  // }         
  // configArray[i] = sintinel;       
  // file.close();
  // return true;               HAVE IN LOCAL BUILD UNTIL HERE !!!


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
  ifstream file;
  int error, offset;
  error = setArray(file, configFilename, configArray, offset);

  if (error == 1)            //check whether encountered errors: error opening config file,
    return false;            //non numeric char, or invalid index. 

  if (error == 2) {          //check whether encountered two identical entries.
    machine->errorDescription(IMPOSSIBLE_PLUGBOARD_CONFIGURATION, configFilename, file);
    return false;
  }

  if (offset % 2 != 0) {     //check whether encountered odd number of plugboard parameters.
    machine->errorDescription(INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS, configFilename, file);
    return false;
  }

  configArray[offset] = sintinel;
  file.close();
  return true;
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
	machine->errorDescription(INVALID_INPUT_CHARACTER); 
	return false;
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
  machine->errorDescription(UNKNOWN_ERROR); //should not ever reach here.
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
  ifstream file;
  int error, offset;
  error = setArray(file, configFilename, configArray, offset);

  if (error == 1)            //check whether encountered errors: error opening config file,
    return false;            //non numeric char, or invalid index. 

  if (error == 2) {          //check whether encountered two identical entries.
    machine->errorDescription(INVALID_REFLECTOR_MAPPING, configFilename, file);
    return false;
  }
    
  if (offset != 26) {        //check whether encountered not 13 pairs of reflector parameters.
    machine->errorDescription(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS, configFilename, file);
    return false;
  }

  configArray[offset] = sintinel;
  file.close();
  return true;
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
  machine->errorDescription(UNKNOWN_ERROR);//all letters should have a mapping, so should
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

bool Rotor::build(const char* configFilename, const char* posFilename, int rotNumber) {
  ifstream file;
  int error, offset;

  /*set configArray*/
  error = setArray(file, configFilename, configArray, offset);
  if (error == 1)
    return false;
  if (error == 2 || offset != 26) {
    machine->errorDescription(INVALID_ROTOR_MAPPING, configFilename, file);
    return false;
  }
  configArray[offset] = sintinel;

  /*set notches*/
  for (offset=0, file >> ws, file >> notches[offset]; 
       !file.eof(); 
       offset++, file >> ws, file >> notches[offset]) {   
    if (!validEntry(file, configFilename, notches[offset]))
      return false;
  }
  notches[offset]=sintinel;
  file.close();

  /*set rotPos*/
  if (!validFilename(file, posFilename))
    return false;
  for (offset=0, file >> ws, file >> rotPos;
       offset < rotNumber && !file.eof();     //extra condition couldn't be a function because
       offset++, file >> ws, file >> rotPos); //we need semicolon at end of this for loop
  /*check that there is a starting position for the rotor*/
  if (file.eof()) {
    machine->errorDescription(NO_ROTOR_STARTING_POSITION, posFilename, file);
    return false;
  }
  /*check that character is numeric and valid*/
  if (!validEntry(file, posFilename, rotPos))
    return false;

  file.close();
  return true;
}

  // if ( !PieceOfHardware::build(configFilename, rot) )
  //   return false;
  // else if ( !setNotches(configFilename) )
  //   return false;
  // else if ( !setRotpos(posFilename, rotNumber) )
  //   return false;
  // else return true;
// bool Rotor::setNotches(const char* configFilename) {
//   int i=0;
//   ifstream file;
//   file.open(configFilename);

//   /*skip to where notch positions are supposed to be*/
//   for (int k=0; k!=26; k++) {
//     file >> ws;
//     file >> notches[i];
//   }
  
//   /*check that entries are valid*/
//   for (file >> ws, file >> notches[i]; 
//        !file.eof(); 
//        i++, file >> ws, file >> notches[i]) {   

//     if (!notches[i] && file.fail() && !file.eof()) {
//       machine->errorDescription(NON_NUMERIC_CHARACTER, configFilename, file);    
//       return false;
//     }

//     if (notches[i] < 0 || notches[i] > 25) { 
//       machine->errorDescription(INVALID_INDEX, configFilename, file);     
//       return false;
//     }
//   }

//   notches[i] = sintinel;
//   file.close();
//   return true;
// }

// bool Rotor::setRotpos(const char* posFilename, int rotNumber) {
//   int count=0;
//   ifstream file;

//   if (!fileIsOpenable(file, posFilename))
//     return false;

//   for (file >> ws, file >> rotPos;
//        count < rotNumber && !file.eof(); 
//        count++, file >> ws, file >> rotPos);

//   /*check that there is a starting position for the rotor*/
//   if (file.eof()) {
//     machine->errorDescription(NO_ROTOR_STARTING_POSITION, posFilename, file);
//     return false;
//   }

//   /*check that character is numeric and valid*/
//   if (!rotPos && file.fail() && !file.eof()) {
//     machine->errorDescription(NON_NUMERIC_CHARACTER, posFilename, file);    
//     return false;
//   }
//   if (rotPos < 0 || rotPos > 25) {
//     machine->errorDescription(INVALID_INDEX, posFilename, file);
//     return false;
//   }

//   file.close();
//   return true;
// }

bool Rotor::rotate() {
  rotPos = (rotPos + 1) % 26;
  for (int i=0; notches[i] != sintinel; i++) {
    if (rotPos == notches[i])
       return true;                       //if leftmost rotor does rotate, 'true' will be returned but
  }                                       //'i>0' condition in 'for' loop in main will fail anyway.
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
  machine->errorDescription(UNKNOWN_ERROR); //should not ever reach here.
  return -1;
}
/*END OF ROTOR DEFINITIONS*/
