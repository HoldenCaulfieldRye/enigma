#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<iterator>
#include<sstream>
#include<string>

using namespace std;

#include"enigma.h"


class Skeleton 
{
protected:
  int message[500];
  int config_array[100];

public:

  /*constructor 1*/
  Skeleton(const char* config_filename) {

    /*check that index is valid*/
    int num, i=0;
    ifstream file;
    file.open(config_filename);

    cout << config_filename << " opened again to check index" << endl;

    for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
      num = *begin;
      if (num < 0 || num > 25) {
	cout << "in file " << i << ": ";	 
	error_description(-10);
      }
      config_array[i] = num;
    }
    config_array[i] = sintinel;
    file.close();

    cout << "\nfile has valid index" << endl;
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

  Plugboard(const char* config_filename) : Skeleton(config_filename) {
  }

  Plugboard() : Skeleton() {
    input = "ABCDEFG";
  }

  void getInput(const char* filename)
  {
    cout << "getting input" << endl;
    int i = 0;
    char ch;
    int ascii;
    ifstream input;

    /*check that input is a capital letter, new line, carriage return, tab or space*/
    input.open(filename);
    input >> ws;
    while (!input.fail()) {
      input.get(ch);
      ascii = (int) ch;
      if (ascii>91 || ascii<64) {
	if (ch!='.' && ascii!=10 && ascii!=13 && ascii!=9 && ascii!=32) {
	  error_description(-13);
	  exit(-13);
	}                          //if new line, carriage return, tab or space, do nothing
      }
      else {                       //add to message
	message[i] = ascii - 65;
	i++;
      }
    }
    message[i] = sintinel;
  }

  void scramble() {
  }

};


class Reflector : public Skeleton
{
private:

protected:

public:

  Reflector(const char* config_filename) : Skeleton(config_filename) {
  }

  Reflector() : Skeleton() {
  }

  void scramble() {
  }

};



class Rotor : public Skeleton
{
private:

protected:

public:

  Rotor(const char* config_filename) : Skeleton(config_filename) {
  }

  Rotor() : Skeleton() {
  }

};


class Protor : public Rotor    //front rotor which does IO with plugboard
{
private:

protected:

public:

  Protor(const char* config_filename) : Rotor(config_filename) {
  }

  Protor() : Rotor() {
  }

};


class Brotor : public Rotor   //back rotor which does IO with reflector
{
private:

protected:

public:

  Brotor(const char* config_filename) : Rotor(config_filename) {
  }

  Brotor() : Rotor() {
  }

};
