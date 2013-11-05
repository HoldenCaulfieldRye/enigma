#include<iostream>
#include<string>
#include<cstring>

using namespace std;

#include"enigma.h"
#include"classes.cpp"

int main(int argc, char **argv) 
{
  check(argc, argv);

  char continu = 'y';
  const int nb_rotors = argc - 3;
  cout << "there are " << nb_rotors << " rotors" << endl;

  /*initialisation: declare objects and assign them to config files specified on command line.
    reach here iif there are no errors.*/
  Plugboard pb(argv[1]);

  Reflector rf(argv[2]); //Reflector rf(argv[2])
  Rotor *rotor = new Rotor;

  for (int i=0; i<nb_rotors; i++)
    rotor[i](argv[i+2]);
  cout << "plugboard: "; pb.print(); cout << endl;
  cout << "reflector: "; rf.print(); cout << endl;

  for (int i=0; i<nb_rotors; i++) {
    cout << "rotor[" << i << "]: "; rotor[i].print(); cout << endl; }

  while(continu=='y') {

    /*get input from command line, checking char by char that it is correct*/
    pb.getInput();


    cout << "again? ";
    cin >> continu;
  }
  delete [] rotor;
  return 0;
}
