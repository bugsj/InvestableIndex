#pragma once

namespace InvestableIndex {

	namespace Debug {

		class DebugOutput
		{
		private:
			HANDLE m_logfile;

			DebugOutput() {
				m_logfile = ::CreateFileW(L"C:\\Users\\luojie\\Documents\\IIlog.txt", FILE_APPEND_DATA,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			}
			int writeStr(const char* log) const;
			int writeStr(const wchar_t* wlog) const;

		public:
			~DebugOutput() {
				CloseHandle(m_logfile);
			}
			static const DebugOutput& get() {
				static DebugOutput obj;
				return obj;
			}

			int writeLog(const char* log, const char* endline = "\n") const;
			int writeLog(const wchar_t* wlog, const wchar_t* wendline = L"\n") const;

		};

		inline int WriteLog(const wchar_t* wlog, const wchar_t* wendline = L"\n")
		{
			return DebugOutput::get().writeLog(wlog, wendline);
		}

		inline int WriteLog(const char* log, const char* endline = "\n")
		{
			return DebugOutput::get().writeLog(log, endline);
		}

	}

}
