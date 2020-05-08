#include "Panel.h"

Panel::Panel()
{
	this->sold = 0.0;
	this->sn = "";
	this->pin = 0;
	this->fn = "";
	this->ln = "";
	this->isRunning = false;
}

Panel::~Panel()
{
	delete[] this->arr;
}

//VERIFICARI
bool Panel::checkLength()
{
	int n = this->pin;
	int count = 0;
	while (n != 0)
	{
		count++;
		n = n / 10;
	}

	if (count == 4)
		return true;

	return false;
}

bool Panel::checkLengthPw(unsigned int pass)
{
	int n = pass;
	int count = 0;
	while (n != 0)
	{
		count++;
		n = n / 10;
	}

	if (count == 4)
		return true;

	return false;
}

void Panel::scanDB()
{
	std::ifstream fin("db.txt");
	std::string x;
	unsigned int count = 0;
	unsigned int i = 0;
	this->arr = new std::string[COLUMNO];

	while (fin >> x)
	{
		if (count >= COLUMNO)
		{
			arr[i] = x;

			if (i == COLUMNO - 1)
			{
				this->myVec.push_back(arr);
				this->arr = new std::string[COLUMNO];
				i = -1; // Indexul o sa fie de fapt 0, ca face i++ la final
			}

			i++;
		}
		count++;
	}
}

void Panel::updateDB()
{
	std::ofstream fout("db.txt");
	remove("db.txt");
	std::string capTabel = "id\tserialNumber\tpassword\tsold\tclientFn\tclientLN\ttransfered\n"; //Mai eficient decat un loop pt fiecare

	fout << capTabel;

	for (size_t i = 0; i < this->myVec.size(); i++)
	{
		for (size_t j = 0; j < COLUMNO; j++)
		{
			fout << myVec[i][j] << "\t";
		}
		fout << "\n";
	}
}

bool Panel::checkData()
{
	this->indexLine = -1;
	bool verified = false, verified1 = false, verified2 = false;
	for (size_t i = 0; i < this->myVec.size(); i++)
	{
		for (size_t j = 0; j < COLUMNO; j++)
		{
			if (this->sn == myVec[i][j])
			{
				this->indexLine = i; //Cu linia asta memoram unde s-a gasit contul, dar ne va trebui si cand afisam sold-ul
				verified1 = true;
			}
			if (std::to_string(this->pin) == myVec[i][j] && i == indexLine)
			{
				verified2 = true;
			}
		}
	}

	if (verified1 == true && verified2 == true)
	{
		verified = true;
	}

	return verified;
}

bool Panel::checkForId(unsigned int id)
{
	this->indexLine = -1;
	bool verified = false;
	for (size_t i = 0; i < this->myVec.size(); i++)
	{
		for (size_t j = 0; j < COLUMNO; j++)
		{
			if (std::to_string(this->id) == myVec[i][j])
			{
				this->indexLine = i; //Cu linia asta memoram unde s-a gasit contul, dar ne va trebui si cand afisam sold-ul
				verified = true;
				break;
			}
		}
		if (verified)
			break;
	}
	return verified;
}
//FINAL

void Panel::initData()
{
	std::cout << "=== Bine ati venit la ATM-ul nostru === \n";
	std::cout << std::setw(40) << std::setfill('=') << "\n";
	std::cout << "Va rugam sa introduceti datele cardului dvs. \n";
	std::cout << "Codul cardului: \n";
	std::cin >> this->sn;
	std::cout << "Parola asociata (4 cifre): \n";
	std::cin >> this->pin;
	std::cout << std::setw(40) << std::setfill('=') << "\n";
	while (!checkLength())
	{
		std::cout << "Dimensiunea asociata pin-ului nu este corecta.\nVa rugam reintroduceti pin-ul \n";
		std::cin >> this->pin;
		std::cout << std::setw(40) << std::setfill('=') << "\n";
	}
	scanDB();
	if (checkData())
	{
		this->isRunning = true;
	}
	else
	{
		std::cout << "Date introduse gresit \n";
	}
}

void Panel::printSold() const
{
	std::cout << std::setw(40) << std::setfill('=') << "\n";
	std::cout << "Suma curenta disponibila este de " << this->myVec[this->indexLine][SOLDCOLUMN] << " RON" << "\n";
	std::cout << std::setw(40) << std::setfill('=') << "\n";
}

void Panel::extractMoney()
{
    int sum;
	std::cout << std::setw(40) << std::setfill('=') << "\n";
	std::cout << "Introduceti suma pe care doriti sa o extrageti: \n";
	std::cin >> sum;

	if (std::stoi(this->myVec[this->indexLine][SOLDCOLUMN]) - sum >= 0)
	{
		unsigned int x;
		x = std::stoi(this->myVec[this->indexLine][SOLDCOLUMN]) - sum;
		this->myVec[this->indexLine][SOLDCOLUMN] = std::to_string(x);
		std::cout << "Tranzactie efectuata cu succes \n";
		std::cout << "Suma disponibila: " << this->myVec[this->indexLine][SOLDCOLUMN] << " RON" << "\n";
		std::cout << std::setw(40) << std::setfill('=') << "\n";
		updateDB();
	}
	else
	{
		std::cout << "Fonduri insuficiente";
		std::cout << std::setw(40) << std::setfill('=') << "\n";
	}
}

void Panel::transferMoney()
{
	std::cout << "Intrduceti id-ul persoanei in contul careia doriti sa depuneti fonduri: \n";
	std::cin >> this->id;
	std::cout << "Suma pe care doriti sa o transferati: ";
	std::cin >> this->sum;
	if (checkForId(this->id))
	{
		std::cout << "Banii vor fii transferati catre persoana " << this->myVec[this->indexLine][FIRSTNAME] << " "
			<< this->myVec[this->indexLine][LASTNAME] << "\n";
		std::cout << "Confirmati? (0-NU, 1-DA) \n";
		std::cin >> this->answer;
		if (this->answer == '1')
		{
			unsigned int x = std::stoi(this->myVec[this->indexLine][SOLDCOLUMN]);
			x += this->sum; //Variabila temporara pentru adunat numere (initial sunt stringuri)
			this->myVec[this->indexLine][SOLDCOLUMN] = std::to_string(x);
			this->myVec[this->indexLine][TRANSFER] = '1';
			updateDB();
			std::cout << "Suma de " << this->sum << " RON a fost transferata cu succes catre persoana selectata.\n";
		}
		else
		{
			return;
		}

	}
	else
	{
		std::cout << "ID-ul introdus nu este corect sau nu exista in baza de date. \n";
	}
	return;
}

void Panel::resetPassword()
{
	unsigned int password;
	std::cout << std::setw(40) << std::setfill('=') << "\n";
	std::cout << "Resetare parola pe baza ID-ului. Din motive de securitate\neste necesara cunoasterea "
		<< "ID-ului asociat contului dvs. pentru efectuarea acestei operatiuni.\n";
	std::cout << "Introduceti ID-ul \n";
	std::cin >> this->id;
	checkForId(this->id);
	unsigned int i1 = this->indexLine;
	checkData();
	unsigned int i2 = this->indexLine;
	if (i1 == i2)
	{
		std::cout << "Potrivire ID-SERIE-PAROLA detectata. Introduceti noua parola \n";
		std::cin >> password;
		while (!checkLengthPw(password))
		{
			std::cout << "\nDimesiunea parolei trebuie sa fie de exact 4 cifre.\n Alegeti o noua parola care sa indeplineasca criteriul \n";
			std::cin >> password;
		}
		this->myVec[this->indexLine][PASSWORD] = std::to_string(password);
		updateDB();
	}
	else
	{
		std::cout << "ID incorect!";
		return;
	}
}

void Panel::menu()
{
	std::cout << "== Meniu central == \n";
	std::cout << std::setw(40) << std::setfill('=') << "\n";
	std::cout << "Selectati optiunea dorita: \n";
	std::cout << "1: Retragere numerar \n";
	std::cout << "2: Afisare sold curent \n";
	std::cout << "3: Transfer bani \n";
	std::cout << "4: Resetare parola \n";
	std::cout << "5: Iesire \n";
	std::cin >> this->option;
	switch (option)
	{
	case 1:
		extractMoney();
		break;
	case 2:
		printSold();
		break;
	case 3:
		transferMoney();
		break;
	case 4:
		resetPassword();
		break;
	default:
		this->isRunning = false;
		break;
	}
}
