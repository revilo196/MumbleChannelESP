#include "test_main.h"
#include <unity.h>


std::vector<TestClass*> TestClass::tests = std::vector<TestClass*>();
TestClass* TestClass::active = nullptr;


void tearDown(void) {
    TestClass::tearDown();
}

void setUp(void) {
    TestClass::setUp();
}

int main()
{
    UNITY_BEGIN();
    TestClass::runAll();
    UNITY_END();
}


void setup() {main();}
void loop() {}