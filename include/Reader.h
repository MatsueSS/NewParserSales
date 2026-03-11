#ifndef _READER_H_
#define _READER_H_

//needed to transfer data from json objects to database objects

#include <string>
#include <chrono>

struct card{
    std::string title;
    std::string price, discount;
};

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
    void make_note(const std::string&, const std::string&, const std::string&);

    //take psql profile
    void make_a_json(const std::string&);
};

#endif //_READER_H_