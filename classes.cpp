#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<iterator>
#include<sstream>
#include<vector>

using namespace std;

#include"enigma.h"


class Skeleton 
{
protected:
  int message[500];
  int config_array[100];

public:

  /*constructor 1*/
  Skeleton(const char *config_filename) {
    int temp;
    ifstream config_file;
    config_file.open(config_filename);        //begin is an istream iterator linked to config
                                              //also creating end, which by default has a kind of sentinel value
    istream_iterator<int> begin(config_file), end;

    for (temp=0; begin != end; ++begin, temp++) { //stop looping when value of begin is sentinel
      config_array[temp] = *begin;
    }

    config_array[temp] = sintinel;            //add my own sentinel value for integer arrays
  }

  /*constructor 2*/
  Skeleton() {
    for (int i =0; i<26; i++) {
      message[i] = i;
      config_array[i] = i;
    }

    message[26]=sintinel;
    config_array[26]=sintinel;
  }

  void scramble(); 

  void invert();

  void sendTo(Skeleton obj) {
    for (int i=0; message[i] != sintinel; i++)
      obj.message[i] = message[i];
  }

  void print() {
    cout << "message: ";
    for (int i=0; message[i]!=sintinel; cout << message[i] << " ", i++);
    cout << endl;

    cout << "config_array: ";
    for (int i=0; config_array[i]!=sintinel; cout << config_array[i] << " ", i++);
    cout << endl;
  }

};



class Plugboard : public Skeleton {
private:
  string input;

public:

  Plugboard() : Skeleton() {
    input = "ABCDEFG";
  }

  void getInput()
  {
    int i = 0;
    char input;

    do {
      cout << "Enter input (enter '.' to stop): ";
      cin >> input;


      if ((int)input<91 && (int)input>64) {
	if (input!='.' && input!=10 && input!=13 && input!=9 && input!=32) {
	  /*if input is neither a capital letter or a new line, carriage return, tab, space, then ERROR*/
	  error_description(-13);
	  exit(-13);
	}
	/*if input is a new line, carriage return, tab, space, then do nothing*/
      }

      else { /*if input is a capital letter, add it to message*/
	message[i] = (int) input - 65;
	i++;
      }

    } while (input != '.');

    message[i] = sintinel;
  }

  void scramble() {
  }

  // Skeleton::print();

};


class Reflector : public Skeleton
{
private:

protected:

public:

  Reflector() : Skeleton() {
  }

  void scramble() {
  }

  // Skeleton::print();

};



class Rotor : public Skeleton
{
private:

protected:

public:

  Rotor() : Skeleton() {
  }

  //Skeleton::print();

};


class Protor : public Rotor    //front rotor which does IO with plugboard
{
private:

protected:

public:

  Protor() : Rotor() {
  }

};


class Brotor : public Rotor   //back rotor which does IO with reflector
{
private:

protected:

public:

  Brotor() : Rotor() {
  }

};
