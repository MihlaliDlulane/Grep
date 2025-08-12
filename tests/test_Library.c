#include <assert.h>
#include "Library.h"

int main() {

    // Test regexSearch
    assert(regexSearch("a", "apple") == 1);
    assert(regexSearch("z", "apple") == -1);
    assert(regexSearch("p", "apple") == 1);

    return 0;
}