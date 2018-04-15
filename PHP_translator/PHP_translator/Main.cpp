#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;
void Syntactic_analyser();
int Lexical_analyser();
void Run_police();
int main() {
	Lexical_analyser();
	Syntactic_analyser();
	Run_police();
	system("pause");
}