#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"
using namespace std;


class Game :public olc::PixelGameEngine
{
private:
	int N;
	int SRN;
	int** arr;
	int width;
	int height;
	olc::vf2d borders = { 2,2 };
	olc::vf2d displace = { 20,80 };
	bool firsttime = true;
	olc::vf2d size;
public:
	bool OnUserCreate() override
	{
		N = 16;
		SRN = sqrt(N);

		size = { 40,40 };

		arr = new int* [N];
		for (int i = 0; i < N; i++)
			arr[i] = new int[N];

		width = N;
		height = N;

		for (int y = 0; y < N; y++)
			for (int x = 0; x < N; x++)
				arr[y][x] = 0;


		return true;
	}
	bool OnUserUpdate(float ftime) override
	{
		
		if (firsttime)
		{
			StartingScreen();
			return true;

		}

		olc::vf2d selected = GetMousePos() - displace;


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
			BackTrackSolver(0, 0);
		}

		if (GetKey(olc::Key::R).bReleased)
			Reset();




		Clear(olc::BLACK);




		if (win() && !check())
			DrawString(120, 30, "SUDDDDOKO!", olc::GREEN, 3);
		else if (check())
			DrawString(120, 30, "SUDDDDOKO!", olc::RED, 3);
		else
			DrawString(120, 30, "SUDDDDOKO!", olc::YELLOW, 3);




		//draw squares in the background
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				FillRect(olc::vi2d(x, y) * size + borders + displace, size - 2 * borders, olc::DARK_BLUE);
				FillRect(olc::vi2d(selected.x, selected.y) * size + borders + displace, size - 2 * borders, olc::BLUE);
			}
		}


		//draw white lines  between the smaller grids
		for (int x = 0; x <= width; x += SRN)
		{
			DrawLine(olc::vf2d(x, 0) * size + displace, olc::vf2d(x, height) * size + displace);
			DrawLine(olc::vf2d(0, x) * size + displace, olc::vf2d(width, x) * size + displace);
		}


		//draw the value in the array on top of the squares
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				DrawString(olc::vi2d(x, y) * size + size / 2 + displace, to_string(arr[y][x]), olc::YELLOW,2);
			}
		}


		return true;
	}


	void StartingScreen()
	{

		bool chosen = false;

		olc::vi2d selected = GetMousePos();
		selected = selected / size;

		if (selected == olc::vi2d( width / 3,  height / 2) && GetMouse(0).bPressed)
		{
			N = 4;
			SRN = 2;
			height = N;
			width = N;
			size = { 120,120 };
			chosen = true;
		}


		if (selected == olc::vi2d(width / 2, height / 2) && GetMouse(0).bPressed)
		{
			N = 9;
			SRN = 3;
			height = N;
			width = N;
			size = { 60,60 };
			chosen = true;
		}


		if (selected == olc::vi2d((2 * width) / 3, height / 2) && GetMouse(0).bPressed)
		{
			N = 16;
			SRN = 4;
			height = N;
			width = N;
			chosen = true;
		}




		Clear(olc::DARK_BLUE);


		DrawString(olc::vf2d(size.x * width / 4, size.y * height / 3), "Choose Sudoku Size",olc::WHITE,2);
		
		FillRect(olc::vf2d(size.x * width / 3, size.y * height / 2), size);
		DrawString(olc::vf2d(size.x * width / 3, size.y * height / 2) , "4x4", olc::BLUE, 1);

		FillRect(olc::vf2d(size.x * width / 2, size.y * height / 2), size);
		DrawString(olc::vf2d(size.x * width / 2, size.y * height / 2) , "9x9", olc::BLUE, 1);

		FillRect(olc::vf2d(size.x *2* width / 3, size.y * height / 2), size);
		DrawString(olc::vf2d(size.x * 2 * width / 3, size.y * height / 2) , "16x16",olc::BLUE, 1);


		FillCircle(GetMouseX(),GetMouseY(), 10, olc::YELLOW);

		if (chosen)
			firsttime = false;
		
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
		//smaller grids	
		for (int b = 0; b < height; b += SRN)
			for (int a = 0; a < width; a += SRN)
				for (int k = b; k < SRN + b; k++)
					for (int l = a; l < SRN + a; l++)
						for (int i = b; i < SRN + b; i++)
							for (int j = a; j < SRN + a; j++)
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


		if (i == N)
			return true;



		for (int k = 1; k <= N; k++)
		{
			arr[i][j] = k;

			if (check())
				continue;

			if (j == N-1)
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
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				arr[i][j] = 0;
	}

	bool win()
	{
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (arr[i][j] == 0)
					return false;
		return true;
	}


	bool OnUserDestroy() override
	{
		for (int i = 0; i < N; i++)
			delete[] arr[i];

		delete[] arr;

		return true;

	}

};


int main(void)
{
	Game game;
	if (game.Construct(720, 760, 1, 1))
		game.Start();


}