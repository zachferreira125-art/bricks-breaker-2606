#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.clear();
	for (int i = 0; i < 5; i++)
	{
		Box b;
		b.width = 10;
		b.height = 2;
		b.x_position = i * 13;
		b.y_position = 5;
		b.doubleThick = true;
		b.color = ConsoleColor::DarkGreen;
		bricks.push_back(b);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
    gameOver = false;
	playerWon = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// display win or lose message
	if (gameOver)
	{
		if (playerWon)
			Console::WordWrap(15, 15, 30, "You win! Press 'R' to play again.");
		else
			Console::WordWrap(15, 15, 30, "You lose. Press 'R' to play again.");
	}

	// TODO #3 - Update render to render all bricks
	// draw all the bricks in the vector
	for (const Box& b : bricks)
		b.Draw();

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	// check collision for every brick in the vector
	for (int i = 0; i < bricks.size(); i++)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - remove brick after 3 hits (color reaches black)
			if (bricks[i].color == ConsoleColor::Black)
				bricks.erase(bricks.begin() + i);

			break; // only hit one brick per frame
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	// win condition
	if (bricks.empty())
	{
		ball.moving = false;
		gameOver = true;
		playerWon = true;
	}


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	// lose condition
	if (ball.y_position >= WINDOW_HEIGHT)
	{
		ball.moving = false;
		gameOver = true;
		playerWon = false;
	}
}
