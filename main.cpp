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
  Enigma enigma;
  enigma.build(argc, argv);
  enigma.encrypt();
  cerr << "back to main" << endl;
  return 0;
}

/*ask Reuben whether should specify destructors, whether using exit() is ok, and whether it's worth improving the code by getting rid of ifs for rotors*/
