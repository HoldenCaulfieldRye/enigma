#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<iterator>
#include<sstream>
#include<string>
#include<cassert>

using namespace std;

#include"enigma.h"


class PieceOfHardware 
{
protected:
  int letterIndex;  //holds index value of a letter to encrypt, using 0-based alphabet index
  int config_array[100];
  int config_array_size;

public:

  void print() { //DELETE THIS WHEN PROGRAM FINISHED
    cerr << "letterIndex: " << letterIndex << endl;

    cerr << "config_array: ";
    for (int i=0; config_array[i]!=sintinel; cerr << config_array[i] << " ", i++);
    cerr << endl;
  }

  int showLetterIndex() {
    return letterIndex;
  }

  void showConfig_array() {
    for (int i=0; config_array[i]!=sintinel; i++)
      cerr << config_array[i] << ", ";
    cerr << endl;
  }

  /* because we store index of letter rather than the char letter itself, and */
  /* because any two connected PieceOfHardware objects have a fixed "absolute position 0... */
  /* ...to absolute position 0, absolute postion 1 to absolute position 1, etc" mapping, */
  /* this function is very straightforward*/
  void setLetterIndex(int value) {
    letterIndex = value;
  }
};



class Plugboard : public PieceOfHardware {
private:

public:

  Plugboard(const char* config_filename)
  {
    int num, i=0;
    ifstream file;
    file.open(config_filename);

    /*set config_array*/
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

    cerr << "plugboard configuration valid" << endl;
  }


  /*set letter: read in one character from inFile, perform checks, ignore if space etc*/
  /*if valid, assign index value to letter and return true; if fail (inc eof) return false*/
  bool get_letter_from_input_file()
  {
    int ascii;
    char input;

    for (cin >> ws, cin >> input; !cin.eof(); cin >> ws, cin >> input) {

      /*check that input is a capital letter, new line, carriage return, tab or space*/
      ascii = (int) input;
      if (ascii>91 || ascii<64) {
	if (ascii!=10 && ascii!=13 && ascii!=9 && ascii!=32)
	  error(-13, input);
	//else letter is new line, carriage return, tab or space, so do nothing
      }                         
      else {
	letterIndex = ascii - 65;
	cerr << "getting input" << endl;
	return true;
      }
    }
    return false;        //reach here iif end of file has been reached
  }

  void scramble() {
    for (int i=0; i<config_array_size; i++) {
      if (config_array[i]==letterIndex) {
	if (i % 2 == 0) {
	  /*i is even, so letter(config_array[i]) and letter(config_array[i+1]) are connected*/
	  letterIndex = config_array[i+1]; 
	  return;
	}
	/*i is odd, so letter(config_array[i]) and letter(config_array[i-1]) are connected*/
	letterIndex = config_array[i-1];
	return;
      }
    }
    return; //reach here iif letter(letterIndex) is not scrambled by plugboard 
  }


  int scramble(int number) {
    assert(number>=0 && number<26);
    for (int i=0; i<config_array_size; i++) {
      if (config_array[i]==number) {
	if (i % 2 == 0) {
	  return config_array[i+1];
	}
	return config_array[i-1];
      }
    }
    return number; //letter(number) is not scrambled by plugboard, so letterIndex stays the same
  }


  void inverseScramble() {     //for any valid config file, scramble() is a bijection on {0,.., 25}
    for (int i=0; i<26; i++) { //this inverse function of scramble() exists, and this is it
      if (scramble(i) == letterIndex) {
	letterIndex = i;
	return;
      }
    }
    error(-1);                 //should not ever reach here
  }

};


class Reflector : public PieceOfHardware
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

    cerr << "reflector configuration valid" << endl;
  }


  void scramble() {
    for (int i=0; i<config_array_size; i++) {
      if (config_array[i]==letterIndex) {
	if (i % 2 == 0) {
	  /*i is even, so letter(config_array[i]) and letter(config_array[i+1]) are connected*/
	  letterIndex = config_array[i+1]; 
	  return;
	}
	/*i is odd, so letter(config_array[i]) and letter(config_array[i-1]) are connected*/
	letterIndex = config_array[i-1];
	return;
      }
    }
    error(-1); //all letters should have a mapping, so should not ever reach here
  }
};



class Rotor : public PieceOfHardware
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

    cerr << "rotor configuration valid" << endl;


    /*set start_pos*/
    file.open(pos_filename);
    istream_iterator<int> begin2(file), end2;
    for (int count=0; count < rotor_nb && begin2!=end2; ++begin2, count++);

    /*check that index is valid*/
    if (*begin2 < 0 || *begin2 > 25)
      error(-10);

    start_pos = *begin2;

    cerr << "starting position for rotor[" << rotor_nb << "] is " << start_pos << endl;

  }

  int showStart_pos() {
    return start_pos;
  }

  bool rotate(int rotor_nb) {

    cerr << "incrementing rotor position of rotor[" << rotor_nb << "]" << endl;

    start_pos++;
    for (int i=0; notches[i] != sintinel; i++) {
      if (start_pos == notches[i] && rotor_nb!=0) {

	cerr << "notch met! left rotor will rotate too" << endl;

	 return true;   //even if leftmost rotor does rotate, need to return false so that...
      }
    }                   //...code in main won't try to run rotate(0) on left neighbour
    return false;
  }

  void scramble() {
    letterIndex = config_array[(letterIndex + start_pos) % 26];
  }

  int scramble(int number)  {
    assert(number>=0 && number<26);
    return config_array[(number + start_pos) % 26];
  }

  void inverseScramble() {     //for any valid config file, scramble() is a bijection on {0,.., 25}
    for (int i=0; i<26; i++) { //this inverse function of scramble() exists, and this is it
      if (scramble(i) == letterIndex) {
	letterIndex = i;
	return;
      }
    }
    error(-1);                 //should not ever reach here
  }
};


/*Improve this code by increasing inheritance where possible*/
