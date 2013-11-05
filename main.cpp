#include<iostream>
#include<string>
#include<cstring>
#include<vector>

using namespace std;

#include"enigma.h"
#include"classes.cpp"

int main(int argc, char **argv) 
{
  cout << "argc = " << argc;

  check(argc, argv);

  /*initialisation: declare objects and assign them to config files specified on command line.
    reach here iif there are no errors.*/
  const int nb_rotors = argc - 3;
  Plugboard pb(argv[1]);
  Reflector rf(argv[2]);
  vector<Rotor> rotor(1, Rotor(argv[3], argv[argc-1], 0));
  for (int i=1; i<nb_rotors; i++)
    rotor.push_back(Rotor(argv[i+3], argv[argc-1], i));

  /*plugboard gets input from file, checking char-by-char it is valid*/
  pb.getInput("input.txt");
  cout << "plugboard: "; pb.print(); cout << endl;
  // cout << "reflector: "; rf.print(); cout << endl;

  // for (int i=0; i<nb_rotors; i++) {
  //   cout << "rotor[" << i << "]: "; rotor[i].print(); cout << endl; }

  // delete [][] rotor;
  return 0;
}
