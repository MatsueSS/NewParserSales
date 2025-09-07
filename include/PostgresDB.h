#ifndef _POSTGRES_DB_H_
#define _POSTGRES_DB_H_

#include <postgresql/libpq-fe.h>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>

using PGconnDeleter = std::function<void(PGconn*)>;
using PGresultClear = std::function<void(PGresult*)>;

using PGresultPTR = std::unique_ptr<PGresult, PGresultClear>;
using PGconnPTR = std::unique_ptr<PGconn, PGconnDeleter>;

class DBexception : public std::exception{
protected:
    std::string error;

public:
    DBexception(std::string);
    DBexception(const DBexception&);

    const char* what() const noexcept override;

};

class ErrorQueryResultDBexception : public DBexception{
public:
    ErrorQueryResultDBexception(std::string);

};

class BadConnectionDBexception : public DBexception{
public:
    BadConnectionDBexception(std::string);

};

class BadTypeValueDBexception : public DBexception{
public:
    BadTypeValueDBexception(std::string);
};

class PostgresDB{
public:
    PostgresDB() = default;

    PostgresDB(const PostgresDB&) = delete;
    PostgresDB& operator=(const PostgresDB&) = delete;

    PostgresDB(PostgresDB&&) noexcept;
    PostgresDB& operator=(PostgresDB&&) noexcept;

    template<typename Type>
    bool connect(Type&& conn);

    bool is_connect() const;

    //need pre-conect on postgres-user for make a new db
    template<typename Type>
    bool make_db(Type&& conn) const;

    template<typename Container, typename Type>
    bool execute(Type&& query, Container&& container) const;

    template<typename Container, typename Type>
    std::vector<std::vector<std::string>> fetch(Type&& query, Container&& container) const;

    // template<typename Type>
    // std::vector<std::vector<std::string>> simple_query(Type&& query) const;

    void close();

    ~PostgresDB();

private:
    PGconnPTR conn;

    template<typename Container>
    std::vector<const char*> params_transform(Container&& container) const;
};

template<typename Type>
bool PostgresDB::connect(Type&& data)
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueDBexception("Value must be string\n");
    }

    PGconn* temp_conn = PQconnectdb(data.c_str());

    if(PQstatus(temp_conn) == CONNECTION_BAD){
        PQfinish(temp_conn);
        throw BadConnectionDBexception("Cannot be connect\n");
    }

    conn = PGconnPTR(
        temp_conn, 
        [](PGconn* ptr){ PQfinish(ptr); }
    );

    return 1;
}

template<typename Type>
bool PostgresDB::make_db(Type&& data) const
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueDBexception("Value must be string\n");
    }

    if(!conn)
        throw BadConnectionDBexception("No connect\n");

    PGresultPTR result (
        PQexec(conn.get(), ("CREATE DATABASE " + data).c_str()),
        [](PGresult* res){ PQclear(res); }
    );

    if(PQresultStatus(result.get()) != PGRES_COMMAND_OK){
        throw ErrorQueryResultDBexception("Cannot be create db\n");
    }

    return 1;
}

template<typename Container>
std::vector<const char*> PostgresDB::params_transform(Container&& container) const
{
    std::vector<const char*> n_params;
    for(const auto& obj : container){
        n_params.push_back(obj.c_str());
    }
    return n_params;
}

template<typename Container, typename Type>
bool PostgresDB::execute(Type&& query, Container&& container) const
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueDBexception("Value must be string\n");
    }

    if(!conn)
        throw BadConnectionDBexception("No connect\n");

    std::vector<const char*> n_params = params_transform(std::forward<Container>(container));

    PGresultPTR result(
        PQexecParams(conn.get(), query.c_str(), n_params.size(), 
        nullptr, n_params.data(), nullptr, nullptr, 0), 
        [](PGresult* res){ PQclear(res); }
    );

    if(!result || PQresultStatus(result.get()) != PGRES_COMMAND_OK)
        throw ErrorQueryResultDBexception("Error query complete\n");
    
    return PQresultStatus(result.get()) == PGRES_COMMAND_OK;
}

template<typename Container, typename Type>
std::vector<std::vector<std::string>> PostgresDB::fetch(Type&& query, Container&& container) const
{
    if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
        throw BadTypeValueDBexception("Value must be string\n");
    }

    if(!conn)
        throw BadConnectionDBexception("No connect\n");

    std::vector<const char*> n_params = params_transform(std::forward<Container>(container));

    PGresultPTR result;
    if (n_params.empty()) {
        result = PGresultPTR(
            PQexec(conn.get(), query.c_str()),
            [](PGresult* res){ PQclear(res); }
        );
    } else {
        result = PGresultPTR(
            PQexecParams(conn.get(), query.c_str(), n_params.size(),
                        nullptr, n_params.data(), nullptr, nullptr, 0),
            [](PGresult* res){ PQclear(res); }
        );
    }
    if(!result || PQresultStatus(result.get()) != PGRES_TUPLES_OK)
        throw ErrorQueryResultDBexception("Error query complete\n");

    std::vector<std::vector<std::string>> table;

    int rows = PQntuples(result.get());
    int columns = PQnfields(result.get());

    for(int i = 0; i < rows; i++){
        std::vector<std::string> temp;
        for(int j = 0; j < columns; j++){
            temp.push_back(PQgetvalue(result.get(), i, j));
        }   
        table.emplace_back(std::move(temp));
    }

    return table;
}

#endif //_POSTGRES_DB_H_