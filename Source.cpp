#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"
using namespace std;

class Game :public olc::PixelGameEngine
{
private:
	int arr[9][9] = { 0 };
	int width = 9;
	int height = 9;
	olc::vf2d borders = { 2,2 };
	olc::vf2d displace = { 20,80 };
	bool wow = true;
	olc::vf2d size = { 50,50 };
public:
	bool OnUserCreate() override
	{
		return true;
	}
	bool OnUserUpdate(float ftime) override
	{

		olc::vf2d selected = GetMousePos()-displace;

		selected = selected / size;

		if (selected.x > 0 && selected.x < width && selected.y > 0 && selected.y < height)
		{
			if (GetKey(olc::Key::NP1).bReleased)
				arr[int(selected.y)][int(selected.x)] = 1;
			else if (GetKey(olc::Key::NP2).bReleased)
				arr[int(selected.y)][int(selected.x)] = 2;
			else if (GetKey(olc::Key::NP3).bReleased)
				arr[int(selected.y)][int(selected.x)] = 3;
			else if (GetKey(olc::Key::NP4).bReleased)
				arr[int(selected.y)][int(selected.x)] = 4;
			else if (GetKey(olc::Key::NP5).bReleased)
				arr[int(selected.y)][int(selected.x)] = 5;
			else if (GetKey(olc::Key::NP6).bReleased)
				arr[int(selected.y)][int(selected.x)] = 6;
			else if (GetKey(olc::Key::NP7).bReleased)
				arr[int(selected.y)][int(selected.x)] = 7;
			else if (GetKey(olc::Key::NP8).bReleased)
				arr[int(selected.y)][int(selected.x)] = 8;
			else if (GetKey(olc::Key::NP9).bReleased)
				arr[int(selected.y)][int(selected.x)] = 9;
			else if (GetKey(olc::Key::NP0).bReleased)
				arr[int(selected.y)][int(selected.x)] = 0;
		}
		
 		if (GetKey(olc::Key::SPACE).bReleased)
		{
			BackTrackSolver(0,0);
		}

		if (GetKey(olc::Key::R).bReleased)
			Reset();
	
		Clear(olc::BLACK);

		if (win() && !check())
			DrawString(120,30, "SUDDDDOKO!", olc::GREEN,3);
		else if (check())
			DrawString(120,30, "SUDDDDOKO!", olc::RED,3);
		else
			DrawString(120,30, "SUDDDDOKO!", olc::YELLOW,3);

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{	
				FillRect(olc::vi2d(x, y) * size + borders + displace, size - 2 * borders, olc::DARK_BLUE);
				FillRect(olc::vi2d(selected.x, selected.y) * size + borders + displace, size - 2 * borders, olc::BLUE);
			}
		}

		for (int x = 0; x <= width; x += 3)
		{
			DrawLine(olc::vf2d(x, 0) * size + displace, olc::vf2d(x, height) * size + displace);
			DrawLine(olc::vf2d(0, x) * size + displace, olc::vf2d(width, x) * size + displace);
		}


		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				DrawString(olc::vi2d(x, y) * size + size / 2 + displace, to_string(arr[y][x]), olc::YELLOW);
			}
		}


		return true;
	}


	bool check()
	{

		//check repeating
		//columns
		for (int k = 0; k < width; k++)
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = i + 1; j < height; j++)
					if (arr[i][k] == arr[j][k] && arr[i][k] != 0)
						return true;
			}
		}
		//rows
		for (int k = 0; k < height; k++)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = i + 1; j < width; j++)
					if (arr[k][i] == arr[k][j] && arr[k][i] != 0)
						return true;
			}
		}
		//grids	
		for (int b = 0;b<height;b+=3)
			for (int a = 0;a<width;a+=3)
				for (int k = b;k<3+b;k++)
					for (int l = a;l<3+a;l++)
						for (int i = b; i < 3+b; i++)
							for (int j = a; j < 3+a; j++)
							{
								if (i == k && j == l)
									continue;
								if (arr[i][j] == arr[k][l] && arr[k][l] != 0)
									return true;
							}


		return false;

		
	}

	bool BackTrackSolver(int i, int j)
	{
		

		if (i == 9)
			return true;

	

		for (int k = 1; k <= 9; k++)
		{
			arr[i][j] = k;

			if (check())
				continue;

			if (j == 8)
			{
				if (BackTrackSolver(i + 1, 0))
					return true;
			}
			else
			{

				if (BackTrackSolver(i, j + 1))
					return true;
			}

		}

		arr[i][j] = 0;
		return false;

	}

	void Reset()
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				arr[i][j] = 0;
	}

	bool win()
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (arr[i][j] == 0)
					return false;
	}

};


int main(void)
{
	Game game;
	if (game.Construct(500, 540, 1, 1))
		game.Start();
	

}