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
  cerr << "hello" << endl;

  Enigma enigma;

  cerr << "enigma initialised" << endl;
  cerr << "start building" << endl;

  if(!enigma.build(argc, argv))
    return enigma.getErrorCode();

  cerr << "start encrypting" << endl;

  if (!enigma.encrypt())
    return enigma.getErrorCode();

  cerr << "back to main" << endl;
  return enigma.getErrorCode();
}

/*ask Reuben whether should specify destructors, whether using exit() is ok, and whether it's worth improving the code by getting rid of ifs for rotors*/
