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
   int config_array_size;

public:

  /*constructor 1*/
  Skeleton(const char* config_filename) {

    /*check that index is valid*/
    int num, i=0;
    ifstream file;
    file.open(config_filename);

    for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
      num = *begin;
      if (num < 0 || num > 25) { 
	error(-10);
      }
      config_array[i] = num;
    }

    config_array_size = i;
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

  Plugboard(const char* config_filename)
  {
    int num, i=0;
    ifstream file;
    file.open(config_filename);

    for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
      num = *begin;

      /*check that index is valid*/
      if (num < 0 || num > 25) { 
	error(-10);
      }
      config_array[i] = num;

      /*check no two identical entries in config_array*/
      for (int j=i-1; j>=0; j--) {
	if (config_array[i]==config_array[j])
	  error(-14);
      }
    }

    /*check that we have an even number of plugboard parameters*/
    if (i % 2 != 00)
      error(-15);

    /*reach here iif plugboard config perfectly valid*/
    config_array[i] = sintinel;
    config_array_size = i;
    file.close();

    cout << "plugboard configuration valid" << endl;
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
	if (ch!='.' && ascii!=10 && ascii!=13 && ascii!=9 && ascii!=32)
	  error(-13);
      }                          //if new line, carriage return, tab or space, do nothing
      else {                     //add to message
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

  Reflector(const char* config_filename)
  {
    int num, i=0;
    ifstream file;
    file.open(config_filename);

    for (istream_iterator<int> begin(file), end; begin!=end; ++begin, i++) {
      num = *begin;

      /*check that index is valid*/
      if (num < 0 || num > 25) { 
	error(-10);
      }
      config_array[i] = num;

      /*check no two identical entries in config_array*/
      for (int j=i-1; j>=0; j--) {
	if (config_array[i]==config_array[j])
	  error(-18);
      }
    }

    /*check that we have 13 pairs of reflector parameter numbers*/
    if (i != 26)
      error(-19);

    /*reach here iif reflector config perfectly valid*/
    config_array[i] = sintinel;
    config_array_size = i;
    file.close();

    cout << "reflector configuration valid" << endl;
  }


  void scramble() {
  }

};



class Rotor : public Skeleton
{
private:
   int notches[50];
   int start_pos;
   int order;

protected:

public:

  Rotor(const char* config_filename, const char* pos_filename, int rotor_nb) 
  {
    int num, i=0, k=0;
    ifstream file;
    file.open(config_filename);
    istream_iterator<int> begin(file), end;

    /*set config_array*/
    for (; begin!=end && i<26; ++begin, i++) {
      num = *begin;

      /*check that index is valid*/
      if (num < 0 || num > 25)
	error(-10);

      config_array[i] = num;

      /*check no two identical entries in config_array*/
      for (int j=i-1; j>=0; j--) {
	if (config_array[i]==config_array[j])
	  error(-16);
      }
    }

    /*check that we have every input mapped*/
    /*reach here iif no identical config_array entries, so merely need to check size)*/
    if (i != 26)
      error(-16);

    /*set notches, starting from where we left off*/
    for (; begin!=end; ++begin, k++) {
      num = *begin;

      /*check that index is valid*/
      if (num < 0 || num > 25)
	error(-10);

      notches[k] = num;
    }

    /*reach here iif rotor config perfectly valid*/
    notches[k] = sintinel;
    config_array[i] = sintinel;
    config_array_size = i;
    file.close();

    cout << "rotor configuration valid" << endl;


    /*set start_pos*/
    file.open(pos_filename);
    istream_iterator<int> begin2(file), end2;
    for (int count=0; count < rotor_nb && begin2!=end2; ++begin2, count++);

    /*check that index is valid*/
    if (*begin < 0 || *begin > 25)
      error(-10);

    start_pos = *begin;
  }
};
