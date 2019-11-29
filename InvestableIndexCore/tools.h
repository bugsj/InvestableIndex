#pragma once

namespace InvestableIndex {

	int fromExcelDate(int serialdate);
	int toExcelDate(int digitdate);
	
	namespace Tools {

		template<class T>
		long long nulltermlen(const T* s)
		{
			long long n = 0;
			while (*s++ != static_cast<T>(0)) {
				++n;
			}
			return n;
		}

		template<class T>
		const T* appendStr(std::vector<T>* str, const std::initializer_list<const T*>& ss)
		{
			long long s0len = str->empty() ? 0 : nulltermlen(str->data());
			long long n = ss.size();
			std::vector<size_t> sslen(n);
			auto ss_i = ss.begin();
			for (auto& sslen_i : sslen) {
				sslen_i = nulltermlen(*ss_i++);
			}
			str->resize(std::accumulate(sslen.begin(), sslen.end(), s0len) + 1);
			ss_i = ss.begin();
			for (auto& sslen_i : sslen) {
				::CopyMemory(str->data() + s0len, *ss_i++, sizeof(**ss_i) * sslen_i);
				s0len += sslen_i;
			}
			str->data()[s0len] = static_cast<T>(0);
			return str->data();
		}

		template<class T>
		inline const T* appendStr(std::vector<T>* str, const T* s1)
		{
			return appendStr(str, { s1 });
		}

		const wchar_t* convMBCS2UNICODE(std::vector<wchar_t> *dst, const char *src);
		const char* convUNICODE2MBCS(std::vector<char>* dst, const wchar_t* src);

	}

}
