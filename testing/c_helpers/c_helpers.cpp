#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

#include "../../c_helpers.hpp"

using std::string;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::cout;
using std::cin;

template<typename F>
static void Bench(const string& label, F func, size_t iters = 10000000)
{
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < iters; ++i) { func(); }

    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    cout << label << ": " << diff.count() << " s\n";
}

int main()
{
    const char* src = "Hello World!";
    char buffer[128]{};
    string s1 = "Hello";
    string s2 = "World";

    //checks

    Bench("kh_empty", [&] {
        volatile bool res = kh_empty("");
        });
    Bench("string empty", [&] {
        string tmp;
        volatile bool res = tmp.empty();
        });

    Bench("kh_len", [&] {
        volatile size_t len = kh_len(src);
        });
    Bench("string length", [&] {
        volatile size_t len = s1.length();
        });

    Bench("kh_starts", [&] {
        volatile bool res = kh_starts("HelloWorld", "Hello");
        });
    Bench("string starts_with", [&] {
        string tmp = "HelloWorld";
        volatile bool res = tmp.compare(0, 5, "Hello") == 0;
        });

    Bench("kh_ends", [&] {
        volatile bool res = kh_ends("HelloWorld", "World");
        });
    Bench("string ends_with", [&] {
        string tmp = "HelloWorld";
        volatile bool res = tmp.size() >= 5 &&
            tmp.compare(tmp.size() - 5, 5, "World") == 0;
        });

    //comparisons

    Bench("kh_equals", [&] {
        volatile bool res = kh_equals("Hello", "Hello");
        });
    Bench("string ==", [&] {
        volatile bool res = (s1 == "Hello");
        });

    Bench("kh_iequals", [&] {
        volatile bool res = kh_iequals("Hello", "hello");
        });
    Bench("string iequals", [&] {
        string a = "Hello", b = "hello";
        volatile bool res = equal(a.begin(), a.end(), b.begin(), b.end(),
            [](char c1, char c2) { return tolower((unsigned char)c1) == tolower((unsigned char)c2); });
        });

    Bench("kh_nequals", [&] {
        volatile bool res = kh_nequals("Hello", "Helium", 3);
        });

    Bench("kh_niequals", [&] {
        volatile bool res = kh_niequals("Hello", "heLLo", 5);
        });

    //allocation and ownership

    Bench("kh_set", [&] {
        char* dst = nullptr;
        kh_set(dst, src);
        kh_free(dst);
        });
    Bench("string assign", [&] {
        string tmp;
        tmp = src;
        });

    Bench("kh_dup", [&] {
        char* dup = kh_dup(src);
        kh_free(dup);
        });
    Bench("string copy ctor", [&] {
        string tmp(src);
        });

    Bench("kh_free", [&] {
        char* dup = kh_dup(src);
        kh_free(dup);
        });
    Bench("string clear", [&] {
        string tmp = src;
        tmp.clear();
        });

    //copy and append

    Bench("kh_copy", [&] {
        kh_copy(buffer, src, sizeof(buffer));
        });
    Bench("strncpy_s", [&] {
        strncpy_s(buffer, sizeof(buffer), src, _TRUNCATE);
        });

    Bench("kh_cat", [&] {
        kh_copy(buffer, "Hello", sizeof(buffer));
        kh_cat(buffer, " World", sizeof(buffer));
        });
    Bench("string +=", [&] {
        string tmp = s1;
        tmp += " World";
        });

    //search and cleanup

    Bench("kh_fchar", [&] {
        volatile const char* res = kh_fchar("Hello World", ' ');
        });
    Bench("string find", [&] {
        string tmp = "Hello World";
        volatile size_t pos = tmp.find(' ');
        });

    Bench("kh_lchar", [&] {
        volatile const char* res = kh_lchar("a/b/c/file.txt", '/');
        });
    Bench("string rfind", [&] {
        string tmp = "a/b/c/file.txt";
        volatile size_t pos = tmp.rfind('/');
        });

    Bench("kh_trim", [&] {
        char tmp[64];
        kh_copy(tmp, "   hello world   ", sizeof(tmp));
        kh_trim(tmp);
        });
    Bench("string trim (manual)", [&] {
        string tmp = "   hello world   ";
        tmp.erase(tmp.begin(), find_if(tmp.begin(), tmp.end(), [](int ch) { return !isspace(ch); }));
        tmp.erase(find_if(tmp.rbegin(), tmp.rend(), [](int ch) { return !isspace(ch); }).base(), tmp.end());
        });

    Bench("kh_remove", [&] {
        char tmp[64];
        kh_copy(tmp, "a b c d", sizeof(tmp));
        kh_remove(tmp, ' ');
        });

    Bench("kh_aremove", [&] {
        char tmp[64];
        kh_copy(tmp, "a b c d", sizeof(tmp));
        kh_aremove(tmp, ' ');
        });

    //modification

    Bench("kh_tolower", [&] {
        char tmp[64];
        kh_copy(tmp, "HELLO", sizeof(tmp));
        kh_tolower(tmp);
        });
    Bench("string tolower", [&] {
        string tmp = "HELLO";
        transform(tmp.begin(), tmp.end(), tmp.begin(),
            [](unsigned char c) { return tolower(c); });
        });

    Bench("kh_toupper", [&] {
        char tmp[64];
        kh_copy(tmp, "hello", sizeof(tmp));
        kh_toupper(tmp);
        });
    Bench("string toupper", [&] {
        string tmp = "hello";
        transform(tmp.begin(), tmp.end(), tmp.begin(),
            [](unsigned char c) { return toupper(c); });
        });

    Bench("kh_replace", [&] {
        char tmp[64];
        kh_copy(tmp, "a_b_c", sizeof(tmp));
        kh_replace(tmp, '_', '-');
        });

    Bench("kh_areplace", [&] {
        char tmp[64];
        kh_copy(tmp, "a_b_c", sizeof(tmp));
        kh_areplace(tmp, '_', '-');
        });

    cin.get();

    return 0;
}