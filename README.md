enigma
======

Implementation of an Enigma encrypting machine in object-oriented C++.

An Enigma machine is any of a family of related electro-mechanical rotor cipher machines used in the twentieth century 
for enciphering and deciphering secret messages. Models were adopted by military and government services of numerous 
countries throughout the first half of the 20th century, particularly Germany during World War II.  
photo: http://www.ilord.com/images/enigma-8-rotors-1000px.jpg  

A machine is composed of a plugboard, a reflector and a number of rotors, each of which further encrypt the message. This 
program is run from a shell command containing pathnames to configuration files for each machine component. Several 
configuration files can be found in the 'plugboards','reflectors' and 'rotors' folders. So, an example of how to run this 
program from Linux shell would be:

1) git clone https://github.com/HoldenCaulfieldRye/enigma Enigma  
2) cd Enigma  
3) ./enigma plugboards/I.pb reflectors/III.rf rotors/II.rot rotors/V.rot rotors/VII.rot rotors/5.pos < encrypted.txt  
4) <enter your message, capital letter only, no punctuation, then hit return>  
5) cat encrypted.txt  
6) ./enigma plugboards/I.pb reflectors/III.rf rotors/II.rot rotors/V.rot rotors/VII.rot rotors/5.pos > encrypted.txt  
  
The 6th command is there to show that another remote user can obtain the original message by plugging the encrypted message 
into a machine with the same configuration.   
  
The encryption can only be cracked by figuring how each component maps input to output. In cryptography, the approach is to 
deduce this bijection with combinatorics and a sufficiently large input-output dataset. What makes Enigma particularly 
difficult to crack is that as each character is encrypted, rotors rotate pseudo-randomly, and this alters their mapping.

For full specification of this program, see pdf file in repo.  
