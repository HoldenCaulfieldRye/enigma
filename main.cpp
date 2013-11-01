#include<iostream>
using namespace std;

#include"enigma.cpp"

int main() {
  char continu = 'y';
  Plugboard pb;

  while(continu=='y') {
    pb.getInput();
    cout << "again? ";
    cin >> continu;
  }
  return 0;
}
