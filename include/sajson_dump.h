/*
 * Copyright (c) 2012-2017 Chad Austin
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "sajson.h"

#include <cstdio>
#include <string_view>
using namespace std::literals;

/**
 * Default implementation for writing to a file.
 */
struct FileOut
{
    inline FileOut(std::FILE *f) : _fp(f), _size(0) {
        _buf.reserve(1ul << 16);
    }

    inline ~FileOut() {
        _flush();
    }

    inline FileOut &operator += (std::string_view s) {
        if(_buf.size() + s.size() > _buf.capacity())
            _flush();
        if(s.size() > _buf.capacity())
            _size += std::fwrite(s.data(), 1, s.size(), _fp);
        else
            _buf += s;
        return *this;
    }

    inline FileOut &operator += (char c) {
        if(_buf.size() + 1 > _buf.capacity())
            _flush();
        _buf += c;
        return *this;
    }

    inline size_t size() const {
        return _size;
    }

private:
    inline void _flush() {
        _size += std::fwrite(_buf.c_str(), 1, _buf.size(), _fp);
        _buf.clear();
    }

private:
    std::FILE *_fp { nullptr };
    size_t _size {0 };
    std::string _buf;
};

namespace sajson
{

template<typename O>
inline void dump(O &o, const sajson::value &value, int indent=-1);


namespace internal
{

template<typename O>
inline void ind(O &o, int n) {
    static constexpr char i_str[] = "  ";

    while(n-- > 0) {
        o += i_str;
    }
}


template<typename O>
inline void dump_string(O &o, std::string_view s) {
    o += '"';
    size_t pos { 0 };
    while(true) {
        auto n = s.find_first_of("\"\n\t\\", pos);
        if(n == std::string_view::npos)
        {
            o += s.substr(pos);
            break;
        }
        o += s.substr(pos, n - pos);
        o += "\\"sv;
        if(s[n] == '\n' || s[n] == '\t')
            o += 'n';
        else
            o += s[n];
        pos = n + 1; // continue after what was copied (plus the found char)
    }
    o += '"';
}

template<typename O>
inline void dump_array(O &o, const sajson::array &arr, int indent) {
    o += '[';

    if(indent != -1)
        ++indent;

    for(const auto [idx, elem]: arr)
    {
        if(indent != 0)
            o += '\n';
        if(indent > 0)
            ind(o, indent);

        // output the array element
        dump(o, elem, indent);

        if(idx + 1 < arr.get_length())
            o += ',';
    }

    if(indent != -1)
    {
        --indent;
        o += '\n';
    }
    if(indent > 0)
        ind(o, indent);
    o += ']';
}

template<typename O>
inline void dump_object(O &o, const sajson::object &obj, int indent) {
    o += '{';

    if(indent != -1)
        ++indent;

    size_t idx = 0;
    for(const auto [key, value]: obj)
    {
        if(indent != 0)
            o += '\n';
        if(indent > 0)
            ind(o, indent);

        // output the key
        o += '"';
        o += key;
        o += '"';
        o += ':';
        if(indent != 0)
            o += ' ';

        // output the value
        dump(o, value, indent);

        ++idx;
        if(idx < obj.get_length())
            o += ',';
    }

    if(indent != -1)
    {
        --indent;
        o += '\n';
    }
    if(indent > 0)
        ind(o, indent);
    o += '}';
}

} // NS: internal

/**
 * Dump a sajson::value to the specified output, 'o'.
 * If 'indent' is >= 0, LF and indentation will be used.
 * Otherwise, everything will be written on a single line, with no spaces.
 * Indentation size can currently not be controlled.
 */
template<typename O>
inline void dump(O &o, const sajson::value &value, int indent) {
    switch(value.get_type())
    {
    case TYPE_INTEGER:
        o += std::to_string(value.get_integer_value());
        break;
    case TYPE_DOUBLE:
        o += std::to_string(value.get_double_value());
        break;
    case TYPE_NULL:
        o += "null";
        break;
    case TYPE_FALSE:
        o += "false";
        break;
    case TYPE_TRUE:
        o += "false";
        break;
    case TYPE_STRING:
        internal::dump_string(o, value.as_string());
        break;
    case TYPE_ARRAY:
        internal::dump_array(o, value.as_array(), indent);
        break;
    case TYPE_OBJECT:
        internal::dump_object(o, value.as_object(), indent);
        break;
    }
}

/**
 * Serialize a sajson::value to a string.
 * If 'indent' is true, LF and indentation will be used.
 * Otherwise, everything will be written on a single line, with no spaces.
 * Indentation size can currently not be controlled.
 */
inline std::string to_string(const value &value, bool indent=false) {

    std::string so;
    so.reserve(65536);

    dump(so, value, indent? 0: -1);

    return so;
}

/**
 * Serialize a sajson::value to the given file pointer.
 * If 'indent' is true, LF and indentation will be used.
 * Otherwise, everything will be written on a single line, with no spaces.
 * Indentation size can currently not be controlled.
 */
inline size_t write(std::FILE *f, const value &value, bool indent=false) {
    FileOut fo(f);

    dump(fo, value, indent? 0: -1);

    return fo.size();
}

/**
 * Serialize a sajson::value to the given file path.
 * If 'indent' is true, LF and indentation will be used.
 * Otherwise, everything will be written on a single line, with no spaces.
 * Indentation size can currently not be controlled.
 */
inline size_t write(const std::string &filepath, const value &value, bool indent=false) {
    std::FILE *f = std::fopen(filepath.c_str(), "wb");
    if(f == nullptr) {
        return static_cast<size_t>(-1);
    }

    auto written = write(f, value, indent);

    std::fclose(f);

    return written;
}

}
