#include<iostream>
#include<fstream>
#include<cstring>
#include<iterator>

using namespace std;


const char *error_description(int code);

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


const char *error_description(int code) {
  switch(code) {
  case -10: 
    return "invalid index (file contains a number that is not between 0 and 25)";
  case -11:
    return "non numeric character (in file)";
  case -12:
    return "insufficient number of parameters (given in command line)";
  case -13:
    return "invalid input character (a non capital letter was input)";
  case -14:
    return "impossible plugboard configuration (file attempts to connect a contact with itself or with multiple contacts)";
  case -15:
    return "incorrect number of plugboard parameters (should be an even number of them)";
  case -16:
    return "invalid rotor mapping (an input is not mapped or an input has multiple mappings";
  case -17:
    return "no rotor starting position (insufficient number of starting positions)";
  case -18:
    return "invalid reflector mapping (file attempts to connect a contact with itself or with multiple contacts)";
  case -19:
    return "incorrect number of reflector parameters (file does not contain exactly 13 pairs of numbers)";
  }
 default:
   return "Unknown error";
}
