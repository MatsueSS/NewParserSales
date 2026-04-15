#include "PrefixTree.h"
#include "doctest.h"

TEST_CASE("prefix tree"){
    SUBCASE("has prefix and has word for empty tree"){
        PrefixTree pt;

        CHECK(pt.has_word("1") == 0);
        CHECK(pt.has_word("1") == 0);
    }

    SUBCASE("dfs for empty tree"){
        PrefixTree pt;

        CHECK(pt.give_word_for_prefix("1") == "1");
    }

    SUBCASE("1 word"){
        PrefixTree pt;
        pt.add_word("abc");

        CHECK(pt.has_prefix("a") == 1);
        CHECK(pt.has_word("a") == 0);
        CHECK(pt.has_word("abc") == 1);
        CHECK(pt.has_prefix("abc") == 1);
        CHECK(pt.give_word_for_prefix("a") == "abc");
        CHECK(pt.give_word_for_prefix("c") == "c");
        CHECK(pt.give_word_for_prefix("abc") == "abc");
    }

    SUBCASE("russian letters"){
        PrefixTree pt;
        pt.add_word("бфы");

        CHECK(pt.has_prefix("б") == 1);
        CHECK(pt.has_word("б") == 0);
        CHECK(pt.has_word("бфы") == 1);
        CHECK(pt.has_prefix("бфы") == 1);
        CHECK(pt.give_word_for_prefix("б") == "бфы");
        CHECK(pt.give_word_for_prefix("ы") == "ы");
        CHECK(pt.give_word_for_prefix("бфы") == "бфы");
    }

    SUBCASE("2 equal word"){
        PrefixTree pt;
        pt.add_word("abc");
        pt.add_word("abc");

        CHECK(pt.has_word("abc") == 1);
        CHECK(pt.has_prefix("abc") == 1);
        CHECK(pt.has_prefix("b") == 0);
        CHECK(pt. has_prefix("ab") == 1);
        CHECK(pt.has_prefix("a") == 1);
    }

    SUBCASE("word is prefix of another word"){
        PrefixTree pt;
        pt.add_word("abc");
        pt.add_word("abcd");

        CHECK(pt.has_word("abc") == 1);
        CHECK(pt.has_word("abcd") == 1);
        CHECK(pt.has_prefix("abc") == 1);

        std::string result = pt.give_word_for_prefix("abc");
        CHECK((result == "abc" || result == "abcd") == 1);
    }

    SUBCASE("word added after its prefix"){
        PrefixTree pt;
        pt.add_word("abc");
        pt.add_word("ab");

        CHECK(pt.has_word("abc") == 1);
        CHECK(pt.has_word("abc") == 1);
        
        std::string result = pt.give_word_for_prefix("ab");
        CHECK((result == "ab" || result == "abc"));
    }

    SUBCASE("empty string"){
        PrefixTree pt;
        pt.add_word("");

        CHECK(pt.has_prefix("") == 1);
        CHECK(pt.has_word("") == 1);

        std::string result = pt.give_word_for_prefix("");
        CHECK(result == "");
    }

    SUBCASE("multiple words with same prefix"){
        PrefixTree pt;
        pt.add_word("apple");
        pt.add_word("application");
        pt.add_word("apricot");
        pt.add_word("apocalypse");

        CHECK(pt.has_word("apple") == 1);
        CHECK(pt.has_word("application") == 1);
        CHECK(pt.has_word("apocalypse") == 1);
        CHECK(pt.has_word("apricot") == 1);

        CHECK(pt.has_prefix("ap") == 1);
        std::string result = pt.give_word_for_prefix("ap");
        CHECK((result == "apple" || result == "apricot" || result == "apocalypse" || result == "application") == 1);
    }

    SUBCASE("non-existent prefix"){
        PrefixTree pt;
        pt.add_word("hello");
        pt.add_word("world");

        CHECK(pt.has_prefix("heaven") == 0);
        CHECK(pt.has_prefix("he") == 1);
        CHECK(pt.has_prefix("xyz") == 0);
    }

    SUBCASE("many words"){
        PrefixTree pt;

        for(int i = 0; i < 1000; ++i){
            pt.add_word("word_" + std::to_string(i));
        }

        for(int i = 0; i < 1000; ++i){
            CHECK(pt.has_word("word_" + std::to_string(i)) == 1);
        }

        CHECK(pt.has_prefix("word_") == 1);
        CHECK(pt.has_word("word_1000") == 0);
    }
}