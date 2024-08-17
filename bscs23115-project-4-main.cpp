#include<windows.h>
#include<conio.h>
#include<iostream>
#include<time.h>
using namespace std;
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
struct food
{
	int fr, fc;
};
struct position
{
	int ri, ci;
};
enum direction
{
	R, L, D, U
};
struct snake
{
	position* p;
	int size;
	int c;
	direction dir;
	int U, D, R, L;
};
void addatend(int * & a,int &size, int e)
{
	int* t = a;
	a = new int[size + 1];
	for (int i = 0; i < size; i++)
	{
		a[i] = t[i];
	}
	a[size]=e;
	size++;
	delete[]t;
}
void init(snake* &sa, int rows, int cols,string *&names,int &nop)
{
	cout << "ENTER NUMBER OF PLAYERS (AT MAX 4): " << endl;
	do
	{
		cin >> nop;
		if (nop > 4)
		{
			cout<<"ENTER NUMBER LESS THAN 5" << endl;
		}
	} while (nop > 4);
	names = new string[nop];
	sa = new snake[nop];
	for (int i = 0; i < nop; i++)
	{
		cout << "ENTER NAME OF PLAYER " << i + 1<<" : ";
		cin >> names[i];
		sa[i].size = 5;
		sa[i].p = new position[sa[i].size];
		sa[i].dir = R;
		sa[i].c = i + 2;
		for (int j = 0; j <sa[i].size; j++)
		{
			sa[i].p[j].ri = (i+1)*20;
			sa[i].p[j].ci = cols/4 - i;
		}
		cout << "---------------CONTROLS CONFIGURATION FOR PLAYER " << i + 1 << "---------------" << endl;
		cout << "DO NOT USE SPECIAL KEYS" << endl<<endl<<endl;
		cout << "ENTER KEY FOR RIGHT MOVEMENT : " << endl;
		sa[i].R = _getch();
		cout << "ENTER KEY FOR LEFT MOVEMENT : " << endl;
		sa[i].L = _getch();
		cout << "ENTER KEY FOR UPWARDS MOVEMENT : " << endl;
		sa[i].U = _getch();
		cout << "ENTER KEY FOR DOWNWARDS MOVEMENT : " << endl;
		sa[i].D = _getch();
	}
}
bool ispresent(int* p, int size, int e)
{
	for (int i = 0; i < size; i++)
	{
		if (p[i]==e)
			return true;
	}
	return false;
}
void displaysnake(snake* sa, char sym,int nop,int *dead,int dsize)
{
	for (int i = 0; i < nop; i++)
	{
		if (!ispresent(dead, dsize, i))
		{
			for (int j = 0; j < sa[i].size; j++)
			{
				if (j == 0)
				{
					gotoRowCol(sa[i].p[j].ri, sa[i].p[j].ci);
					SetClr(sa[i].c);
					cout << char(153);

				}
				else {
					gotoRowCol(sa[i].p[j].ri, sa[i].p[j].ci);
					SetClr(sa[i].c);
					cout << sym;
				}
			}
		}
	}
}
void movesnake(snake* &sa, int rows, int cols,int nop,int *dead,int dsize)
{
	for (int j = 0; j < nop; j++)
	{
		if (!ispresent(dead, dsize, j))
		{
			for (int i = sa[j].size - 1; i > 0; i--)
			{
				sa[j].p[i] = sa[j].p[i - 1];
			}
			switch (sa[j].dir)
			{
			case U:
				sa[j].p[0].ri--;
				break;
			case D:
				sa[j].p[0].ri++;
				break;
			case R:
				sa[j].p[0].ci++;
				break;
			case L:
				sa[j].p[0].ci--;
				break;
			default:
				break;
			}
		}
		
	}
}
void chagedirection(snake * & sa, int  in,int nop,int *dead,int dsize)
{
	for (int i = 0; i < nop; i++)
	{
		if(!ispresent(dead, dsize, i))
		{
		if (in == sa[i].R)
		{
			if (sa[i].dir == L)
				return;
			sa[i].dir = R;
		}

		else if (in == sa[i].L)
		{
			if (sa[i].dir == R)
				return;
			sa[i].dir = L;
		}

		else if (in == sa[i].D)
		{
			if (sa[i].dir == U)
				return;
			sa[i].dir = D;
		}

		else if (in == sa[i].U)
		{
			if (sa[i].dir == D)
				return;
			sa[i].dir = U;
		}
		}
	}
}
bool isvalid(food f, snake * sa,int nop,int *dead,int dsize,int rows,int cols)
{
	if (f.fc > cols - 2 || f.fr > rows - 2 || f.fc < 2 || f.fr < 2)
		return false;
	for (int j = 0; j < nop; j++)
	{
		if (!ispresent(dead, dsize, j))
		{
			for (int i = 0; i < sa[j].size; i++)
			{
				if (sa[j].p[i].ri == f.fr && sa[j].p[i].ci == f.fc)
					return false;
			}
		}
	}
	return true;
}
void generatefood(food& f, int rows, int cols, snake *s,int nop,int *dead,int dsize)
{
	do {
		f.fc = rand() % (cols - 2);
		f.fr = rand() % (rows - 2);
	} while (!isvalid(f, s,nop, dead, dsize,rows,cols));
}
void displayfood(food f, char sym)
{
	SetClr(12);
	gotoRowCol(f.fr, f.fc);
	cout << sym;
}
void displayspecialfood(food f, char sym)
{
	SetClr(7);
	gotoRowCol(f.fr, f.fc);
	cout << sym;
}
bool foodeaten(snake *sa, food f,int nop,int &sn,int *dead,int dsize)
{
	for (int i = 0; i < nop; i++)
	{
		if (!ispresent(dead, dsize, i))
		{
			if (f.fc == sa[i].p[0].ci && f.fr == sa[i].p[0].ri)
			{
				sn = i;
				return true;
			}
		}
	}
	return false;
}
bool snakekilled(snake* s, int nop, int sn, int rows, int cols,int *dead,int dsize)
{
	if (s[sn].p[0].ri == 1 || s[sn].p[0].ri == rows - 2 || s[sn].p[0].ci == 1 || s[sn].p[0].ci == cols - 2)
		return true; 
	for (int i = 0; i < nop; i++)
	{
		if (ispresent(dead, dsize, i))
			continue;
		if (i == sn)
		{
			for (int j = 1; j < s[i].size; j++)
			{
				if (s[sn].p[0].ri == s[i].p[j].ri && s[sn].p[0].ci == s[i].p[j].ci)
					return true;
			}
		}
		else
		{
			for (int j = 0; j<s[i].size; j++)
			{
				if (s[sn].p[0].ri == s[i].p[j].ri && s[sn].p[0].ci == s[i].p[j].ci)
					return true;
			}
		}
	}
	return false;
}
void iskill(snake* s, int nop,int rows,int cols, int* &dead,int &dsize)
{
	for (int i = 0; i < nop; i++)
	{
		if (ispresent(dead, dsize, i))
			continue;
		if (snakekilled(s, nop, i, rows, cols, dead, dsize))
		{
			addatend(dead, dsize, i);
			
			for (int k = 1; k < s[i].size; k++)
			{
				gotoRowCol(s[i].p[k].ri, s[i].p[k].ci);
				cout <<' ';
			}
			return;
		}
	}
}
void snakestretch(snake* & s,int sn)
{
	position* t = new position[s[sn].size + 1];
	for (int i = s[sn].size - 1; i - 1 >= 0; i--)
	{
		t[i - 1] = s[sn].p[i];
	}
	t[0] = s[sn].p[1];
	delete[]s[sn].p;
	s[sn].p = t;
	s[sn].size++;
}
void increasescore(int* score, int n)
{
	score[n]++;
}
void increasescore2(int* score, int n)
{
	score[n]+=5;
}
void displayscore(int* score, int nop)
{
	for (int i = 0; i < nop; i++)
	{
		gotoRowCol(0, i *25);
		cout << "PLAYER " << i+1<< " : " << score[i];
	}
}
void printboundary(int rows, int cols, char sym)
{
	SetClr(35);
	for (int i = 1; i < rows - 1; i++)
	{
		gotoRowCol(i, 1);
		cout << sym;
		gotoRowCol(i, cols - 2);
		cout << sym;
	}
	for (int i = 1; i < cols - 1; i++)
	{
		gotoRowCol(1, i);
		cout << sym;
		gotoRowCol(rows - 2, i);
		cout << sym;
	}

}
void movesnake2(snake*& sa, int rows, int cols, int nop, int* dead, int dsize)
{
	for (int j = 0; j < nop; j++)
	{
		if (!ispresent(dead, dsize, j))
		{
			for (int i = sa[j].size - 1; i > 0; i--)
			{
				sa[j].p[i] = sa[j].p[i - 1];
			}
			switch (sa[j].dir)
			{
			case U:
			{
				sa[j].p[0].ri--;
				if (sa[j].p[0].ri < 0)
					sa[j].p[0].ri = rows - 1;
				break;
			}
			case D:
			{
				sa[j].p[0].ri++;
				if (sa[j].p[0].ri >= rows)
					sa[j].p[0].ri = 0;
				break;
			}
			case R:
			{
				sa[j].p[0].ci++;

				if (sa[j].p[0].ci >=cols)
					sa[j].p[0].ci = 0;
				break;
			}
			case L:
			{
				sa[j].p[0].ci--;
				if (sa[j].p[0].ci <0 )
					sa[j].p[0].ci = cols-1;
				break;
			}
			default:
				break;
			}
		}

	}
}
bool snakekilled2(snake* s, int nop, int sn, int rows, int cols, int* dead, int dsize)
{
	for (int i = 0; i < nop; i++)
	{
		if (ispresent(dead, dsize, i))
			continue;
		if (i == sn)
		{
			for (int j = 1; j < s[i].size; j++)
			{
				if (s[sn].p[0].ri == s[i].p[j].ri && s[sn].p[0].ci == s[i].p[j].ci)
					return true;
			}
		}
		else
		{
			for (int j = 0; j < s[i].size; j++)
			{
				if (s[sn].p[0].ri == s[i].p[j].ri && s[sn].p[0].ci == s[i].p[j].ci)
					return true;
			}
		}
	}
	return false;
}
void iskill2(snake* s, int nop, int rows, int cols, int*& dead, int& dsize)
{
	for (int i = 0; i < nop; i++)
	{
		if (ispresent(dead, dsize, i))
			continue;
		if (snakekilled2(s, nop, i, rows, cols, dead, dsize))
		{
			addatend(dead, dsize, i);

			for (int k = 0; k < s[i].size; k++)
			{
				gotoRowCol(s[i].p[k].ri, s[i].p[k].ci);
				cout <<' ';
			}
			return;
		}
	}
}
void printresults(string* names, int nop, int rows, int cols,int *score)
{
	system("cls");
	for (int i = 0; i < nop; i++)
	{
		gotoRowCol(rows / 2 - 2, cols / 2 - 6);
		cout << "***GAME OVER***";
		gotoRowCol(rows / 2 + i+1, cols / 2 - 11);
		cout << "PLAYER " << i + 1 << " "<<names[i]<<"'s score : " << score[i];
	}
}
void generatespfood(food &f,int rows,int cols,int nop,int *dead,int dsize,snake *s)
{
	do {
		f.fc = rand() % cols - 1;
		f.fr = rand() % rows - 1;
	} while (!isvalid(f, s, nop, dead, dsize,rows,cols));
}

int main2()
{
	srand(time(0));
	int t = time(0);
	int rows = 91, cols = 160, nop, dsize = 0, sn = -1;
	snake* sa;
	string* names;
	int* dead = new int[dsize];
	food f, specialfood{};
	char sksym = -37, spsym = ' ', frsym = 3;
	init(sa, rows, cols,names, nop);
	int* score = new int[nop] {};
	system("cls");
	generatefood(f, rows, cols, sa, nop, dead, dsize);
	displayfood(f, frsym);
	printboundary(rows, cols, '*');
	while (true)
	{
		displayscore(score, nop);
		if (_kbhit())
		{
			int in = _getch();
			chagedirection(sa, in, nop, dead, dsize);
		}
		displaysnake(sa, sksym, nop, dead, dsize);
		Sleep(120);
		displaysnake(sa, spsym, nop, dead, dsize);
		movesnake(sa, rows, cols, nop, dead, dsize);
		iskill(sa, nop, rows, cols, dead, dsize);
		if (time(0) - t == 20)
		{
			generatefood(specialfood, rows, cols,sa,nop,dead,dsize);
		}
		if (time(0)-t>20)
		{
			displayspecialfood(specialfood,'*');
			if (time(0)-t>= 35)
			{
				t = time(0);
				displayfood(specialfood, ' ');
			}
		}
		if (foodeaten(sa, f, nop, sn, dead, dsize))
		{
			cout << "\a";
			snakestretch(sa, sn);
			increasescore(score, sn);
			generatefood(f, rows, cols, sa, nop, dead, dsize);
			displayfood(f, frsym);
		}
		if (foodeaten(sa, specialfood, nop, sn, dead, dsize))
		{
			cout << "\a";
			snakestretch(sa,sn);
			increasescore2(score, sn);
			specialfood.fc = -1;
			specialfood.fr = -1;
			t = time(0);
		}
		if (dsize == nop)
		{
			printresults(names, nop, rows, cols, score);
			break;
		}
	}
	_getch();
	return 0;
}

int main1()
{
	srand(time(0));
	int t = time(0);
	int rows = 91, cols = 160, nop;
	snake* sa;
	int dsize = 0;
	string* names;
	int* dead = new int[dsize];
	food f, specialfood{};
	char sksym = -37, spsym = ' ', frsym = 3;
	int sn = -1;
	init(sa, rows, cols, names,nop);
	int* score = new int[nop] {};
	system("cls");
	generatefood(f, rows, cols, sa, nop, dead, dsize);
	displayfood(f, frsym);
	while (true)
	{
		if (_kbhit())
		{
			int in = _getch();
			chagedirection(sa, in, nop, dead, dsize);
		}
		displaysnake(sa, sksym, nop, dead, dsize);
		Sleep(120);
		displaysnake(sa, spsym, nop, dead, dsize);
		movesnake2(sa, rows, cols, nop, dead, dsize);
		iskill2(sa, nop, rows, cols, dead, dsize);
		if (time(0) - t == 20)
		{
			generatefood(specialfood, rows, cols, sa, nop, dead, dsize);
		}
		if (time(0) - t > 20)
		{
			displayspecialfood(specialfood, '*');
			if (time(0) - t >= 35)
			{
				t = time(0);
				displayfood(specialfood, ' ');
			}
		}
		if (foodeaten(sa, f, nop, sn, dead, dsize))
		{
			cout << "\a";
			snakestretch(sa, sn);
			increasescore(score, sn);
			generatefood(f, rows, cols, sa, nop, dead, dsize);
			displayfood(f, frsym);
		}
		if (foodeaten(sa, specialfood, nop, sn, dead, dsize))
		{
			cout << "\a";
			snakestretch(sa, sn);
			increasescore2(score, sn);
			specialfood.fc = -1;
			specialfood.fr = -1;
			t = time(0);
		}
		if (dsize == nop)
		{
			printresults(names, nop, rows, cols, score);
			break;
		}
	}
	_getch();
	return 0;
}





void menu()
{
	cout << "-------------------------------------" << endl;
	cout << "1. SNAKE GAME WITHOUT BOUNDARIES" << endl;
	cout << "2. SNAKE GAME WITH BOUNDARIES" << endl;
	cout << "-------------------------------------" << endl;
}
int main()
{
	int choice;
		menu();
		cout << endl << "ENTER CHOICE : ";
		cin >> choice;
		system("cls");
		switch (choice)
		{
		case 1:
			main1();
			break;
		case 2:
			main2();
			break;
		default:
			break;
		}
		_getch();
	return 0;
}










