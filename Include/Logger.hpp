//
// Created by arthur on 24/09/2022.
//

#ifndef MINEPLUSPLUS_LOGGER_HPP
#define MINEPLUSPLUS_LOGGER_HPP

#include "StreamOutput.hpp"

namespace MinePP
{
	namespace Terminal::Color
	{
		static constexpr auto DEFAULT = "\x1B[0m";
		static constexpr auto RED = "\x1B[31m";
		static constexpr auto GREEN = "\x1B[32m";
		static constexpr auto YELLOW = "\x1B[33m";
		static constexpr auto BLUE = "\x1B[34m";
		static constexpr auto MAGENTA = "\x1B[35m";
		static constexpr auto CYAN = "\x1B[36m";
	}
	class Logger
	{
	public:
		enum class LoggingLevel
		{
			Debug,
			Info,
			Warning,
			Error
		};

		/**
		* @brief Log a message with the Debug level = info
		* @param message The message to Log
		*/
		template<typename T>
		static void Info(const T& message)
		{
			Log(message, LoggingLevel::Info);
		}

		/**
		* @brief Log a message with the Debug level = Debug
		* @param message The message to Log
		*/
		template<typename T>
		static void Debug(const T& message)
		{
			Log(message, LoggingLevel::Debug/*, location*/);
		}

		/**
		* @brief Log a message with the Debug level = WARNING
		* @param message The message to Log
		*/
		template<typename T>
		static void Warning(const T& message)
		{
			Log(message, LoggingLevel::Warning);
		}

		/**
		* @brief Log a message with the Debug level = ERROR
		* @param message The message to Log
		*/
		template<typename T>
		static void Error(const T& message)
		{
			Log(message, LoggingLevel::Error);
		}

		/**
		* @brief Log a message
		* @param level The level of the message
		* @param message The message to Log
		* @param location The location of the message
		*/
		template<typename T>
		static void Log(const T& message, LoggingLevel level)
		{
			switch (level)
			{
			case LoggingLevel::Debug:
				std::cout << Terminal::Color::CYAN << "[D] " << message << Terminal::Color::DEFAULT << std::endl;
				break;
			case LoggingLevel::Info:
				std::cout << Terminal::Color::GREEN << "[i] " << message << Terminal::Color::DEFAULT << std::endl;
				break;
			case LoggingLevel::Warning:
				std::cout << Terminal::Color::YELLOW << "[!] " << message << Terminal::Color::DEFAULT << std::endl;
				break;
			case LoggingLevel::Error:
				std::cerr << Terminal::Color::RED << "[X] " << message << Terminal::Color::DEFAULT << std::endl;
				break;
			}
		}
	};
}

#endif //MINEPLUSPLUS_LOGGER_HPP
