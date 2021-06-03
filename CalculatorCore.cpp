#include "CalculatorCore.h"
#include <sstream>
#include <string>

enum{ TK_EOF, TK_CONST, TK_PLUS, TK_MINUS, TK_MULT, TK_DIV, TK_POW, TK_LPAREN, TK_RPAREN };

LPTSTR g_pbuf;
RR g_value;
int g_back;
int g_error;

int get_token()
{
	if (g_back >= 0){ int ret = g_back; g_back = -1; return ret; }
	while (isspace(*g_pbuf))g_pbuf++;
	switch (*g_pbuf)
	{
	case TEXT('\0'):return TK_EOF;
	case TEXT('+'):g_pbuf++; return TK_PLUS;
	case TEXT('-'):g_pbuf++; return TK_MINUS;
	case TEXT('*'):g_pbuf++; return TK_MULT;
	case TEXT('/'):g_pbuf++; return TK_DIV;
	case TEXT('^'):g_pbuf++; return TK_POW;
	case TEXT('('):g_pbuf++; return TK_LPAREN;
	case TEXT(')'):g_pbuf++; return TK_RPAREN;
	default:
		{
			const int nSize = WideCharToMultiByte(CP_ACP, 0, g_pbuf, -1, NULL, 0, NULL, NULL);
			LPSTR lpStr = (LPSTR)GlobalAlloc(GMEM_FIXED, nSize);
			WideCharToMultiByte(CP_ACP, 0, g_pbuf, -1, lpStr, nSize, NULL, NULL);
			g_value = to_RR(lpStr);
			g_pbuf += strcspn(lpStr, "+-*/()^");
			GlobalFree(lpStr);
		}
		return TK_CONST;
	}
}

//
// �v�f
//
RR prim()
{
	RR val;
	const int token = get_token();
	switch (token)
	{
	case TK_CONST:return g_value;
	case TK_MINUS:return -prim();
	case TK_LPAREN:val = expr(); get_token(); return val;
	default:return to_RR(0);
	}
}

//
// ���q
//
RR factor()
{
	RR val = prim();          // �ŏ��̊�{�v�f
	for (;;)
	{
		const int token = get_token();
		switch (token)
		{
		case TK_POW:val = power(val, to_long(prim())); break;
		default:g_back = token; return val;
		}
	}
}

//
// �揜
//
RR term()
{
	RR val1 = factor(), val2;
	g_back = get_token();
	for (;;)
	{
		const int token = get_token();
		switch (token)
		{
		case TK_MULT:
			val1 *= factor();
			break;
		case TK_DIV:
			val2 = factor();
			g_back = get_token();
			if (val2 != 0)
			{
				val1 /= val2;
			}
			else
			{
				val1 = 1;
				g_error = -1;//0�ŏ��Z
			}
			break;
		default:
			g_back = token;
			return val1;
		}
	}
}

//
// ����
//
RR expr()
{
	RR val = term();
	for (;;)
	{
		const int token = get_token();
		switch (token)
		{
		case TK_PLUS:val += term(); break;
		case TK_MINUS:val -= term(); break;
		default:g_back = token; return val;
		}
	}
}

//
// ������̒����i10�i���j
//
long RR_len(const RR& n)
{
	long i;
	RR num;
	num = abs(n);
	for (i = 0; num != 0; i++)
	{
		num /= 10;
	}
	return (n>0) ? i : i + 1;
}

//
// �]���ȕ�������菜��
//
std::string trim(const std::string& string, const char* trimCharacterList = "0./")
{
	std::string result;

	// ��������g�������镶���ȊO��������ʒu���������܂��B
	std::string::size_type left = string.find_first_not_of(trimCharacterList);

	if (left != std::string::npos)
	{
		// ��������g�������镶���ȊO�����������ꍇ�́A�����悤�ɉE��������������܂��B
		std::string::size_type right = string.find_last_not_of(trimCharacterList);

		// �߂�l�����肵�܂��B�����ł͉E�����猟�����Ă��A�g�������镶���ȊO���K�����݂���̂Ŕ���s�v�ł��B
		result = string.substr(left, right - left + 1);
	}

	return result;
}

//
// ������֕ϊ�
//
LPTSTR RRtoString(const RR& r)
{
	if (IsZero(r))
	{
		LPTSTR p = (LPTSTR)GlobalAlloc(GMEM_FIXED, sizeof(TCHAR)* 2);
		p[0] = TEXT('0');
		p[1] = 0;
		return p;
	}
	else
	{
		r.SetOutputPrecision(1000);
		std::ostringstream stream;
		stream << r;
		std::string result = stream.str();
		trim(result);
		const int nSize = (int)result.size();
		LPSTR lpszText = (LPSTR)GlobalAlloc(0, nSize + 1);
		lstrcpyA(lpszText, result.c_str());

		const DWORD len = MultiByteToWideChar(CP_ACP, 0, lpszText, -1, 0, 0);
		LPWSTR pwsz = (LPWSTR)GlobalAlloc(0, len*sizeof(WCHAR));
		MultiByteToWideChar(CP_ACP, 0, lpszText, -1, pwsz, len);

		GlobalFree(lpszText);

		return pwsz;
	}
}

//
// ���Z�b�g
//
void Reset(LPTSTR *lpszText)
{
	*lpszText = (LPTSTR)GlobalReAlloc(*lpszText, sizeof(TCHAR), GMEM_MOVEABLE);
	*lpszText[0] = 0;
}

//
// �������ǉ�
//
void AddText(LPTSTR *lpszText, LPTSTR lpszAddText)
{
	const DWORD dwSize = (DWORD)GlobalSize(*lpszText);
	*lpszText = (LPTSTR)GlobalReAlloc(*lpszText, dwSize + sizeof(TCHAR)*lstrlen(lpszAddText), GMEM_MOVEABLE);
	lstrcat(*lpszText, lpszAddText);
}

//
// �v�Z���̏�����
//
void InitValue(LPTSTR lpszText)
{
	g_pbuf = lpszText;
	g_back = -1;
	g_error = 0;
}

BOOL IsError()
{
	return (g_error != 0);
}