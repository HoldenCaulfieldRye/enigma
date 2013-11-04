#include<iostream>
using namespace std;

<<<<<<< HEAD
#include"enigma.h"

int main(int argc, char argv**) 
{
  if (argc<3)
    error_description(-12); //insufficient number of parameters
  exit(-12);

  //string lastArg = argv[argc-1];
  if (argv[argc-1].compare(argv[argc-1].length-4, 4, ".pos") != 0 && argc>3) //lastArg.compare(lastArg.length - 4, 4, ".pos")
    error_description(-17); //no rotor starting position
  exit(-17);

  //check for whether a .pos file is the final argument using str.compare()

  cout << "I hope you entered config files in pb, ref, (rot) order!" << endl << endl;

  char continu = 'y';
  const int nb_rotors = argc - 3;

  /*initialisation: declare objects and assign them to config files specified on command line*/
  Plugboard pb(argv[1]);
  Reflector rf(argv[2]);
  Rotor *rotor;
  for (int i=3; i<nb_rotors; i++) {
    rotor = new Rotor(argv[i-3]);
  }
  rotor[nb_rotors] = NULL;

  /*compliance: check that configuration files are valid*/
  pb.check();
  rf.check();
  for (int i=0; rotor[i] != NULL; i++)
    rotor[i].check();   


  while(continu=='y') {

    pb.getInput();






    cout << "again? ";
    cin >> continu;
  }
  delete [] rotor;
=======
#include"enigma.cpp"

int main() {
  char continu = 'y';
  Plugboard pb;

  while(continu=='y') {
    pb.getInput();
    cout << "again? ";
    cin >> continu;
  }
>>>>>>> 7e8d8930df69764be9b0215f01aab61f57846696
  return 0;
}
