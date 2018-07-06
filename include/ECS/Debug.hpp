// Copyright (c) 2018 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <optional>
#include <string>

namespace ecs
{
	class Debug
	{
	public:
		Debug() = delete;

		// Print info message
		static void logInfo(std::string const &message);

		// Print success message
		static void logSuccess(std::string const &message);

		// Print warning message
		static void logWarning(std::string const &message);

		// Print error message
		static void logError(std::string const &message);

		// Enable or disable colors
		static void enableColors(bool enable = true);

		// Enable or disable date time
		static void enableDateTime(bool enable = true);

	private:
		class Impl;

		// Get Debug class instance
		static Impl &get();

		// Debug class instance
		static std::optional<Impl> m_debug;
	};

	class Debug::Impl
	{
	public:
		Impl();
		~Impl() = default;

		Impl(Impl const &) = delete;
		Impl(Impl &&) noexcept = default;

		Impl &operator=(Impl const &) = delete;
		Impl &operator=(Impl &&) noexcept = default;

		// Print info message
		void logInfo(std::string const &message) const;

		// Print success message
		void logSuccess(std::string const &message) const;

		// Print warning message
		void logWarning(std::string const &message) const;

		// Print error message
		void logError(std::string const &message) const;

		// Enable or disable colors
		void enableColors(bool enable = true);

		// Enable or disable date time
		void enableDateTime(bool enable = true);

	private:
		enum class Color
		{
			Default,
			Cyan,
			Green,
			Yellow,
			Red
		};

		enum class Style
		{
			Default,
			Bold,
			Underline
		};

		// Log a message
		void log(std::string const &header, Color headerColor, std::string const &message) const;

		// Print message header to stdout
		void printHeader(std::string const &str, Color color) const;

		// Print text to stdout followed by a new line
		void printLine(std::string const &str) const;

		// Print text to stdout
		void print(std::string const &str) const;

		// Change console text style
		void format(Style style) const;

		// Change console text color
		void format(Color color) const;

		// Get current date
		std::string getDate() const;

		// Configure colors for Windows 10 console
		bool initWindowsConsole();

		// Are the colors enabled
		bool m_enableColors{ true };

		// Is the date enabled
		bool m_enableDateTime{ true };

		// Is the console configured
		static bool m_isInit;

		// Are the colors available
		static bool m_colorsAvailable;
	};
}
