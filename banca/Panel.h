#ifndef PANEL_H
#define PANEL_H
#include "includes.h"
class Panel
{
public:
	//Constructori si destructori
	Panel();
	~Panel();

	//Functii
	void menu();
	void initData();
	void extractMoney(); // retragere numerar
	void transferMoney(); //transferul unei sume de bani din contul curent intr-un alt cont al clientului de la aceeasi banca
	void printSold() const;
	void resetPassword();
	//Getter
	inline const bool& getRunning() const { return this->isRunning; }; //Getter mai EFICIENT


private:
	//Date asociate cardului
	double sold;
	std::string sn; //codul contului asociat cardului
	int pin; // pinul
	unsigned int id;
	int option;
	std::string fn;
	std::string ln;
	std::string* arr;
	std::vector <std::string*> myVec;
	unsigned int sum;
	char answer; //Char, mai eficient decat bool

	//Interne program
	bool isRunning;
	unsigned int indexLine;

	bool checkLength(); // verifica daca pin-ul are 4 caractere
	bool checkLengthPw(unsigned int);
	bool checkData();

	//Citire DB
	void scanDB();
	void updateDB();
	bool checkForId(unsigned int);
};
#endif // !PANEL_H
