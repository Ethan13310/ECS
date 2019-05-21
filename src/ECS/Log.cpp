// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

// std::localtime
#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
	// Win32 API
	#include <windows.h>
#endif

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <ECS/Log.hpp>

void ecs::Log::info(std::string const &message)
{
	log("info", Color::Cyan, message);
}

void ecs::Log::success(std::string const &message)
{
	log("success", Color::Green, message);
}

void ecs::Log::warning(std::string const &message)
{
	log("warning", Color::Yellow, message);
}

void ecs::Log::error(std::string const &message)
{
	log("error", Color::Red, message);
}

void ecs::Log::enableColors(bool enable)
{
	m_enableColors = m_colors.available() && enable;
}

void ecs::Log::enableDateTime(bool enable)
{
	m_enableDateTime = enable;
}

bool ecs::Log::colorsEnabled() noexcept
{
	return m_enableColors;
}

void ecs::Log::log(std::string const &header, Color headerColor, std::string const &message)
{
	if (m_enableDateTime) {
		printHeader(getDate(), Color::Default);
	}

	printHeader(header, headerColor);
	printLine(message);
}

void ecs::Log::printHeader(std::string const &str, Color color)
{
	print("[ ");

	format(Style::Bold);
	format(color);
	print(str);

	format(Style::Default);
	print(" ] ");
}

void ecs::Log::printLine(std::string const &message)
{
	std::cout << message << std::endl;
}

void ecs::Log::print(std::string const &message)
{
	std::cout << message;
}

void ecs::Log::format(Style style)
{
	if (!m_enableColors) {
		return;
	}

	switch (style) {
	case Style::Bold:
		std::cout << "\033[1m";
		break;

	case Style::Underline:
		std::cout << "\033[4m";
		break;

	default:
		std::cout << "\033[0m";
	}
}

void ecs::Log::format(Color color)
{
	if (!m_enableColors) {
		return;
	}

	switch (color) {
	case Color::Cyan:
		std::cout << "\033[96m";
		break;

	case Color::Green:
		std::cout << "\033[92m";
		break;

	case Color::Yellow:
		std::cout << "\033[93m";
		break;

	case Color::Red:
		std::cout << "\033[91m";
		break;

	default:
		std::cout << "\033[39m";
	}
}

std::string ecs::Log::getDate()
{
	auto const now{ std::chrono::system_clock::now() };
	auto const time{ std::chrono::system_clock::to_time_t(now) };

	std::ostringstream ss;
	ss << std::put_time(std::localtime(&time), "%X");
	
	return ss.str();
}

ecs::Log::ColorsInitializer::ColorsInitializer() noexcept :
	m_colorsAvailable{ initWindowsConsole() }
{}

bool ecs::Log::ColorsInitializer::initWindowsConsole() const noexcept
{
	// Windows only:
	// Enable console colors
	#ifdef _WIN32

	HANDLE const hOut{ GetStdHandle(STD_OUTPUT_HANDLE) };

	if (hOut == INVALID_HANDLE_VALUE) {
		return false;
	}

	DWORD dwMode{ 0 };

	if (!GetConsoleMode(hOut, &dwMode)) {
		return false;
	}

	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		// Fix build error with MinGW
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	if (!SetConsoleMode(hOut, dwMode)) {
		return false;
	}

	#endif

	return true;
}

bool ecs::Log::ColorsInitializer::available() const noexcept
{
	return m_colorsAvailable;
}
