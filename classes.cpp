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

  Skeleton() {
    message = {0,1,2,3,4,5,6};
    config_array = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
  }

  void scramble(); //if all methods are pure virtual, no point having this base class

  void invert();   //would be great if this could be defined once for all classes

  void sendTo(Skeleton obj) {
    obj.message = message;
  }

  ~Plugboard() {
    config_file.close();       //do I need to delete all fields, or only dynamic ones?
  }

};



class Plugboard : Skeleton {
private:
  string input;

public:

  Pluboard() : Skeleton() {
    input = "ABCDEFG";
  }

  void scramble() {
  }
};


class Reflector
{
private:

protected:

public:

  Reflector() : Skeleton() {
  }

  void scramble() {
  }

};



class Rotor : Skeleton
{
private:

protected:

public:

  Rotor() : Skeleton() {
  }

};


class Protor : Rotor    //front rotor which does IO with plugboard
{
private:

protected:

public:

  Protor() : Rotor() {
  }

};


class Brotor : Rotor   //back rotor which does IO with reflector
{
private:

protected:

public:

  Protor() : Rotor() {
  }

};
