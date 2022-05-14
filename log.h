#pragma once

#include <string>

namespace MyEngine
{
	// логгер для записи
	class Log
	{
	public:
		Log();
		~Log();
		// статичный одиночка
		static Log* Get() { return m_instance; }
		// писать сообщение
		void Print(const char* message, ...);
		// писать сообщение с префиксом DEBUG
		void Debug(const char* message, ...);
		// писать сообщение с префиксом ERROR
		void Error(const char* message, ...);

	private:
		static Log* m_instance; // сам объект
		// инициализация скрыта
		void m_init();
		// завершение
		void m_close();
		// печать сообщения
		void m_print(const char* levtext, const char* text);

		FILE* m_file; // файл куда пишем лог
	};

}