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

// Log class Instance
std::optional<ecs::Log::Impl> ecs::Log::m_log;

void ecs::Log::info(std::string const &message)
{
	get().info(message);
}

void ecs::Log::success(std::string const &message)
{
	get().success(message);
}

void ecs::Log::warning(std::string const &message)
{
	get().warning(message);
}

void ecs::Log::error(std::string const &message)
{
	get().error(message);
}

void ecs::Log::enableColors(bool enable)
{
	get().enableColors(enable);
}

void ecs::Log::enableDateTime(bool enable)
{
	get().enableDateTime(enable);
}

ecs::Log::Impl &ecs::Log::get()
{
	if (!m_log.has_value()) {
		m_log = Impl{};
	}

	return m_log.value();
}

// ecs::Log::Impl static variables
bool ecs::Log::Impl::m_isInit{ false };
bool ecs::Log::Impl::m_colorsAvailable{ false };

ecs::Log::Impl::Impl()
{
	if (!m_isInit) {
		m_colorsAvailable = initWindowsConsole();
		m_isInit = true;
	}

	m_enableColors = m_colorsAvailable;
}

void ecs::Log::Impl::info(std::string const &message) const
{
	log("info", Color::Cyan, message);
}

void ecs::Log::Impl::success(std::string const &message) const
{
	log("success", Color::Green, message);
}

void ecs::Log::Impl::warning(std::string const &message) const
{
	log("warning", Color::Yellow, message);
}

void ecs::Log::Impl::error(std::string const &message) const
{
	log("error", Color::Red, message);
}

void ecs::Log::Impl::enableColors(bool enable)
{
	m_enableColors = m_colorsAvailable && enable;
}

void ecs::Log::Impl::enableDateTime(bool enable)
{
	m_enableDateTime = enable;
}

void ecs::Log::Impl::log(std::string const &header, Color headerColor, std::string const &message) const
{
	printHeader(header, headerColor);

	if (m_enableDateTime) {
		printHeader(getDate(), Color::Default);
	}

	printLine(message);
}

void ecs::Log::Impl::printHeader(std::string const &str, Color color) const
{
	print("[ ");

	format(Style::Bold);
	format(color);
	print(str);

	format(Style::Default);
	print(" ] ");
}

void ecs::Log::Impl::printLine(std::string const &message) const
{
	std::cout << message << std::endl;
}

void ecs::Log::Impl::print(std::string const &message) const
{
	std::cout << message;
}

void ecs::Log::Impl::format(Style style) const
{
	if (m_enableColors) {
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
}

void ecs::Log::Impl::format(Color color) const
{
	if (m_enableColors) {
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
}

std::string ecs::Log::Impl::getDate() const
{
	auto const now{ std::chrono::system_clock::now() };
	auto const time{ std::chrono::system_clock::to_time_t(now) };

	std::ostringstream ss;
	ss << std::put_time(std::localtime(&time), "%X");
	return ss.str();
}

bool ecs::Log::Impl::initWindowsConsole()
{
	// Windows only:
	// Enable console colors
	#ifdef _WIN32

	HANDLE hOut{ GetStdHandle(STD_OUTPUT_HANDLE) };

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
