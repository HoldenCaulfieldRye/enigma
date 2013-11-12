#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>

using namespace std;

#include"enigma.h"

int main(int argc, char **argv)
{
  /*check sufficient number of parameters*/
  if (argc<3) {
    cerr << "insufficient number of parameters (given in command line)" << endl;
    exit(-12);
  }

  /*if rotors, check that there is a rotor starting positions file*/
  string lastArg = argv[argc-1];
  if (lastArg.compare(lastArg.size() - 4, 4, ".pos") != 0 && argc>3) {
      cerr << "no rotor starting position (insufficient number of starting positions)" << endl;
      exit(-17);
  }

  /*check that all characters are numeric, new line, carriage return, tab or space*/
  for (int i=1; i<argc && i<10; i++) {
    char ch;
    ifstream file;
    file.open(argv[i]);

    for (file.get(ch); !file.eof(); file.get(ch)) {
      if ((int) ch < 48 || (int) ch > 57) {
	if (ch != 9 && ch != 10 && ch != 13 && ch != 32) {
	  cerr << "in file " << i << ": ";	 
	  cerr << "non numeric character (in file)" << endl;
	  exit(-11);
	}
      }
      file.close();
    }
  }

  /*initialisation: declare objects and assign them to config files specified on command line.
    reach here iif there are no errors*/
  char outputLetter;
  const int nb_rotors = argc - 4;
  Plugboard pb(argv[1]);
  Reflector rf(argv[2]);
  Rotor **rotor;
  if (nb_rotors>0) {
    rotor = new Rotor *[nb_rotors];
    for (int i=0; i<nb_rotors; i++)
      rotor[i] = new Rotor(argv[i+3], argv[argc-1], i);  //starting positions are set
  }
  else rotor=NULL;

  /*encryption: while input file has valid letters to give, the process loops*/
  while (pb.getLetterFromInputFile()) 
    {
      /*plugboard scrambles letterIndex*/ 
      pb.scramble();

      if (nb_rotors>0) {
	/*rightmost rotor rotates*/
	for (int i=nb_rotors-1; i>=0 && rotor[i]->rotate(); i--);

	/*plugboard sends letterIndex to rightmost rotor*/
	rotor[nb_rotors-1]->setLetterIndex(pb.showLetterIndex());

	/*each rotor with a left neighbour scrambles letterIndex & sends it to neighbour*/
	for (int i=nb_rotors-1; i>0; i--) {
          rotor[i]->scramble();
          rotor[i-1]->setLetterIndex(rotor[i]->showLetterIndex());
	}    

	/*leftmost rotor scrambles letterIndex*/
	rotor[0]->scramble();

	/*leftmost rotor sends letterIndex to reflector*/
	rf.setLetterIndex(rotor[0]->showLetterIndex());
      }
      /*if no rotors, plugboard leads straight to reflector*/
      else rf.setLetterIndex(pb.showLetterIndex());


      /*reflector scrambles letterIndex*/
      rf.scramble();

      if (nb_rotors>0) {
	/*reflector sends letterIndex to leftmost rotor*/
	rotor[0]->setLetterIndex(rf.showLetterIndex());

	/*each rotor with a right neighbour inversely scrambles letterIndex & sends to neighbour*/
	for (int i=0; i<nb_rotors-1; i++) {
          rotor[i]->inverseScramble();
          rotor[i+1]->setLetterIndex(rotor[i]->showLetterIndex());
	} 

	/*rightmost rotor inversely scrambles letterIndex*/
	rotor[nb_rotors-1]->inverseScramble();

	/*rightmost rotor sends letterIndex to plugboard*/
	pb.setLetterIndex(rotor[nb_rotors-1]->showLetterIndex());
      }
      /*if no rotors, reflector leads straight to plugboard*/
      else pb.setLetterIndex(rf.showLetterIndex());


      /*plugboard inversely scrambles letterIndex*/
      pb.inverseScramble();

      /*plugboard outputs letter corresponding to letterIndex*/
      outputLetter = pb.showLetterIndex() + 65;
      cout << outputLetter ;
    }

  delete [] rotor;
  return 0;
}

/*ask Reuben whether should specify destructors, whether using exit() is ok, and whether it's worth improving the code by getting rid of ifs for rotors*/
