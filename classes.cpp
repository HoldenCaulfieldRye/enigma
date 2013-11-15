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


/*ENIGMA DEFINITIONS*/

/*minimalist constructor, declares machine components*/
Enigma::Enigma() {
  nb_rotors = 0;
}

/*function for errors*/
void Enigma::error(int code) {
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

/* checks for errors in configuration files and configures machine components*/
void Enigma::build(int argc, char** argv) {

  /*check sufficient number of parameters*/
  if (argc<3) {
    error(-12);
  }

  /*check that all characters are numeric, new line, carriage return, tab or space*/
  for (int i=1; i<argc && i<10; i++) {
    char ch;
    ifstream file;
    file.open(argv[i]);

    if (file.fail()) {
      cerr << "could not open " << argv[i] << " (perhaps filename is misspecified?)" << endl;
      error(-1);
    }

    for (file.get(ch); !file.eof(); file.get(ch)) {
      if ((int) ch < 48 || (int) ch > 57) {
	if (ch != 9 && ch != 10 && ch != 13 && ch != 32) {
	  cerr << "in file " << i << ": ";
	  error(-11);
	}
      }
    }
    file.close();
  }

  /*reach here iif there are no errors. configuration can begin*/
  cerr << "initialising" << endl;

  nb_rotors = argc - 4;
  pb.build(argv[1]);
  rf.build(argv[2]);
  if (nb_rotors>0) {
    Rotor *rotor;
    rotor = new Rotor [nb_rotors];
    for (int i=0; i<nb_rotors; i++) {
      rotor[i].build(argv[i+3], argv[argc-1], i);  //starting positions are set.
    rotor[i].showConfig();
    }  
  }
}

void Enigma::encrypt() {
  cerr << "beginning encryption" << endl;

  char outputLetter;

  /*encryption: while input file has valid letters to give, the process loops*/
  int x = 0;


  while (pb.getLetterFromInputFile()) 
    {

      cerr << x << ", "; x++;
      cerr << nb_rotors << endl;

      if (nb_rotors>0) {
	/*rightmost rotor rotates*/
	for (int i=nb_rotors-1; 
             i>=0 && rotor[i]->rotate(); 
             i--);

	cerr << x << ", "; x++;

	/*plugboard sends letterIndex to rightmost rotor*/
	rotor[nb_rotors-1]->setLetterIndex(pb.scramble());

      cerr << x << ", "; x++;

	/*each rotor with a left neighbour scrambles letterIndex & sends it to neighbour*/
	for (int i=nb_rotors-1; i>0; i--) {

      cerr << x << ", "; x++;

          // rotor[i]->scramble();
          rotor[i-1]->setLetterIndex(rotor[i]->scramble());
	}    

      cerr << x << ", "; x++;

	/*leftmost rotor sends letterIndex to reflector*/
	rf.setLetterIndex(rotor[0]->scramble());

      cerr << x << ", "; x++;
      }
      /*if no rotors, plugboard leads straight to reflector*/
      else rf.setLetterIndex(pb.scramble());

      cerr << x << ", "; x++;

      if (nb_rotors>0) {
	/*reflector sends letterIndex to leftmost rotor*/
	rotor[0]->setLetterIndex(rf.scramble());

      cerr << x << ", "; x++;

	/*each rotor with a right neighbour inversely scrambles letterIndex & sends to neighbour*/
	for (int i=0; i<nb_rotors-1; i++) {

          rotor[i+1]->setLetterIndex(rotor[i]->inverseScramble());
	} 

	/*rightmost rotor sends letterIndex to plugboard*/
	pb.setLetterIndex(rotor[nb_rotors-1]->inverseScramble());

      cerr << x << ", "; x++;

      }
      /*if no rotors, reflector leads straight to plugboard*/
      else pb.setLetterIndex(rf.scramble());

      cerr << x << ", "; x++;


      /*plugboard inversely scrambles letterIndex*/
      pb.inverseScramble();

      cerr << x << ", "; x++;

      /*plugboard outputs letter corresponding to letterIndex*/
      outputLetter = pb.getLetterIndex() + 65;

      cerr << "outputting " << outputLetter << endl;

      cout << outputLetter;
    }

  cerr << "finished encryption" << endl;
}

Enigma::~Enigma() {
  for (int i=nb_rotors-1; i>=0; i--) {
    delete [] rotor;
    rotor = NULL;
  }
  if (nb_rotors>0)
    cerr << "deleted Rotor pointer" << endl;
}
/*END OF ENIGMA DEFINITIONS*/


/*PIECEOFHARDWARE DEFINITIONS*/
PieceOfHardware::PieceOfHardware() {
  letterIndex = 0;               //holds index value of letter to encrypt.
  for (int i=0; i<27; i++)
    configArray[i] = 0;
  configArraySize = 0;
}

void PieceOfHardware::showConfig() {
  for (int i=0; configArray[i]!=sintinel; i++)
    cerr << configArray[i] << ", ";
  cerr << endl;
}

void PieceOfHardware::build(const char* configFilename, int type)
{
  int num, i=0;
  ifstream file;
  file.open(configFilename);

  /*set configArray*/
  for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25) { 
      Enigma::error(-10);
    }
    configArray[i] = num;

    /*check no two identical entries in configArray*/
    for (int j=i-1; j>=0; j--) {
      if (configArray[i]==configArray[j]) {
	if (type==plugboard)
	  Enigma::error(-14);
	if (type==reflector)
	  Enigma::error(-18);
      }
    }
  }

  if (type==plugboard) {         //check that we have an even number of plugboard parameters.
    if (i % 2 != 00) 
      Enigma::error(-15);
  }
  else if(type==reflector) {     //check that we have 13 pairs of reflector parameter numbers.
    if (i != 26)
      Enigma::error(-19);
  }

  configArray[i] = sintinel;     //reach here iif config perfectly valid.
  configArraySize = i;
  file.close();
}

int PieceOfHardware::getLetterIndex() const {
  return letterIndex;
}

/* storing the index value of the letter rather than the letter itself is great because it 
   keeps this function very simple */
void PieceOfHardware::setLetterIndex(int value) {
  letterIndex = value;
}
/*END OF PIECEOFHARDWARE DEFINITIONS*/


/*PLUGBOARD DEFINITIONS*/
Plugboard::Plugboard() : PieceOfHardware::PieceOfHardware() {
}

void Plugboard::build(const char* configFilename) {
  PieceOfHardware::build(configFilename, plugboard);
}

/*sets letter: reads in one character from inFile, performs checks, ignores if space etc
  if valid, assigns index value to letter and returns true; if fail (inc eof) returns false*/
bool Plugboard::getLetterFromInputFile()
{
  int ascii;
  char input;

  for (cin >> ws, cin >> input; 
       !cin.eof(); 
       cin >> ws, cin >> input) {

    /*check that input is a capital letter, new line, carriage return, tab or space*/
    ascii = (int) input;
    if (ascii>91 || ascii<64) {
      if (ascii!=10 && ascii!=13 && ascii!=9 && ascii!=32)
	Enigma::error(-13);
                                        //else letter is new line, carriage return, tab or space
    }                                   //so do nothing.       
    else {
      letterIndex = ascii - 65;
      return true;
    }
  }
  return false;                         //reach here iif end of file has been reached.
}

int Plugboard::scramble(int number) {
  assert(number>=0 && number<26);
  for (int i=0; i<configArraySize; i++) {
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

int Plugboard::inverseScramble() {      //for any valid config file, scramble() is a
                                        //bijection on {0,.., 25}.
  for (int i=0; i<26; i++) {            //so inverse function of scramble() exists; this is it.
    if (scramble(i) == letterIndex) {
      return i;
    }
  }
  Enigma::error(-1);                    //should not ever reach here.
  return -1;
}
/*END OF PLUGBOARD DEFINITIONS*/


/*REFLECTOR DEFINITIONS*/
Reflector::Reflector() : PieceOfHardware::PieceOfHardware() {
}

void Reflector::build(const char* configFilename) {
  PieceOfHardware::build(configFilename, reflector);
}

int Reflector::scramble() {
  for (int i=0; i<configArraySize; i++) {
    if (configArray[i]==letterIndex) {
      if (i % 2 == 0) {                 //i is even, so letter(configArray[i]) and
	return configArray[i+1];        //letter(configArray[i+1]) are connected. 
      }                              
      return configArray[i-1];          //i is odd, so letter(configArray[i]) and
    }                                   //letter(configArray[i-1]) are connected.
  }
  Enigma::error(-1);                    //all letters should have a mapping, so should
  return -1;                            //not ever reach here.
}
/*END OF REFLECTOR DEFINITIONS*/


/*ROTOR DEFINITIONS*/
Rotor::Rotor() : PieceOfHardware() {
  for (int i=0; i<50; i++)  
    notches[i] = 0;
  rotPos = 0;
}

void Rotor::build(const char* configFilename, const char* posFilename, int rotor_nb)
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
      Enigma::error(-10);

    configArray[i] = num;

    /*check no two identical entries in configArray*/
    for (int j=i-1; j>=0; j--) {
      if (configArray[i]==configArray[j])
	Enigma::error(-16);
    }
  }

  /*check that we have every input mapped*/
  /*reach here iif no identical configArray entries, so merely need to check size)*/
  if (i != 26)
    Enigma::error(-16);

  /*set notches, starting from where we left off*/
  for (; begin!=end; ++begin, k++) {
    num = *begin;

    /*check that index is valid*/
    if (num < 0 || num > 25)
      Enigma::error(-10);

    notches[k] = num;
  }

  /*reach here iif rotor config perfectly valid*/
  configArray[i] = sintinel;
  configArraySize = i;
  file.close();
  notches[k] = sintinel;

  /*set rotPos*/
  int count;
  file.open(posFilename);
  istream_iterator<int> begin2(file), end2;
  for (count=0; count < rotor_nb && begin2!=end2; ++begin2, count++);

  /*check that there is a starting position for the rotor*/
  if (begin2==end2)
    Enigma::error(-17);

  /*check that index is valid*/
  if (*begin2 < 0 || *begin2 > 25)
    Enigma::error(-10);
  rotPos = *begin2;
}

int Rotor::getRotPos() const {
  return rotPos;
}

bool Rotor::rotate() {
  rotPos = (rotPos + 1) % 26;
  for (int i=0; notches[i] != sintinel; i++) {
    if (rotPos == notches[i]) {
      cerr << "notch met!" << endl;
      return true;               //if leftmost rotor does rotate, 'true' will be returned but
    }                            //'i>0' condition in 'for' loop in main will fail anyway.
  }    
  return false;
}

int Rotor::scramble(int number)  {
  assert(number>=0 && number<26);
  return (configArray[(number + rotPos) % 26] + 26 - rotPos) % 26;
}

int Rotor::scramble() {
  return scramble(letterIndex);
}

int Rotor::inverseScramble() {
  for (int i=0; i<26; i++) { 
    if (scramble(i) == letterIndex) {
      return i;
    }
  }
  Enigma::error(-1);            //should not ever reach here.
  return -1;
}
/*END OF ROTOR DEFINITIONS*/

/*make sure that you're picking up errors correctly with the error config files you created!*/
