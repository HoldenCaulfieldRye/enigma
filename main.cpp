#include<iostream>
#include<string>
#include<cstring>
#include<vector>

using namespace std;

#include"enigma.h"
#include"classes.cpp"

int main(int argc, char **argv) 
{
  cerr << "argc = " << argc;

  check(argc, argv);

  /*initialisation: declare objects and assign them to config files specified on command line.
    reach here iif there are no errors.*/
  const int nb_rotors = argc - 4;
  Plugboard pb(argv[1]);
  Reflector rf(argv[2]);
  Rotor **rotor;
  rotor = new Rotor *[nb_rotors];
  for (int i=0; i<nb_rotors; i++)
    rotor[i] = new Rotor(argv[i+3], argv[argc-1], i);  //starting positions are set

  /*while input file has valid letters to give, the process loops*/
  while (pb.get_letter_from_input_file()) 
    {                         //plugboard has now got a letterIndex


      /*plugboard scrambles letterIndex*/ 
      cerr << "letterIndex was: " << pb.showLetterIndex() << endl;

      cerr << "letterIndex is now: " << pb.showLetterIndex() << endl;


      /*rightmost rotor rotates*/

      /*plugboard sends letterIndex to rightmost rotor*/
      rotor[nb_rotors-1]->setLetterIndex(pb.showLetterIndex());

      /*each rotor with a left neighbour scrambles letterIndex & sends it to neighbour*/
      for (int i=nb_rotors-1; i>0; i--) {
	cerr << "letterIndex was: " << rotor[i]->showLetterIndex() << endl;

	cerr << "letterIndex is now: " << rotor[i]->showLetterIndex() << endl;
      }


      /*leftmost rotor scrambles letterIndex*/
      cerr << "letterIndex was: " << rotor[0]->showLetterIndex() << endl;

      cerr << "letterIndex is now: " << rotor[0]->showLetterIndex() << endl;


      /*leftmost rotor sends letterIndex to reflector*/
      rf.setLetterIndex(rotor[0]->showLetterIndex());

      /*reflector scrambles letterIndex*/
      cerr << "letterIndex was: " << rf.showLetterIndex() << endl;

      cerr << "letterIndex is now: " << rf.showLetterIndex() << endl;


      /*reflector sends letterIndex to leftmost rotor*/
      rotor[0]->setLetterIndex(rf.showLetterIndex());

      /*each rotor with a right neighbour inversely scrambles letterIndex & sends to neighbour*/
      for (int i=0; i<nb_rotors-1; i++) {
	cerr << "letterIndex was: " << rotor[i]->showLetterIndex() << endl;

	cerr << "letterIndex is now: " << rotor[i]->showLetterIndex() << endl;
      }


      /*rightmost rotor inversely scrambles letterIndex*/
      cerr << "letterIndex was: " << rotor[nb_rotors-1]->showLetterIndex() << endl;

      cerr << "letterIndex is now: " << rotor[nb_rotors-1]->showLetterIndex() << endl;


      /*rightmost rotor sends letterIndex to plugboard*/
      pb.setLetterIndex(rotor[nb_rotors-1]->showLetterIndex());

      /*plugboard inversely scrambles letterIndex*/ 
      cerr << "letterIndex was: " << pb.showLetterIndex() << endl;

      cerr << "letterIndex is now: " << pb.showLetterIndex() << endl;


      /*plugboard outputs letter corresponding to letterIndex*/

    }


  delete [] rotor;
  return 0;
}
