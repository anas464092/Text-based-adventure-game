#include <iostream>
#include <windows.h> 
#include <vector>

#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

using namespace std;

void menu();
void exit();
void simulateBattle(char mode);
bool checker();
void encounter();
void response(char, char);

char choice;   //  variable to take your response....
int mhp = 100;     //  mafia hp
int hp = 100;      //  agent hp
int atk = 10;  //  variable for attack of agent...
int matk = 10;   //  attack power for the mafia
int temphp = 0;
int tempmhp = 0;

char tmp_map[23][50];

char map[23][50] = {
	"+=========================================+",
	"|                    |                    |",
	"|    +=+   +=====+   |   +=====+   +=+    |",
	"|    +=+   +=====+   +   +=====+   +=+    |",
	"|                                         |",
	"|    ===   +   ======+======   +   ===    |",
	"|          |         |         |          |",
	"| =====+   +======   +   ======+   +===== |",
	"|      |   |                   |   |      |",
	"| =====+   +   +====   ====+   +   +====  |",
	"|              |           |              |",
	"| =====+   +   |           |   +   +===== |",
	"|      |   |   +====   ====+   |   |      |",
	"|      |   |                   |   |      |",
	"| =====+   +   ======+======   +   +===== |",
	"|                    |                    |",
	"|    ==+   =======   +   =======   +==    |",
	"|      |                           |      |",
	"| ==   +   +   ======+======   +   +   == |",
	"|          |         |         |          |",
	"|    ======+======   +   ======+======    |",
	"|                                         |",
	"+=========================================+"
};

void loading() {   //   function for loading output graphics

	system("cls");
	system("COLOR 0e");

	SetConsoleCP(437);
	SetConsoleOutputCP(437);

	int bar1 = 177, bar2 = 219;
	cout << "Game Start" << endl;
	cout << "\n\n\n\n\t\t\t\tLoading....";
	cout << "\n\n\n\n\t\t\t\t";

	for (int i = 0; i < 25; i++) {
		cout << (char)bar1;
	}

	cout << "\r";
	cout << "\t\t\t\t";

	for (int i = 0; i < 25; i++) {
		cout << (char)bar2;
		Sleep(40);
	}
	cout << endl;

	return;
}

void ShowMap()
{
	for (int i = 0; i < 23; i++)
	{
		cout << YELLOW;
		cout << map[i] << endl;
		cout << RESET;
	}
}

void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}

struct Entity
{
	int x;
	int y;

	Entity(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void move_x(int p)
	{
		if (map[y][x + p] == ' ')
			x += p;
	}

	void move_y(int p)
	{
		if (map[y + p][x] == ' ')
			y += p;
	}

	void move(int p, int q)
	{
		x += p;
		y += q;
	}

	int get_x() { return x; }
	int get_y() { return y; }

	void draw(char p)
	{
		map[x][y] = p;
		gotoxy(x, y);
		printf("%c", p);
	}
};

struct walk
{
	short walk_count;
	short x;
	short y;
	short back;
};

struct target
{
	short x;
	short y;
};

vector<target> walk_queue;

vector<walk> BFSArray;

void AddArray(int x, int y, int wc, int back)
{
	if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.')
	{
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back(tmp);
	}
}

void FindPath(int sx, int sy, int x, int y)   //  (ex , ey , ax , ay)
{
	memcpy(tmp_map, map, sizeof(map));   // copying map to temp map
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;      //  starting x index for enemy
	tmp.y = sy;      //  starting y index for enemy
	tmp.walk_count = 0;    //   counring steps to reach enemy to agent....
	tmp.back = -1;        //   for no predecesor of starting point ....
	BFSArray.push_back(tmp);   //  adding temp(walk struct) to the vector bfsarray...

	int i = 0;
	while (i < BFSArray.size())
	{
		if (BFSArray[i].x == x && BFSArray[i].y == y)
		{
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0)
			{
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back(tmp2);

				i = BFSArray[i].back;
			}

			break;
		}

		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);

		i++;
	}

	BFSArray.clear();
}

int main() {

	char enter;   //   for start or exit the game

	system("COLOR 0e");
	gotoxy(80, 10);
	cout << "+==========+" << endl;
	gotoxy(80, 11);
	cout << "| Agent 47 |";
	gotoxy(80, 12);
	cout << "+==========+" << endl;
	Sleep(2000);
	system("cls");

	cout << "\033[92m";
	cout << "Hey...  Agent you are in a maze city." << endl;
	cout << "You are a secret agent and you have to kill MAFIA..." << endl;
	cout << "There is a mafia in this city from which you have to escape to gaining score..." << endl;
	cout << "If you caught by enemy you will loose..." << endl << endl;

	cout << "\033[96m";
	gotoxy(85, 3);
	cout << "Maximize the console............." << endl;
	cout << "\033[0m";

	cout << endl << endl;
	cout << "\033[93m";

	while (true) {
		cout << "S --> Start" << endl;
		cout << "E --> Exit" << endl << endl;

		cout << "Select your option : ";
		cin >> enter;

		if (enter == 'S' || enter == 's') {
			system("cls");
			Sleep(100);
			loading();
			Sleep(50);
			system("cls");
			menu();
			break;  // Exit the loop after successful menu selection
		}
		else if (enter == 'e' || enter == 'E') {
			exit();
			break;  // Exit the loop if the user chooses to exit
		}
		else {
			system("cls");
			Sleep(50);
			cout << "Invalid option" << endl;
			cout << "Select again...." << endl << endl;
			Sleep(50);
		}
	}
	system("cls");
	return 0;
}

void menu()
{
	bool select = true;   //   for error handling of anyother character..........
	srand(time(NULL));
	bool running = true;
	int x = 1; // hero x  
	int y = 1; // hero y  
	int old_x;
	int old_y;

	int ex = 21;   // enemy x
	int ey = 10;    //  enemy y

	int pts = 0;   //   reward

	char diffi;     //  difficulty level

	printf("Instruction:\n\n1. Arrow Keys to move.\n2. Eat the dots produced by the enemy to gain poins\n3. Don't get caught by the Enemy\n\n");

	while (select)
	{

		cout << "H -> Hard\nN -> Normal\nE -> Easy\nQ -> Quit\n\nSelect game mode : ";
		cin >> diffi;
		if (diffi == 'N' || diffi == 'H' || diffi == 'E' || diffi == 'n' || diffi == 'h' || diffi == 'e')
		{
			loading();
			Sleep(200);

			int speedmod;

			if (diffi == 'E' || diffi == 'e')
			{
				cout << "\033[96m";
				system("cls");
				gotoxy(60, 10);
				cout << "You choose 'EASY' game mode..." << endl;
				gotoxy(60, 11);
				cout << "In this mode you have to score 300 to encounter mafia..." << endl;
				Sleep(3000);
				loading();
				speedmod = 3;
			}

			if (diffi == 'N' || diffi == 'n')
			{
				cout << "\033[96m";
				system("cls");
				gotoxy(60, 10);
				cout << "You choose 'NORMAL' game mode..." << endl;
				gotoxy(60, 11);
				cout << "In this mode you have to score 400 to encounter mafia..." << endl;
				Sleep(3000);
				loading();
				speedmod = 3;
			}
			else if (diffi == 'H' || diffi == 'h')
			{
				cout << "\033[96m";
				system("cls");
				gotoxy(60, 10);
				cout << "You choose 'HARD' game mode..." << endl;
				gotoxy(60, 11);
				cout << "In this mode you have to score 500 to encounter mafia..." << endl;
				Sleep(3000);
				loading();
				speedmod = 2;
			}

			system("cls");
			ShowMap();

			gotoxy(x, y);
			cout << "A";

			int frame = 0;

			FindPath(ex, ey, x, y);

			while (running)
			{
				switch (diffi) {
				case 'E':
					if (pts > 300) {
						encounter();
						simulateBattle('E');
						return;
					}
					break;

				case 'e':
					if (pts > 300) {
						encounter();
						simulateBattle('e');
						return;
					}
					break;

				case 'n':
					if (pts > 400) {
						encounter();
						simulateBattle('n');
						return;
					}
					break;

				case 'N':
					if (pts > 400) {
						encounter();
						simulateBattle('N');
						return;
					}
					break;

				case 'H':
					if (pts > 500) {
						encounter();
						simulateBattle('H');
						return;
					}
					break;

				case 'h':
					if (pts > 500) {
						encounter();
						simulateBattle('h');
						return;
					}
					break;

				default:
					break;
				}
				gotoxy(x, y);
				cout << " ";

				old_x = x;
				old_y = y;

				if (GetAsyncKeyState(VK_UP))
				{
					if (map[y - 1][x] == '.')
					{
						y--;
						pts++;
					}
					else if (map[y - 1][x] == ' ')
						y--;
				}
				if (GetAsyncKeyState(VK_DOWN))
				{
					if (map[y + 1][x] == '.')
					{
						y++;
						pts++;
					}
					else if (map[y + 1][x] == ' ')
						y++;
				}
				if (GetAsyncKeyState(VK_LEFT))
				{
					if (map[y][x - 1] == '.')
					{
						x--;
						pts++;
					}
					else if (map[y][x - 1] == ' ')
						x--;
				}
				if (GetAsyncKeyState(VK_RIGHT))
				{
					if (map[y][x + 1] == '.')
					{
						x++;
						pts++;
					}
					else if (map[y][x + 1] == ' ')
						x++;
				}
				if (GetAsyncKeyState(VK_ESCAPE)) {
					exit();
					return;
				}

				if (old_x != x || old_y != y)
				{
					FindPath(ex, ey, x, y);
				}

				gotoxy(x, y);
				cout << "A";

				map[ey][ex] = '.';
				gotoxy(ex, ey);
				cout << ".";

				if (frame % speedmod == 0 && walk_queue.size() != 0)
				{
					ex = walk_queue.back().x;
					ey = walk_queue.back().y;
					walk_queue.pop_back();
				}

				gotoxy(ex, ey);
				cout << "M";

				if (ex == x && ey == y)
				{
					cout << "\033[31m";
					system("cls");
					gotoxy(60, 10);
					cout << "You caught by enemy... YOU LOOSE." << endl;
					gotoxy(60, 11);
					cout << "Your Score : " << pts << endl;
					Sleep(3000);
					cout << "\033[0m";
					return;   //   when agent caught by enemy.... 
					break;
				}

				cout << "\033[36m";
				gotoxy(90, 3);
				cout << "Press Esc key to quit..." << endl;
				gotoxy(90, 5);
				cout << "Score : " << pts;
				cout << "\033[0m";
				Sleep(100);
				frame++;
			}
		}

		if (diffi == 'q' || diffi == 'Q') {
			exit();
			return;
		}
		else {
			system("cls");
			Sleep(50);
			cout << "Invalid option" << endl;
			cout << "Select again...." << endl << endl;
			Sleep(50);
		}
	}

	cin.get();
	cin.get();
	cin.get();
	cin.get();
	cin.get();
	cin.get();
	cin.get();
	cin.get();

	return;
}

void exit() {     //  function for quiting game ........
	system("cls");
	Sleep(50);
	gotoxy(75, 6);
	cout << "+==============+" << endl;
	gotoxy(75, 7);
	cout << "|  GAME  EXIT  |" << endl;
	gotoxy(75, 8);
	cout << "+==============+" << endl;
	Sleep(2000);
	return;
}

void simulateBattle(char mode) {     //   alternate function.....

	cout << endl << "Choose your ACTION......" << endl << endl;
	Sleep(3000);

	while (true) {
		system("cls");

		cout << "A --> Attack" << endl;
		cout << "D --> Defend" << endl;
		cout << "H --> Heal" << endl << endl << endl;
		gotoxy(140, 3);
		cout << "Your current hp is : " << hp << endl;
		gotoxy(140, 4);
		cout << "Current hp of Mafia is : " << mhp << endl;
		cout << "Click your reponse : ";
		cin >> choice;

		cout << endl << endl;
		if (choice == 'A' || choice == 'a') {
			temphp = hp;
			tempmhp = mhp;
			if (checker()) return;

			else {

				response(choice, mode);
				cout << "You give a damage to mafia of : " << tempmhp - mhp << endl;
				cout << "You got damage of : " << temphp - hp << endl;
			}
			if (checker()) return;
			Sleep(1300);
		}

		else if (choice == 'D' || choice == 'd') {
			temphp = hp;
			tempmhp = mhp;
			if (checker()) return;

			else {
				response(choice, mode);
				cout << "Defending....." << endl << endl;
				Sleep(1000);
				cout << "You give a damage to mafia of : " << tempmhp - mhp << endl;
				cout << "You got damage of : " << temphp - hp << endl;
			}
			if (checker()) return;
			Sleep(1300);
		}

		else if (choice == 'H' || choice == 'h') {
			temphp = hp;
			tempmhp = mhp;
			if (checker()) return;

			else {
				response(choice, mode);
				cout << "Mafia Heal of : " << mhp - tempmhp << endl;
				cout << "You heal of of : " << hp - temphp << endl;
			}
			if (checker()) return;
			Sleep(1300);

		}

		else {
			system("cls");
			Sleep(50);
			cout << "Invalid option" << endl;
			cout << "Select again...." << endl << endl;
			Sleep(50);
		}
	}
	return;
}

void response(char x, char mode) {    //   function for your response......

	if (mode == 'E' || mode == 'e')   //   if the game mode easy.....
	{
		switch (x)
		{
		case 'A':    //  attack
			mhp -= rand() % 20 + 10;
			hp -= rand() % 15 + 5;
			break;
		case 'D':   //  defend
			mhp -= rand() % 6 + 1;
			break;
		case 'H':   //  heal;
			if (hp < 50 || mhp < 50) {
				{
					if (hp < 50)
						hp += rand() % 10 + 1;
				}
				{
					if (mhp < 50)
						mhp += rand() % 10 + 1;
				}
				break;
			}
		case 'a':    //  attack
			mhp -= rand() % 20 + 10;
			hp -= rand() % 15 + 5;
			break;
		case 'd':   //  defend
			mhp -= rand() % 6 + 1;
			break;
		case 'h':   //  heal;
			if (hp < 50 || mhp < 50) {
				{
					if (hp < 50)
						hp += rand() % 10 + 1;
				}
				{
					if (mhp < 50)
						mhp += rand() % 10 + 1;
				}
				break;
			}
		}
	}

	if (mode == 'N' || mode == 'n')
	{
		switch (x)
		{
		case 'A':    //  attack
			mhp -= rand() % 20 + 10;
			hp -= rand() % 20 + 10;
			break;
		case 'D':   //  defend
			mhp -= rand() % 5 + 1;
			break;
		case 'H':   //  heal;
			if (hp < 50 || mhp < 50) {
				{
					if (hp < 50)
						hp += rand() % 10 + 1;
				}
				{
					if (mhp < 50)
						mhp += rand() % 10 + 1;
				}
				break;
			}
		case 'a':    //  attack
			mhp -= rand() % 20 + 10;
			hp -= rand() % 20 + 10;
			break;
		case 'd':   //  defend
			mhp -= rand() % 5 + 1;
			break;
		case 'h':   //  heal;
			if (hp < 50 || mhp < 50) {
				{
					if (hp < 50)
						hp += rand() % 10 + 1;
				}
				{
					if (mhp < 50)
						mhp += rand() % 10 + 1;
				}
				break;
			}
		}
	}

	if (mode == 'H' || mode == 'h')
	{
		switch (x)
		{
		case 'A':    //  attack
			mhp -= rand() % 11 + 5;
			hp -= rand() % 11 + 5;
			break;
		case 'D':   //  defend
			mhp -= rand() % 6 + 1;
			break;
		case 'H':   //  heal;
			if (hp < 50 || mhp < 50) {
				{
					if (hp < 50)
						hp += rand() % 6 + 1;
				}
				{
					if (mhp < 50)
						mhp += rand() % 5 + 1;
				}
				break;
			}
		case 'a':    //  attack
			mhp -= rand() % 11 + 5;
			hp -= rand() % 11 + 5;
			break;
		case 'd':   //  defend
			mhp -= rand() % 3 + 1;
			break;
		case 'h':   //  heal;
			if (hp < 50 || mhp < 50) {
				{
					if (hp < 50)
						hp += rand() % 6 + 1;
				}
				{
					if (mhp < 50)
						mhp += rand() % 5 + 1;
				}
				break;
			}
		}
	}
	return;
}

bool checker() {    //   FUNCTION FOR CHECKING THAT HEALTH CANT BE NEGATIVE......
	if (hp < 1 || mhp < 1) {

		if (hp < 1) {
			hp = 0;
			system("cls");
			Sleep(500);
			gotoxy(70, 10);
			cout << "Your current hp is : " << hp << endl;
			gotoxy(70, 11);
			cout << "Current hp of Mafia is : " << mhp << endl;
			Sleep(5000);
			system("cls");
			gotoxy(70, 10);
			cout << "You are DEAD...." << endl;
			Sleep(2000);
			system("cls");
			gotoxy(70, 10);
			cout << "+=================+" << endl;
			gotoxy(70, 11);
			cout << "|    You LOOSE    |" << endl;
			gotoxy(70, 12);
			cout << "+=================+" << endl;
			Sleep(4000);
			return true;
		}

		else if (mhp < 1)
		{
			mhp = 0;
			system("cls");
			Sleep(500);
			gotoxy(70, 10);
			cout << "Your current hp is : " << hp << endl;
			gotoxy(70, 11);
			cout << "Current hp of Mafia is : " << mhp << endl;
			Sleep(5000);
			system("cls");
			gotoxy(70, 10);
			cout << "You Killed Mafia...." << endl;
			Sleep(2000);
			system("cls");
			gotoxy(70, 10);
			cout << "+=================+" << endl;
			gotoxy(70, 11);
			cout << "|    You WON!!    |" << endl;
			gotoxy(70, 12);
			cout << "+=================+" << endl;
			Sleep(4000);
			return true;
		}

	}
	return false;
}

void encounter() {    //   FUNCTION TO PRINT STORY WHEN ENCOUNTER MAFIA....
	system("cls");
	gotoxy(70, 10);
	cout << "+========================+" << endl;
	gotoxy(70, 11);
	cout << "|  You Encounterd MAFIA  |" << endl;
	gotoxy(70, 12);
	cout << "+========================+" << endl;
	Sleep(1000);
	system("cls");
	loading();
	system("cls");
	cout << "Instructions : " << endl;
	cout << "Enter the Character as your response...." << endl;
	cout << "You can heal only when your health is less than 50." << endl;
	cout << "When you heal the mafia will also get healed.." << endl << endl;
}