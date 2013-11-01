#include<iostream>
#include<fstream>

using namespace std;

#define sintinel -15 //sentinel for integer array


class Skeleton {
protected:
  int *message;
  ifstream configuration;

public:
  virtual void scramble() = 0; //if all methods are pure virtual, no point having this base class
  virtual void invert() = 0;   //would be great if this could be defined once for all classes
  virtual void sendTo(Skeleton obj) {
    obj.message = message;
  }
};


class Plugboard : Skeleton {
private:
  string input;

  void strInt() {
    int i=0;
    for (; input[i]!='\0'; i++)
      message[i] = (int) input[i];
    message[i] = sintinel;
  }

public:
  void getInput() { //involve INVALID_INPUT_CHARACTER
  }
  void scramble() {
  }
};

class Rotor{
};

class Protor : Rotor {   //front rotor which does IO with plugboard
};

class Brotor : Rotor{   //back rotor which does IO with reflector
};

class Reflector {
};

