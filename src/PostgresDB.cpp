#include "PostgresDB.h"

void PostgresDB::close()
{
    if(conn)
        conn.reset();
}

bool PostgresDB::is_connect() const
{
    return PQstatus(conn.get()) == CONNECTION_OK;
}

PostgresDB::~PostgresDB()
{
    this->close();
}

PostgresDB::PostgresDB(PostgresDB&& obj) noexcept : conn(std::move(obj.conn)) {}

PostgresDB& PostgresDB::operator=(PostgresDB&& obj) noexcept
{
    if(this == &obj)
        return *this;

    conn = std::move(obj.conn);
    return *this;
}

DBexception::DBexception(std::string str) : error(std::move(str)) {}

DBexception::DBexception(const DBexception& obj) : error(obj.error) {}

const char* DBexception::what() const noexcept
{
    return error.c_str();
}

ErrorQueryResultDBexception::ErrorQueryResultDBexception(std::string str) : DBexception(std::move(str)) {}

BadConnectionDBexception::BadConnectionDBexception(std::string str) : DBexception(std::move(str)) {}

BadTypeValueDBexception::BadTypeValueDBexception(std::string str) : DBexception(std::move(str)) {}