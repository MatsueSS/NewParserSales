#ifndef _READER_H_
#define _READER_H_

#include <string>

class Reader{
private:
    std::string clean_price(const std::string&);

public:
    Reader() = default;

    Reader(Reader&& ) noexcept = default;
    Reader(const Reader&) = default;

    Reader& operator=(Reader&&) noexcept = default;
    Reader& operator=(const Reader&) = default;

    //take psql profile 
    void make_note(std::string);
};

#endif //_READER_H_