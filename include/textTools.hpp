#include <string>
#include <cstdint>

namespace skripler
{
    //The number of characters printed on the screen, which may be much smaller than the number of bytes when working with non-english languages.
    size_t truelength(const std::string& s);
    std::string true_substr(const std::string& s,size_t start_symbol, size_t end_symbol);

    std::string overlay(const std::string& foreground,const std::string& background);

    void makeLowercase(std::string& data);
}
