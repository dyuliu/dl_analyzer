
#pragma once

#include <iostream>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define COUT_R(x) ( std::cout << analyzer::_red  	<< (x) << analyzer::_white )
#define COUT_Y(x) ( std::cout << analyzer::_yellow  << (x) << analyzer::_white )
#define COUT_B(x) ( std::cout << analyzer::_blue	<< (x) << analyzer::_white )
#define COUT_G(x) ( std::cout << analyzer::_green	<< (x) << analyzer::_white )

// GREEN - normal running
#define COUT_READ ( std::cout << analyzer::_green << "[ READ ]  " << analyzer::_white )
#define COUT_WRIT ( std::cout << analyzer::_green << "[ WRIT ]  " << analyzer::_white )
#define COUT_RUNN ( std::cout << analyzer::_green << "[ RUNN ]  " << analyzer::_white )
#define COUT_SUCC ( std::cout << analyzer::_green << "[ SUCC ]  " << analyzer::_white )

// YELLOW - warning
#define COUT_WARN ( std::cout << analyzer::_yellow << "[ WARN ]  " << analyzer::_white )
#define COUT_METD ( std::cout << analyzer::_yellow << "[ METD ]  " << analyzer::_white )

// BLUE - some display
#define COUT_CHEK ( std::cout << analyzer::_blue << "[ CHEK ]  " << analyzer::_white )

// RED - error
#define COUT_ERRO ( std::cout << analyzer::_red << "[ ERRO ]  " << analyzer::_white )


namespace analyzer {

	inline std::ostream& _blue(std::ostream &s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
			| FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& _red(std::ostream &s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& _green(std::ostream &s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& _yellow(std::ostream &s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& _white(std::ostream &s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return s;
	}

	struct color {
		color(WORD attribute) :m_color(attribute) {};
		WORD m_color;
	};

	template <class _Elem, class _Traits>
	std::basic_ostream<_Elem, _Traits>&
		operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, c.m_color);
		return i;
	}

}