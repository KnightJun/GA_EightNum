#include "EnumMap.h"


int GoalNum[3][3] = { 1,4,7,
2,5,8,
3,6,0 };

EnumMap::EnumMap()
{
	zorepoint.x = 2;
	zorepoint.y = 1;
}

EnumMap::~EnumMap()
{
}

void EnumMap::Render(const int cxClient, const int cyClient, HDC surface)
{
	const int border = 20;

	int BlockSizeX = (cxClient - 2 * border) / MAP_WIDTH;
	int BlockSizeY = (cyClient - 2 * border) / MAP_HEIGHT;
	int x, y;
	string s;

	HBRUSH BlackBrush, RedBrush, OldBrush;
	HPEN NullPen, OldPen;
	HFONT OldFont;
	HFONT font = CreateFont(
		130,                                                  //   nHeight  
		75,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		FW_NORMAL,                                   //   nWeight  
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		"Î¢ÈíÑÅºÚ");
	SetBkMode(surface, TRANSPARENT);

	OldFont = (HFONT)SelectObject(surface, font);
	NullPen = (HPEN)GetStockObject(NULL_PEN);
	BlackBrush = CreateSolidBrush(0xa0adbb);
	RedBrush = CreateSolidBrush(0xc9e0ef);

	OldBrush = (HBRUSH)SelectObject(surface, BlackBrush);
	OldPen = (HPEN)SelectObject(surface, NullPen);

	Rectangle(surface, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(surface, RedBrush);
	for (y = 0; y < MAP_HEIGHT; ++y)
	{
		for ( x = 0; x < MAP_WIDTH; ++x)
		{
			if (map[x][y] == 0)continue;
			int left = border + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = border + (BlockSizeY * y);
			int bottom = top + BlockSizeY;
			
			Rectangle(surface, left, top, right, bottom);
			
			s = itos(map[x][y]);
			TextOut(surface, left, top, s.c_str(), 1);

		}
	}
	SelectObject(surface, OldBrush);

	//and pen
	SelectObject(surface, OldPen);
	SelectObject(surface, OldFont);
	DeleteObject(font);
	DeleteObject(BlackBrush);
	DeleteObject(RedBrush);
	DeleteObject(OldBrush);
	DeleteObject(OldPen);
}

void EnumMap::MoveDwon() {
	if (zorepoint.y == 0)return;
	swap(map[zorepoint.x][zorepoint.y], map[zorepoint.x][zorepoint.y-1]);
	zorepoint.y = zorepoint.y - 1;
}

void EnumMap::MoveUp() {
	if (zorepoint.y == 2)return;
	swap(map[zorepoint.x][zorepoint.y], map[zorepoint.x][zorepoint.y + 1]);
	zorepoint.y = zorepoint.y + 1;
}

void EnumMap::MoveLeft() {
	if (zorepoint.x == 2)return;
	swap(map[zorepoint.x][zorepoint.y], map[zorepoint.x+1][zorepoint.y]);
	zorepoint.x = zorepoint.x + 1;
}

void EnumMap::MoveRight() {
	if (zorepoint.x == 0)return;
	swap(map[zorepoint.x][zorepoint.y], map[zorepoint.x - 1][zorepoint.y]);
	zorepoint.x = zorepoint.x - 1;
}

void EnumMap::MemoryRender(const int cxClient, const int cyClient, HDC surface)
{

}

void EnumMap::ResetMemory()
{
	int x, y;
	for (x = 0; x < 3; ++x) {
		for ( y = 0; y < 3; y++)
		{
			memory[x][y] = 0;
		}
	}
}

double EnumMap::TestRoute(const vector<int>& vecPath, EnumMap & memory)
{
	int i, calcdiff;
	for (i = 0; i < vecPath.size(); i++)
	{
		switch (vecPath[i]) 
		{
		case 0://ÉÏ
			memory.MoveUp();
			break;
		case 1://down
			memory.MoveDwon();
			break;
		case 2://left
			memory.MoveLeft();
			break;
		case 3:
			memory.MoveRight();
			break;
		}
		int x, y;
		calcdiff = 0;
		for ( x = 0; x < 3; x++)
		{
			for (y = 0; y < 3; y++) {
				if (memory.map[x][y] != GoalNum[x][y])
				{
					calcdiff++;
				}
			}
		}
		if (calcdiff == 0) {
			// Finish
			return 1 +500 / (double)i;
		}
	}

	return 1/(double) (calcdiff+1);
}

