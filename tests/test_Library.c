#include "Library.h"
#include <assert.h>

void stageOneTest(){
    assert(regexSearch("a", "apple") == 1);
    assert(regexSearch("z", "apple") == -1);
    assert(regexSearch("p", "apple") == 1);
    
    assert(regexSearch("d", "zombie") == -1);
    assert(regexSearch("b", "zombie") == 1);
    assert(regexSearch("l", "zombie") == -1);

}

int main() {

    // Test regexSearch
    stageOneTest();

    return 0;
}

