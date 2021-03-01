#pragma once

#include <vector>


class TestClass
{
private:
    void(*setUp_Ptr)();
    void(*teardown_Ptr)();
    int(*main_ptr)();

    static std::vector<TestClass*> tests;

    static TestClass* active;

    void run() { active = this; main_ptr(); };
    TestClass();
public:
    TestClass(void(*setUp_Ptr)(), void(*teardown_Ptr)(),int(*main_ptr)() ) {
        this->teardown_Ptr = teardown_Ptr;
        this->setUp_Ptr = setUp_Ptr;
        this->main_ptr = main_ptr;
        tests.push_back(this);
    }

    static void runAll() {
        for (auto &&i : tests)
        {
            i->run();
        }
    }

    static void setUp(void) {active->setUp_Ptr();}
    static void tearDown(void) {active->teardown_Ptr();}
};

