#include <iostream>
#include <sstream>

#include "serializer.h"

#define equal(a, b) do { if (a != b) std::cout << __LINE__ << " line: error in checking for equality\n"; } while(0)

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};

int main()
{
    Data x { 1, true, 2 };

    std::stringstream s1;

    Serializer serializer(s1);
    equal(serializer.save(x), Error::NoError);

    Data y { 0, false, 0 };

    Deserializer d1(s1);
    equal(d1.load(y), Error::NoError);

    equal(x.a, y.a);
    equal(x.b, y.b);
    equal(x.c, y.c);

    y = { 0, true, 0 };

    auto s6 = std::stringstream("100 false 42");
    Deserializer d6(s6);
    equal(d6.load(y), Error::NoError);

    equal(y.a, 100);
    equal(y.b, false);
    equal(y.c, 42);
    std::cout << "Wow, it works fine!\n";
    return 0;
}
