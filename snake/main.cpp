#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}

char tmp_map[18][32];
char map[18][32] = {
	"+-----------------------------+",
	"|                             |",
	"|                             |",
	"|-- ----------- --   ---------|",
	"|   |                         |",
	"| | |--- |  |           |     |",
	"| |      |  | |---  |   |  |  |",
	"| | -----|  | |      -- |     |",
	"| |           |---  |      |  |",
	"| |----- ---         --       |",
	"|          ------  ------- ---|",
	"|                             |",
	"|- --- ----      ---   -------|",
	"|                             |",
	"|                             |",
	"|                             |",
	"|                             |",
	"+-----------------------------+"
};

void ShowMap()
{
	for (int i = 0; i < 18; i++) {
		cout << (map[i]) << endl;
	}
}

void ShowPacMan(int x, int y) {
	gotoxy(x, y);
	cout << "@";
}

void ShowGhost(int x, int y) {
	gotoxy(x, y);
	cout << "G";
}

void ClearLastMove(int x, int y) {
	gotoxy(x, y);

	cout << " ";
}

void ShowDots(int x, int y) {
	gotoxy(x, y);
	cout << ".";
}

void ShowPoints(int x, int y, int points) {
	gotoxy(x, y);
	cout << "Punkty: " << points;

}

void ShowMenu(int x, int y) {
	gotoxy(x, y);
	cout << "@ - Pacman";
	gotoxy(x, y + 1);
	cout << "G - Ghost";
	gotoxy(x, y + 2);
	cout << "Strzalki - Poruszanie pacmanem";
	gotoxy(x, y + 3);
	cout << "ESC - Zakoncz gre";
}

void EndGame(int x, int y, int points) {
	system("cls");
	gotoxy(x, y);
	cout << "Zdobyles/as " << points << " punktow";
	getchar();
}

struct walk {
	short walk_count;
	short x;
	short y;
	short back;
};

struct target {
	short x;
	short y;
};

vector<target> Generate_dots;

vector<walk> Moving;

void AddDot(int x, int y, int back) {
	if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.') {
		tmp_map[y][x] = '|';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.back = back;
		Moving.push_back(tmp);
	}
}

void FindPath(int sx, int sy, int x, int y) {
	memcpy(tmp_map, map, sizeof(map));
	Moving.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	Moving.push_back(tmp);

	for (int i = 0; i<Moving.size(); i++) {
		if (Moving[i].x == x && Moving[i].y == y) {
			Generate_dots.clear();
			target tmp2;
			while (Moving[i].walk_count != 0) {
				tmp2.x = Moving[i].x;
				tmp2.y = Moving[i].y;
				Generate_dots.push_back(tmp2);
				i = Moving[i].back;
			}
			break;
		}

		AddDot(Moving[i].x + 1, Moving[i].y, i);
		AddDot(Moving[i].x - 1, Moving[i].y, i);
		AddDot(Moving[i].x, Moving[i].y + 1, i);
		AddDot(Moving[i].x, Moving[i].y - 1, i);
	}

	Moving.clear();
}

void SetupGame(int x, int y, int ex, int ey) {
	ShowMap();
	ShowPacMan(x, y);
	FindPath(ex, ey, x, y);
}

int main()
{
	bool running = true;
	int x = 15;
	int y = 16;
	int old_x;
	int old_y;
	int ex = 1;
	int ey = 1;
	int points = 0;
	int speedmod = 3;
	int frame = 0;

	SetupGame(x, y, ex, ey);

	while (running) {
		ClearLastMove(x, y);

		old_x = x;
		old_y = y;

		if (GetAsyncKeyState(VK_UP)) {
			if (map[y - 1][x] == '.') { y--; points++; }
			else
				if (map[y - 1][x] == ' ') y--;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			if (map[y + 1][x] == '.') { y++; points++; }
			else
				if (map[y + 1][x] == ' ') y++;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			if (map[y][x - 1] == '.') { x--; points++; }
			else
				if (map[y][x - 1] == ' ') x--;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			if (map[y][x + 1] == '.') { x++; points++; }
			else
				if (map[y][x + 1] == ' ') x++;
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			return 0;
		}
		if (old_x != x || old_y != y) {
			FindPath(ex, ey, x, y);
		}

		ShowPacMan(x, y);
		map[ey][ex] = '.';
		ShowDots(ex, ey);

		if (frame%speedmod == 0 && Generate_dots.size() != 0) {
			ex = Generate_dots.back().x;
			ey = Generate_dots.back().y;
			Generate_dots.pop_back();
		}
		ShowGhost(ex, ey);
		if (ex == x && ey == y) {
			break;
		}

		ShowMenu(32, 1);
		ShowPoints(32, 8, points);
		Sleep(100);
		frame++;
	}

	EndGame(20, 5, points);
}