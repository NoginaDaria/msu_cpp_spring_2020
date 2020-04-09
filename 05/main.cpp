#include "bigint.h"
#define equal(a, b) do { if (a != b) std::cout << __LINE__ << " line: nice to meet you, error\n"; } while(0)

int main()
{
    BigInt x = 5;
    BigInt y = 4;
    equal(x < y, false);

    std::cout << x - y << "\n";

    std::cout << "done\n";

    return 0;
}
