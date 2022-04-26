#pragma once

#include "sajson.h"

#include <string_view>
using namespace std::literals;

struct FileOut
{
    inline FileOut(FILE *f) : _fp(f), _size(0) {}
    inline FileOut &operator += (std::string_view s) {
        _size += fwrite(s.data(), 1, s.size(), _fp);
        return *this;
    }
    inline FileOut &operator += (char c) {
        _size += fwrite(&c, 1, 1, _fp);
        return *this;
    }
    inline size_t size() const {
        return _size;
    }

private:
    FILE *_fp { nullptr };
    size_t _size {0 };
};

namespace sajson
{

inline std::string escape(std::string_view s) {
    std::string escaped;
    escaped.reserve(s.size() + s.size()/10); // make initial space to 10% double quotes, which seems like a lot ;)
    size_t pos { 0 };
    while(true) {
        auto n = s.find_first_of("\"\\", pos);
        if(n == std::string_view::npos)
        {
            escaped += s.substr(pos);
            break;
        }
        escaped += s.substr(pos, n - pos);
        escaped += "\\"sv;
        escaped += s[n];
        pos = n + 1; // continue after what was copied (plus the found char)
    }
    return escaped;
}

template<typename O>
inline void dump(O &o, const sajson::value &value, int indent=-1);


namespace internal
{

template<typename O>
inline void ind(O &o, int n) {
    static const char i_str[] = "  ";

    while(n-- >= 0) {
        o += i_str;
    }
}

template<typename O>
inline void dump_array(O &o, const sajson::array &arr, int indent=-1) {
    o += '[';

    if(indent != -1)
        ++indent;

    size_t idx = 0;
    for(const auto elem: arr)
    {
        if(indent > 0)
        {
            o += '\n';
            ind(o, indent);
        }
        dump(o, elem, indent);

        ++idx;
        if(idx < arr.get_length())
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
inline void dump_object(O &o, const sajson::object &obj, int indent=-1) {
    o += '{';

    if(indent != -1)
        ++indent;

    size_t idx = 0;
    for(const auto [key, value]: obj)
    {
        if(indent > 0)
        {
            o += '\n';
            ind(o, indent);
        }

        o += '"';
        o += key;
        o += '"';
        o += ':';
        if(indent >= 0)
            o += ' ';

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
        o += '"';
        o += escape(value.as_string());  // TODO: escape double-quotes (")
        o += '"';
        break;
    case TYPE_ARRAY:
        internal::dump_array(o, value.as_array(), indent);
        break;
    case TYPE_OBJECT:
        internal::dump_object(o, value.as_object(), indent);
        break;
    }
}

inline std::string to_string(const value &value, bool indent=false) {

    std::string so;
    so.reserve(65536);

    dump(so, value, indent? 0: -1);

    return so;
}

inline size_t write(FILE *f, const value &value, bool indent=false) {
    FileOut fo(f);

    dump(fo, value, indent? 0: -1);

    return fo.size();
}


}
