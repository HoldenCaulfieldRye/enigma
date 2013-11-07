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
      cerr << "letterIndex was: " << pb.showLetterIndex() << ", then plugboard scrambles with configuration:"; pb.showConfig_array();
      

      pb.scramble();
      cerr << "letterIndex is now: " << pb.showLetterIndex() << endl;


      /*rightmost rotor rotates*/
      cerr << "rotor starting positions were: "; 
      for (int i=0; i<nb_rotors; i++)
	cerr << rotor[i]->showRot_pos() << ", ";
      cerr << "then rightmost rotor rotates" << endl;

      for (int i=nb_rotors-1; i>0; rotor[i]->rotate(i), i--); 
      /*NB: if there is only one rotor in the setup, it can still rotate*/

      cerr << "rotor starting positions are now: "; 
      for (int i=0; i<nb_rotors; i++)
	cerr << rotor[i]->showRot_pos() << ", ";
      cerr << endl;


      /*plugboard sends letterIndex to rightmost rotor*/
      rotor[nb_rotors-1]->setLetterIndex(pb.showLetterIndex());


      /*each rotor with a left neighbour scrambles letterIndex & sends it to neighbour*/
      for (int i=nb_rotors-1; i>0; i--) {
	cerr << "letterIndex was: " << rotor[i]->showLetterIndex() << ", then rotor[" << i << "] scrambles with configuration:" << endl; rotor[i]->showConfig_array();

	  rotor[i]->scramble();
	  rotor[i-1]->setLetterIndex(rotor[i]->showLetterIndex());
	
	cerr << "letterIndex is now: " << rotor[i]->showLetterIndex() << endl;
      }    


      /*leftmost rotor scrambles letterIndex*/
      cerr << "letterIndex was: " << rotor[0]->showLetterIndex() << ", then rotor[0] scrambles with configuration:" << endl; rotor[0]->showConfig_array();

      rotor[0]->scramble();

      cerr << "letterIndex is now: " << rotor[0]->showLetterIndex() << endl;


      /*leftmost rotor sends letterIndex to reflector*/
      rf.setLetterIndex(rotor[0]->showLetterIndex());

      /*reflector scrambles letterIndex*/
      cerr << "letterIndex was: " << rf.showLetterIndex() << ", then reflector scrambles with configuration:" <<endl; rf.showConfig_array();

      rf.scramble();

      cerr << "letterIndex is now: " << rf.showLetterIndex() << endl;


      /*reflector sends letterIndex to leftmost rotor*/
      rotor[0]->setLetterIndex(rf.showLetterIndex());


      /*each rotor with a right neighbour inversely scrambles letterIndex & sends to neighbour*/
      for (int i=0; i<nb_rotors-1; i++) {
	cerr << "letterIndex was: " << rotor[i]->showLetterIndex() << ", then rotor[" << i << "] inverse scrambles with configuration:" << endl; rotor[i]->showConfig_array();

	  rotor[i]->inverseScramble();
	  rotor[i+1]->setLetterIndex(rotor[i]->showLetterIndex());
	
	cerr << "letterIndex is now: " << rotor[i]->showLetterIndex() << endl;
      } 


      /*rightmost rotor inversely scrambles letterIndex*/
      cerr << "letterIndex was: " << rotor[nb_rotors-1]->showLetterIndex() << ", then rightmost rotor inverse scrambles with configuration:" << endl; rotor[nb_rotors-1]->showConfig_array();

      rotor[nb_rotors-1]->inverseScramble();

      cerr << "letterIndex is now: " << rotor[nb_rotors-1]->showLetterIndex() << endl;


      /*rightmost rotor sends letterIndex to plugboard*/
      pb.setLetterIndex(rotor[nb_rotors-1]->showLetterIndex());

      /*plugboard inversely scrambles letterIndex*/ 
      cerr << "letterIndex was: " << pb.showLetterIndex() << ", then plugboard inversely scrambles with configuration:" << endl; pb.showConfig_array();

      pb.inverseScramble();

      cerr << "letterIndex is now: " << pb.showLetterIndex() << endl;


      /*plugboard outputs letter corresponding to letterIndex*/
      char ch = pb.showLetterIndex() + 65;
      cout << ch ;
    }


  delete [] rotor;
  return 0;
}
