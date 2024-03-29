#pragma once

#include <string>

namespace MyEngine
{
	// ������ ��� ������
	class Log
	{
	public:
		Log();
		~Log();
		// ��������� ��������
		static Log* Get() { return m_instance; }
		// ������ ���������
		void Print(const char* message, ...);
		// ������ ��������� � ��������� DEBUG
		void Debug(const char* message, ...);
		// ������ ��������� � ��������� ERROR
		void Error(const char* message, ...);

	private:
		static Log* m_instance; // ��� ������
		// ������������� ������
		void m_init();
		// ����������
		void m_close();
		// ������ ���������
		void m_print(const char* levtext, const char* text);

		FILE* m_file; // ���� ���� ����� ���
	};

}