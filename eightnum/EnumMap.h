#pragma once
#include "stdlib.h"
#include "windows.h"
#include <vector>
#include "utils.h"
#include "defines.h"



class EnumMap
{
public:
	EnumMap();
	~EnumMap();

	void Render(const int cxClient, const int cyClient, HDC surface);
	void MoveDwon();
	void MoveUp();
	void MoveLeft ();
	void MoveRight();
	int memory[3][3];
	void MemoryRender(const int cxClient, const int cyClient, HDC surface);
	void ResetMemory();
	double TestRoute(const vector<int> &vecPath, EnumMap &memory);

private:
	int map[3][3] = { 2, 8, 6,
		4, 1, 3,
		5, 0, 7 };
	POINT zorepoint;
	int ZoreX, ZoreY;
};


