/*sentinel for integer array, intended for arrays containing configuration information. takes up less memory than remembering array size. -1 is a good value because -1 could never appear as a 'proper' configuration value*/
#define sintinel -1    

/* error codes  */
#define INSUFFICIENT_NUMBER_OF_PARAMETERS		1
#define INVALID_INPUT_CHARACTER				2
#define INVALID_INDEX					3
#define NON_NUMERIC_CHARACTER				4
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION		5
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS	6
#define INVALID_ROTOR_MAPPING				7
#define NO_ROTOR_STARTING_POSITION			8
#define INVALID_REFLECTOR_MAPPING			9
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS	10
#define ERROR_OPENING_CONFIGURATION_FILE		11
#define UNKNOWN_ERROR                                   -1
#define NO_ERROR					0

/*used in base class constructor*/      
#define plugboard 0
#define reflector 1

/*this class is only instanciated once in the program, but it is still relevant, because it indicates that there is a specific, "private" machine mechanism*/
class Enigma;

/*this base class is for machine component subclasses to inherit common fields and methods*/
class PieceOfHardware {
 protected:
  Enigma* machine;
  int letterIndex;           //holds index value of letter to encrypt
  int configArray[27];

 public:
  PieceOfHardware();
  PieceOfHardware(Enigma* _machine);
  bool build(const char* configurationFilename, int hardwareType); //performs most error checks
  void setLetterIndex(int const &newLetterIndex);
};


class Plugboard : public PieceOfHardware {
 public:
  Plugboard(); //TRY DELETING
  Plugboard(Enigma *_machine);
  bool build(const char* configurationFilename);
  bool getLetterFromInputFile();
  int scramble();
  int scramble(int letterIndexToScramble) const;
  void inverseScramble();
  int getLetterIndex() const;
};


class Reflector : public PieceOfHardware {
 public:
  Reflector(); //TRY DELETING
  Reflector(Enigma *_machine);
  bool build(const char* configurationFilename);
  int scramble();
};


class Rotor : public PieceOfHardware {
private:
   int notches[50];
   int rotPos;

 public: 
   Rotor(); //TRY DELETING
   Rotor(Enigma *_machine);
   bool build(const char* configFileName, const char* rotPosFilename, int rotNumber);
   int getRotPos() const;
   bool rotate();
   int scramble();
   int scramble(int letterIndexToScramble) const;
   int inverseScramble();
};


class Enigma {
 private:
  int nb_rotors;
  int errorCode;
  Plugboard pb;
  Reflector rf;
  Rotor **rotor;

 public:
  Enigma();
  void errorDescription(int code);//also sets errorCode
  int getErrorCode() const;
  bool build(int argc, char** argv);
  bool encrypt();
  ~Enigma();
};

