#include "io.hpp"

IO *IO::getInstance()
{
	static IO Instance;

	return &Instance;
}

IO::IO()
{
	/* Start curses mode and data structures for stdscr*/
	initscr();

	refresh();
	/* Line buffering disabled, Pass on everything to me; except for 'ctrl + c'*/
	cbreak();
	/* Line buffering disabled, Pass on everything to me; even 'ctrl + c'*/
	// raw();
	nonl();
	// gives the flexibility of echoing characters at any place in the window without updating current (y,x). turn off with echo()
	noecho();
	// It enables the reading of function keys like F1, F2, arrow keys etc for stdscr
	keypad(stdscr, TRUE);

	curs_set(0);

	start_color();

	init_pair(1, COLOR_BLACK, COLOR_WHITE);

	init_pair(2, COLOR_WHITE, COLOR_BLUE);

	init_pair(3, COLOR_WHITE, COLOR_GREEN);

	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	init_pair(5, COLOR_BLUE, COLOR_YELLOW);

	init_pair(6, COLOR_WHITE, COLOR_RED);

	init_pair(7, COLOR_RED, COLOR_WHITE);

	init_pair(8, COLOR_YELLOW, COLOR_BLACK);

	init_pair(9, COLOR_GREEN, COLOR_BLACK);

	init_pair(10, COLOR_GREEN, COLOR_WHITE);

	init_pair(11, COLOR_RED, COLOR_BLACK);

	init_pair(12, COLOR_BLACK, COLOR_RED);
}

IO::~IO()
{
	endwin();
}

void IO::render(WINDOW *window)
{
	// refresh() checks the window and updates only the portion which has been changed
	wrefresh(window);
}

int IO::input(int to, WINDOW *window)
{
	wtimeout(window, to);
	int keypress = wgetch(window);

	return keypress;
}