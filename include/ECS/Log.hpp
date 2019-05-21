// Copyright (c) 2019 Ethan Margaillan <contact@ethan.jp>.
// Licensed under the MIT Licence - https://raw.githubusercontent.com/Ethan13310/ECS/master/LICENSE

#pragma once

#include <optional>
#include <string>

namespace ecs
{
	class Log
	{
	public:
		Log() = delete;

		// Print info message
		static void info(std::string const &message);

		// Print success message
		static void success(std::string const &message);

		// Print warning message
		static void warning(std::string const &message);

		// Print error message
		static void error(std::string const &message);

		// Enable or disable colors
		static void enableColors(bool enable = true);

		// Enable or disable date time
		static void enableDateTime(bool enable = true);

		// Check whether colors are enabled
		static bool colorsEnabled() noexcept;

	private:
		class ColorsInitializer
		{
		public:
			ColorsInitializer() noexcept;
			~ColorsInitializer() = default;

			// Determine if colors can be outputted to the console
			bool available() const noexcept;

		private:
			// Configure colors for Windows 10 console
			bool initWindowsConsole() const noexcept;

			// Are colors available
			bool m_colorsAvailable{ false };
		};

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
		static void log(std::string const& header, Color headerColor, std::string const& message);

		// Print message header to stdout
		static void printHeader(std::string const& str, Color color);

		// Print text to stdout followed by a new line
		static void printLine(std::string const& str);

		// Print text to stdout
		static void print(std::string const& str);

		// Change console text style
		static void format(Style style);

		// Change console text color
		static void format(Color color);

		// Get current date
		static std::string getDate();

		// Are the colors enabled
		static inline bool m_enableColors{ true };

		// Is the date enabled
		static inline bool m_enableDateTime{ true };

		// Is the console configured
		static inline ColorsInitializer m_colors{};
	};
}
