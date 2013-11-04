#include<iostream>
using namespace std;

#include"enigma.h"

int main(int argc, char argv**) 
{
  if (argc<3)
    error_description(-12);
  exit(-12);

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
  return 0;
}
