#include<iostream>
#include<string>
#include<cstring>
using namespace std;

#include"enigma.h"
#include"classes.cpp"
#include"imperative.cpp"

int main(int argc, char argv**) 
{
  check(argc, argv);

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


int main() {
  char continu = 'y';
  Plugboard pb;

  while(continu=='y') {
    pb.getInput();
    cout << "again? ";
    cin >> continu;
  }
  return 0;
}
