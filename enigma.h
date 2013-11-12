/*sentinel for integer array, intended for arrays containing configuration information. 
  this is a good value because -1 could never appear as a 'proper' configuration value. */
#define sintinel -1    

/*generic error codes*/
#define INVALID_INDEX -10 //not between 0 and 25
#define NON_NUMERIC_CHARACTER -11

/*input error codes*/
#define INSUFFICIENT_NUMBER_OF_PARAMETERS -12
#define INVALID_INPUT_CHARACTER -13

/*plugboard error codes*/
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION -14
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS -15

/*rotor error codes*/
#define INVALID_ROTOR_MAPPING -16
#define NO_ROTOR_STARTING_POSITION -17

/*reflector error codes*/
#define INVALID_REFLECTOR_MAPPING -18
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS -19

/*used in base class constructor*/      
#define plugboard 0
#define reflector 1


class PieceOfHardware {      //base class for plugboard, reflector, rotor
 protected:
  int letterIndex;           //holds index value of letter to encrypt
  int configArray[27];
  int configArraySize;

 public:
  PieceOfHardware();
  PieceOfHardware(const char* configurationFilename, int hardwareType); //performs most error checks
  void error(int errorCode); //function for outputting error descriptions and exiting
  int showLetterIndex() const;
  void setLetterIndex(int newLetterIndex);
};


class Plugboard : public PieceOfHardware {
 public:
  Plugboard(const char* configurationFilename); //performs most error checks
  bool getLetterFromInputFile();
  void scramble();
  int scramble(int letterIndexToScramble);
  void inverseScramble();
};


class Reflector : public PieceOfHardware {
 public:
  Reflector(const char* configurationFilename); //performs most error checks
  void scramble();
};


class Rotor : public PieceOfHardware {
private:
   int notches[50];
   int rotPos;
   int order;

 public:                                        //constructor performs most error checks
   Rotor(const char* configurationFileName, const char* rotorPositionFilename, int rotorNumber);
   int showRotPos() const;
   void showNotches();
   bool rotate();
   void scramble();
   int scramble(int letterIndexToScramble);
   void inverseScramble();
};
