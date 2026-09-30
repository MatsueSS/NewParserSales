#ifndef POSTGRES_DB_H
#define POSTGRES_DB_H

//Here is the code that allows you to query the database for psql

/*
    @non-thread safety
    @note: Postgres db support work with threads - Need make for all threads connect in db
*/

#include <postgresql/libpq-fe.h>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <ranges>

using PGconnDeleter = std::function<void(PGconn*)>;
using PGresultClear = std::function<void(PGresult*)>;

using PGresultPTR = std::unique_ptr<PGresult, PGresultClear>;
using PGconnPTR = std::unique_ptr<PGconn, PGconnDeleter>;

namespace pg_detail {
    template<typename T>
    concept PreStringConcept = requires(T&& t){
        { t.c_str() } -> std::same_as<const char*>;
    } || std::convertible_to<T, const char*>;

    template<typename T>
    concept PreContainerConcept = std::ranges::range<T>;

    template<pg_detail::PreStringConcept Type>
    const char* to_cstr(Type&& T) noexcept {
        if constexpr(std::convertible_to<Type, const char*>) return T;
        else return T.c_str();
    }
}

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

    template<pg_detail::PreStringConcept Type>
    bool connect(Type&& conn);

    bool is_connect() const;

    template<pg_detail::PreStringConcept Type, pg_detail::PreStringConcept... Args>
    bool execute(Type&& query, Args&&... args) const;

    template<pg_detail::PreStringConcept Type, pg_detail::PreStringConcept... Args>
    std::vector<std::vector<std::string>> fetch(Type&& query, Args&&... args) const;

    void close();

    ~PostgresDB();

private:
    PGconnPTR conn;

    template<pg_detail::PreStringConcept... Args>
    std::vector<const char*> params_transform(Args&&... args) const;
};

template<pg_detail::PreStringConcept Type>
bool PostgresDB::connect(Type&& data)
{
    // if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
    //     throw BadTypeValueDBexception("Value must be string\n");
    // }

    const char* vquery = pg_detail::to_cstr(std::forward<Type>(data));

    PGconn* temp_conn = PQconnectdb(vquery);

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

template<pg_detail::PreStringConcept... Args>
std::vector<const char*> PostgresDB::params_transform(Args&&... args) const
{
    std::vector<const char*> n_params;
    n_params.reserve(sizeof...(Args));
    (n_params.push_back(pg_detail::to_cstr(std::forward<Args>(args))), ...);
    return n_params;
}

template<pg_detail::PreStringConcept Type, pg_detail::PreStringConcept... Args>
bool PostgresDB::execute(Type&& query, Args&&... args) const
{
    // if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
    //     throw BadTypeValueDBexception("Value must be string\n");
    // }

    const char* vquery = pg_detail::to_cstr(std::forward<Type>(query));

    if(!conn)
        throw BadConnectionDBexception("No connect\n");

    std::vector<const char*> n_params = params_transform(std::forward<Args>(args)...);

    PGresultPTR result(
        PQexecParams(conn.get(), vquery, n_params.size(), 
        nullptr, n_params.data(), nullptr, nullptr, 0), 
        [](PGresult* res){ PQclear(res); }
    );

    if(!result || PQresultStatus(result.get()) != PGRES_COMMAND_OK){
        std::string err = result ? PQresultErrorMessage(result.get()) : "Null result";
        throw ErrorQueryResultDBexception("Error query complete: " + err);
    }
    
    return PQresultStatus(result.get()) == PGRES_COMMAND_OK;
}

template<pg_detail::PreStringConcept Type, pg_detail::PreStringConcept... Args>
std::vector<std::vector<std::string>> PostgresDB::fetch(Type&& query, Args&&... args) const
{
    // if constexpr(!std::is_same<std::decay_t<Type>, std::string>::value){
    //     throw BadTypeValueDBexception("Value must be string\n");
    // }

    const char* vquery = pg_detail::to_cstr(std::forward<Type>(query));

    if(!conn)
        throw BadConnectionDBexception("No connect\n");

    std::vector<const char*> n_params = params_transform(std::forward<Args>(args)...);

    PGresultPTR result;
    if (n_params.empty()) {
        result = PGresultPTR(
            PQexec(conn.get(), vquery),
            [](PGresult* res){ PQclear(res); }
        );
    } else {
        result = PGresultPTR(
            PQexecParams(conn.get(), vquery, n_params.size(),
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

#endif // POSTGRES_DB_H