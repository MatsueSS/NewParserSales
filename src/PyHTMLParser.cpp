#include "PyHTMLParser.h"

#include "PyLoader.h"

std::vector<ProductData> PyHTMLParser::fetch_product() const
{
    PyLoader::load("bash -c 'python3 ../py_scripts/ex.py'");
    PyLoader::load("bash -c 'python3 ../py_scripts/2.py'");
    return parse_json("../sensetive_res/products.json");
}