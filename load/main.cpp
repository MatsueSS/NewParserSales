#include <iostream>
#include <fstream>

#include "PostgresDB.h"
#include "good_funcs.h"

int main(void)
{
    PostgresDB db;
    db.connect(get_conn());
    db.execute(std::string("CREATE TABLE users (id TEXT NOT NULL PRIMARY KEY);"), std::vector<std::string>{});
    db.execute(std::string("CREATE TABLE cards (id SERIAL PRIMARY KEY, title TEXT NOT NULL, price INT, discount INT, date DATE, );"), std::vector<std::string>{});
    db.execute(std::string("CREATE TABLE preferences (id TEXT NOT NULL, preference TEXT NOT NULL, PRIMARY KEY (id, preference), FOREIGN KEY (id) P  EFERENCES users(id));"), std::vector<std::string>{});


    std::ofstream file("../.env");
}