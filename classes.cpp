#include<iostream>
#include<fstream>
#include<cstring>
#include<iterator>

using namespace std;

#include"imperative.cpp"


class Skeleton 
{
protected:
  int *message;
  int *config_array;

public:

  Skeleton(const char *config_filename) {
    vector<int> temp;
    ifstream config_file;
    config_file.open(config_filename);
    istream_iterator<int> begin(config_file), end;
    copy(begin, end, back_inserter(temp));
    config_array = &temp[0];
  }

  virtual void scramble() = 0; //if all methods are pure virtual, no point having this base class

  virtual void invert() = 0;   //would be great if this could be defined once for all classes

  virtual void sendTo(Skeleton obj) {
    obj.message = message;
  }

  ~Plugboard() {
    config_file.close();       //do I need to delete all fields, or only dynamic ones?
  }

};



class Plugboard : Skeleton 
{
private:

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

  int check() {

  }

  int getInput() { //involve INVALID_INPUT_CHARACTER    
    int i = 0;
    char input;
    cout << "Enter input (enter "." to stop): ";
    cin >> input;
    while (input != '.') {
      if (input < 91 && input > 64)   //if input is a capital letter
	message[i] = (int) input - 65;
      else 
	strInt();
    }
  }

  void scramble() {
  }



};

class Rotor
{

};

class Protor : Rotor    //front rotor which does IO with plugboard
{

};

class Brotor : Rotor   //back rotor which does IO with reflector
{

};

class Reflector
{

};




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

