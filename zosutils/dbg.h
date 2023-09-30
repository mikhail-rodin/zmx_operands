#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

#ifdef _DEBUG
    #define __BASENAME__ basename(__FILE__)
    #define DBG( x ) Dbg{__BASENAME__, __LINE__} << x << Dbg::endl
#else
    #define DBG( x ) do {} while(0)
#endif //DEBUG

class Dbg {
public:
    Dbg(const char* file, int line)
    {
        // 1. when constructed, first print the error position...
        buf.str(""); // start with a clean buffer
        buf <<  file << ", line " << line << ": ";
    }
    template<typename T>
    Dbg& operator<<(T&& t)
    {
        // 2. ...then relay what's streamed to the buffer
        buf << t;
        return *this;
    }
    Dbg& operator<<(Dbg& (*pFn)(Dbg&))
    {
        // calls stream manipulators
        return (*pFn)(*this);
    }
    static Dbg& endl(Dbg& stream)
    {
        stream.flush();
        return stream;
    }
private:
    static std::ostringstream buf;
    void flush()
    {
        OutputDebugStringA(buf.str().c_str());
        buf.str(""); // emptying the buffer
    }
};
std::ostringstream Dbg::buf;

template<size_t Len>
constexpr const char* basename_impl(const char(&str)[Len], size_t pos)
{
    return pos == 0 ? str : (str[pos] == '/' || str[pos] == '\\') ? str + pos + 1 : basename_impl(str, --pos);
}

template<size_t Len>
constexpr const char* basename(const char(&str)[Len])
{
    return basename_impl(str, Len - 1);
}