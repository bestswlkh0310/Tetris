#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include "block.h"
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ARROW -32
#define SPACEBAR 32
#define BoardWidth 14
#define BoardHeight 22

int xPos = 0, yPos = 0;
int rotate = 0, shape = 0;
int arrBoard[BoardHeight + 1][BoardWidth + 1] = { 0 };
int arrBlock[BoardHeight + 1][BoardWidth + 1] = { 0 };
int arrLevel[BoardHeight + 1][BoardWidth + 1] = { 0 };

void GotoXY(int x, int y);
void initBoxel();
void initPos();
void printBoard();
void printBlock();
int IsCollide();
int IsSide(int to);
void Play();
void resetArrBlock();
void printLevel();
void randShape();
void clear();
void down(int y);

// �̵�
void GotoXY(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void randShape() {

	shape = rand() % 7;
}

int rotateCheck(int rot) {
	int array[BoardHeight + 1][BoardWidth + 1] = { 0 };
	for (int y = 0; y < BoardHeight; y++)
		for (int x = 0; x < BoardHeight; x++)
			array[y][x] = arrBlock[y][x];

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (Block[shape][rot][y][x] == 1)
				array[yPos + y][xPos / 2 + x] = 1;

	for (int y = 0; y < BoardHeight; y++)
	{
		if (array[y][0] == 1)
			return 1;
		if (array[y][BoardWidth - 1] == 1)
			return 1;
	}
	return 0;
}

int IsSide(int to) {
	int array[BoardHeight + 1][BoardWidth + 1] = { 0 };
	for (int y = 0; y < BoardHeight; y++)
		for (int x = 0; x < BoardHeight; x++)
		    array[y][x] = arrBlock[y][x];
	int xCopy = xPos + to, yCopy = yPos;

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			if (Block[shape][rotate][y][x] == 1)
				array[yCopy + y][xCopy / 2 + x] = 1;

	for (int y = 0; y < BoardHeight; y++)
	{
		if (array[y][0] == 1)
			return 1;
		if (array[y][BoardWidth - 1] == 1)
			return 1;
	}
	return 0;
}

void clear() {
	for (int y = BoardHeight; y >= 0; y--)
	{
		int s = 0;
		for (int x = 1; x <= 12; x++) {
			if (arrLevel[y][x] == 1) {
				s++;
			}
		}
		if (s >= 12) {
			for (int x = 1; x <= 12; x++) {
				arrLevel[y][x] = 0;
			}
			down(y);
		}
	}
}

void down(int y) {
	
	for (y; y >= 0; y--) {
		for (int x = 1; x <= 12; x++)
		{
			arrLevel[y][x] = arrLevel[y - 1][x];
		}
	}
}

unsigned _stdcall print(void* arg) {
	while (1)
	{
		resetArrBlock(); // �� ��� �迭 ����
		printBoard();
		printBlock();
		printLevel();	
		clear();
		_sleep(30);
	}
}

unsigned _stdcall inputKey(void* arg)
{
	while (1) {
		char inp;
		if (_kbhit()) {
			inp = _getch();
			if (inp == 'a')
			{
				if (!rotateCheck((rotate + 2) % 4 - 1)) {
					rotate++;
					if (rotate == 4)
						rotate = 0;
				}
			}
			if (inp == 'd') {
				if (!rotateCheck((rotate) % 4 - 1)) {
					rotate--;
					if (rotate == -1)
						rotate = 3;
				}
			}
			if (inp == ARROW)
			{
				inp = _getch();
				switch (inp)
				{
				case LEFT:
					if (IsSide(-2))
						break;
					xPos -= 2;
					break;
				case RIGHT:
					if (IsSide(2))
						break;
					xPos += 2;
					break;
				}
			}
		} else _sleep(10);
	}
	return 0;
}

// ���� �迭 �ʱ�ȭ
void initBoxel() {
	for (int y = 0; y < BoardHeight; y++)
	{
		for (int x = 0; x < BoardWidth; x++)
		{
			arrBoard[y][BoardWidth - 1] = 1;
			arrBoard[y][0] = 1;
			if (y == BoardHeight - 1) {
				arrBoard[BoardHeight - 1][x] = 1;
			}
		}
	}
}

// �ʱ� ��ǥ
void initPos() {
	xPos = 4;
	yPos = 2;
}

// �� ��� ����
void resetArrBlock() {
	for (int y = 0; y < BoardHeight; y++)
	{
		for (int x = 0; x < BoardWidth; x++)
		{
			arrBlock[y][x] = 0;
		}
	}
}

// ���� üũ (�� ��� �迭 && ���� �迭 => 1 ��ȯ)
int IsCollide() {
	for (int y = 0; y < BoardHeight; y++)
	{
		for (int x = 1; x < BoardWidth - 1; x++)
		{
			if ((arrBlock[y][x] == 1 && arrBoard[y + 1][x] == 1)
				|| (arrBlock[y][x] == 1 && arrLevel[y + 1][x] == 1)) {
				return 1;
			}
		}
	}
	return 0;
}

// ���
void printBoard() {
	// ���� ���
	GotoXY(0, 0);
	for (int y = 0; y < BoardHeight; y++)
	{
		for (int x = 0; x < BoardWidth; x++)
		{
			if (arrBoard[y][x] == 1)
			{
				printf("��");
			}
			else
			{
				printf("  ");
			}
		}
		printf("\n");
	}

	// ���� ���
	for (int y = 0; y < BoardHeight; y++)
	{
		for (int x = 0; x < BoardWidth; x++)
		{
			if (arrLevel[y][x] == 1)
			{
				GotoXY(x * 2, y);
				printf("��");
			}
		}
	}
}

// �浹 => ���� ����
void printLevel() {
	if (IsCollide())
	{
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				if (Block[shape][rotate][y][x] == 1)
					arrLevel[y + yPos][x + xPos / 2] = 1;
		randShape();
		xPos = 8;
		yPos = 0;
	}
}

// �� ��� ���
void printBlock() {
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (Block[shape][rotate][y][x] == 1)
			{
				arrBlock[yPos + y][xPos / 2 + x] = 1;
				GotoXY(xPos + 2 * x, yPos + y);
				printf("��");
			}
		}
	}
}

void Play() {
	while (1)
	{
		yPos++;
		if (yPos > 20)
			yPos = 3;
		_sleep(400);
	}
}


void main() {
	srand(time(NULL));
	initPos();
	initBoxel();
	randShape();
	printBoard();
	printBlock();
	_beginthreadex(NULL, 0, inputKey, 0, 0, NULL);
	_beginthreadex(NULL, 0, print, 0, 0, NULL);
	Play();
	GotoXY(0, 24);
}