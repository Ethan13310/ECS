// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#ifdef _WIN32
	// Win32 API
	#include <windows.h>
#endif

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <ECS/Debug.hpp>

// Debug class Instance
std::optional<ecs::Debug::Impl> ecs::Debug::m_debug;

void ecs::Debug::logInfo(std::string const &message)
{
	get().logInfo(message);
}

void ecs::Debug::logSuccess(std::string const &message)
{
	get().logSuccess(message);
}

void ecs::Debug::logWarning(std::string const &message)
{
	get().logWarning(message);
}

void ecs::Debug::logError(std::string const &message)
{
	get().logError(message);
}

void ecs::Debug::enableColors(bool enable)
{
	get().enableColors(enable);
}

void ecs::Debug::enableDateTime(bool enable)
{
	get().enableDateTime(enable);
}

ecs::Debug::Impl &ecs::Debug::get()
{
	if (!m_debug.has_value()) {
		m_debug = Impl{};
	}

	return m_debug.value();
}

// ecs::Debug::Impl static variables
bool ecs::Debug::Impl::m_isInit{ false };
bool ecs::Debug::Impl::m_colorsAvailable{ false };

ecs::Debug::Impl::Impl()
{
	if (!m_isInit) {
		m_colorsAvailable = initWindowsConsole();
		m_isInit = true;
	}

	m_enableColors = m_colorsAvailable;
}

void ecs::Debug::Impl::logInfo(std::string const &message) const
{
	log("info", Color::Cyan, message);
}

void ecs::Debug::Impl::logSuccess(std::string const &message) const
{
	log("success", Color::Green, message);
}

void ecs::Debug::Impl::logWarning(std::string const &message) const
{
	log("warning", Color::Yellow, message);
}

void ecs::Debug::Impl::logError(std::string const &message) const
{
	log("error", Color::Red, message);
}

void ecs::Debug::Impl::enableColors(bool enable)
{
	m_enableColors = m_colorsAvailable && enable;
}

void ecs::Debug::Impl::enableDateTime(bool enable)
{
	m_enableDateTime = enable;
}

void ecs::Debug::Impl::log(std::string const &header, Color headerColor, std::string const &message) const
{
	printHeader(header, headerColor);

	if (m_enableDateTime) {
		printHeader(getDate(), Color::Default);
	}

	printLine(message);
}

void ecs::Debug::Impl::printHeader(std::string const &str, Color color) const
{
	print("[ ");

	format(Style::Bold);
	format(color);
	print(str);

	format(Style::Default);
	print(" ] ");
}

void ecs::Debug::Impl::printLine(std::string const &message) const
{
	std::cout << message << std::endl;
}

void ecs::Debug::Impl::print(std::string const &message) const
{
	std::cout << message;
}

void ecs::Debug::Impl::format(Style style) const
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

void ecs::Debug::Impl::format(Color color) const
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

std::string ecs::Debug::Impl::getDate() const
{
	auto const now{ std::chrono::system_clock::now() };
	auto const time{ std::chrono::system_clock::to_time_t(now) };

	::tm newTime;
	::localtime_s(&newTime, &time);

	std::ostringstream ss;
	ss << std::put_time(&newTime, "%X");
	return ss.str();
}

bool ecs::Debug::Impl::initWindowsConsole()
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

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	if (!SetConsoleMode(hOut, dwMode)) {
		return false;
	}

	#endif

	return true;
}
