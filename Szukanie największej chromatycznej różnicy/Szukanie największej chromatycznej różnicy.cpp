#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <windows.h>

using namespace std;

/*struktura definiujące jeden pixel i kolory z jakich się składa*/
struct pix
{
	unsigned char b;
	unsigned char g;
	unsigned char r;

	pix()
	{
		r = 0;
		g = 0;
		b = 0;
	}
	pix(char a, char b, char c)
	{
		b = a;
		g = b;
		r = c;
	}
};

struct varianceAndPosition
{
	int x, y;
	int variance;

	varianceAndPosition()
	{
		x = 0; y = 0; variance = 0;
	}

	varianceAndPosition(int nv, int nx, int ny)
	{
		x = nx; y = ny; variance = nv;
	}
};

typedef vector<vector<pix>> matrix;
typedef vector<vector<int>> matrixi;



//wczytywanie bitmapy matrix który przechowuje macież struktur pix z zapisanymi trzema kolorami
matrix wczytywaniebmp(string nazwa, bool wyp)
{
	matrix bmp;
	pix ap;
	pix tst;
	unsigned char temc;
	int temi;
	int poczatek;
	int wysokosc;
	int szerokosc;
	ofstream output(nazwa + "M.txt");

	std::ifstream input(nazwa, std::ios::binary);
	if (!input)
	{
		std::cout << "Nie udalo sie otworzyc pliku." << std::endl;
		return bmp;
	}
	input.read(reinterpret_cast<char*> (&temc), 1);
	std::cout << temc;
	input.read(reinterpret_cast<char*> (&temc), 1);
	std::cout << temc << std::endl;
	input.read(reinterpret_cast<char*> (&temi), 4);
	std::cout << "Rozmiar bmp: " << temi << std::endl;
	input.seekg(10);
	input.read(reinterpret_cast<char*> (&poczatek), 4);
	cout << "poczatek = " << poczatek << endl;
	input.seekg(18);
	input.read(reinterpret_cast<char*> (&szerokosc), 4);
	cout << "Szerokosc: " << szerokosc << endl;
	input.seekg(22);
	input.read(reinterpret_cast<char*> (&wysokosc), 4);
	cout << "Wysokosc: " << wysokosc << endl;

	int przesuniecie = szerokosc % 4;
	input.seekg(poczatek);
	for (int i = 0; i < wysokosc; i++)
	{
		bmp.push_back(vector<pix>());
		for (int j = 0; j < szerokosc; j++)
		{
			input.read(reinterpret_cast<char*> (&temc), 1);
			ap.b = temc;

			input.read(reinterpret_cast<char*> (&temc), 1);
			ap.g = temc;

			input.read(reinterpret_cast<char*> (&temc), 1);
			ap.r = temc;

			bmp[i].push_back(ap);
			int temb, temg, temr;
			temb = ap.b;
			temg = ap.g;
			temr = ap.r;
			if (wyp == 1)
			{
				output << i << " " << j << " ";
				output << temb << " ";
				output << temg << " ";
				output << temr << endl; //wypisywanie wyniku do pliku txt
			}
		}
		input.seekg(przesuniecie, ios_base::cur);
	}
	return bmp;
}

//sumowanie wszystkich wartości kolorów w danym pikselu i zapisywanie go do matrixi który przechowuje macież intów
matrixi monizacja(matrix bmp)
{
	matrixi cbmp;
	int ap;
	int x = bmp.size();
	int y = bmp[0].size();
	//ofstream output(nazwa + "CM.txt");

	for (int i = 0; i < x; i++)
	{
		cbmp.push_back(vector<int>());
		for (int j = 0; j < y; j++)
		{
			ap = pow(pow(bmp[i][j].b, 2) + pow(bmp[i][j].g, 2) + pow(bmp[i][j].r, 2), 1/3.0);

			cbmp[i].push_back(ap);

			//output << i << " " << j << " " << ap << endl; //wypisywanie wyniku funkcji do pliku txt

		}
	}
	return cbmp;
}

int main()
{
	matrixi firstPhoto, secondPhoto;
	string name1, name2;

	std::cout << "Podaj nazwe pierwszego zdjecia .bmp:\n";
	std::cin >> name1;
	std::cout << "Podaj nazwe drugiego zdjecia .bmp:\n";
	std::cin >> name2;

	firstPhoto = monizacja(wczytywaniebmp(name1, 0));
	secondPhoto = monizacja(wczytywaniebmp(name2, 0));

	/*porównywanie dwóch wczytanych bitmap i szukanie największej chromatycznej różnicy*/
	int x = 0, y = 0, valueOfDifference = 0, temVoD = 0, temDifference = 0;
	int width, higth;
	std::vector<varianceAndPosition> nwjwiekszyWKolumnie;
	std::vector<varianceAndPosition> nwjwiekszyWWierszu;
	int srednia;
	int intensivity;
	int disChrom;
	varianceAndPosition temBiggest;



	if (firstPhoto.size() < secondPhoto.size())
	{
		higth = firstPhoto.size();
	}
	else
	{
		higth = secondPhoto.size();
	}

	if (firstPhoto[0].size() < secondPhoto[0].size())
	{
		width = firstPhoto[0].size();
	}
	else
	{
		width = secondPhoto[0].size();
	}

	for (int i = 0; i < higth; i++)
	{
		temBiggest = varianceAndPosition(0, i, 0);
		for (int j = 0; j < width; j++)
		{
			
			temDifference = abs(firstPhoto[i][j] - secondPhoto[i][j]);
			if (temDifference > valueOfDifference)
			{

				valueOfDifference = temDifference;
				x = i;
				y = j;
			}

			if (temDifference > temVoD)
			{
				temBiggest = varianceAndPosition(temDifference, i, j);
			}
		}
		nwjwiekszyWWierszu.push_back(temBiggest);
		temVoD = 0;
	}

	for (int i = 0; i < width; i++)
	{
		temBiggest = varianceAndPosition(0, i, 0);
		for (int j = 0; j < higth; j++)
		{
			temDifference = abs(firstPhoto[j][i] - secondPhoto[j][i]);

			if (temDifference > temVoD)
			{
				temBiggest = varianceAndPosition(temDifference, i, j);
			}
		}
		nwjwiekszyWKolumnie.push_back(temBiggest);
		temVoD = 0;
	}

	std::cout << "\nNajwieksza roznica chromatyczna wystepuje w kolumnie " << x << " i w wierszu " << y << "\nWynosi ona: " << valueOfDifference << "/" << 255 <<"\n";
	std::cout << "Szerokości i wysokości: " << nwjwiekszyWKolumnie.size() << " i " << nwjwiekszyWWierszu.size() << "\n";


	matrixi diefferenceBitmap(higth);
	HDC hdcc = GetDC(GetConsoleWindow());

	for (int i = 0; i < higth; i++)
	{
		diefferenceBitmap[i] = std::vector<int>(width);
	}

	for (int i = 0; i < higth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			diefferenceBitmap[i][j] = abs(firstPhoto[i][j] - secondPhoto[i][j]);
		}
		
	}

	int mul = 1;
	mul = 800 / higth;


	int choice = 0;
	do
	{
		std::cout << "0 - wyjdz\n1 - wypisz największe dla wierszy [roznica, numer wiersza]\n2 - wypisz największe dla kolumny [roznica, numer wiersza]\n3 - pokaz monochromatyczna mape roznic (*rekomendowane do wizualnej lokalizacji roznic)\n";
		std::cin >> choice;
		if (choice == 1)
		{
			for (int i = 0; i < nwjwiekszyWWierszu.size(); i++)
			{
				std::cout << "Wiersz nr. " << nwjwiekszyWWierszu[i].x << " kolumna nr. " << nwjwiekszyWWierszu[i].y << " roznica: " << nwjwiekszyWWierszu[i].variance << std::endl;
			}
		}

		if (choice == 2)
		{
			for (int i = 0; i < nwjwiekszyWKolumnie.size(); i++)
			{
				std::cout << "Kolumna nr. " << nwjwiekszyWKolumnie[i].x << " wiersz nr. " << nwjwiekszyWKolumnie[i].y << " roznica: " << nwjwiekszyWKolumnie[i].variance << std::endl;
			}
		}

		if (choice == 3)
		{
			std::cout << "Wybierz kontrast 1-100: ";
			std::cin >> intensivity;
			system("cls");
			for (int t = 0; t < 40; t++)
			{
				std::cout << "\n";
			}
			
			for (int i = 0; i < higth; i++/*int i = width-1; i >-1 ; i--*/)
			{
				for (int j = 0; j < width; j++)
				{
					for (int ii = 0; ii < mul; ii++)
					{
						for (int jj = 0; jj < mul; jj++)
						{
							disChrom = diefferenceBitmap[i][j] * intensivity;
							while (disChrom > 255)
							{
								disChrom -= 1;
							}
							SetPixel(hdcc, j * mul +jj, (higth - i) * mul +ii, RGB(disChrom, disChrom, disChrom));
						}
					}
				}

			}
			
		}

		

	}while (choice != 0);

	Sleep(2500);
}

