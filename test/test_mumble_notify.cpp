#include <unity.h>
#include <mumble_notify.h>
#include "test_main.h"


class ListenDummy : public EventListener
{
public:
    int msgCount = 0;
    virtual void notify(int msg) {msgCount++;}
};

class ProviderWrapper : public EventProvider {
    public:
    void update(int msg) const {EventProvider::update(msg);}
};


#define MSG1 1
#define MSG2 2 
ProviderWrapper * p;
ListenDummy _l1;
ListenDummy * l1;
ListenDummy _l2;
ListenDummy * l2;

void setUp_notify(void) {
    p = new ProviderWrapper();
    l1 = &_l1;
    l2 = &_l2;

}

void tearDown_notify(void) {
    l1->msgCount = 0;
    l2->msgCount = 0;
    delete p;
}


void test_notify_match() {
    p->add_Listener(l1 , MSG1);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(1, l1->msgCount);
}

void test_notify_match_doulbe() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l1 , MSG1);
    p->update(MSG1);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(1, l1->msgCount);
}

void test_notify_match_multible() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l1 , MSG2);
    p->update(MSG1);
    p->update(MSG2);
    TEST_ASSERT_EQUAL_INT(2, l1->msgCount);
}

void test_notify_no_match() {
    p->add_Listener(l1 , MSG1);
    p->update(MSG2);
    TEST_ASSERT_EQUAL_INT(0, l1->msgCount);
}

void test_notify_2match() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l2 , MSG1);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(1, l1->msgCount);
    TEST_ASSERT_EQUAL_INT(1, l2->msgCount);

}

void test_notify_1match() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l2 , MSG2);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(1, l1->msgCount);
    TEST_ASSERT_EQUAL_INT(0, l2->msgCount);
}


void test_notify_0match() {
    p->add_Listener(l1 , MSG2);
    p->add_Listener(l2 , MSG2);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(0, l1->msgCount);
    TEST_ASSERT_EQUAL_INT(0, l2->msgCount);
}

void test_remove_listener() {
    p->add_Listener(l1 , MSG1);
    p->remove_Listener(l1);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(0, l1->msgCount);
}

void test_remove_listener_doulbe() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l1 , MSG1);
    p->remove_Listener(l1);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(0, l1->msgCount);
}


void test_remove_listener_all() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l1 , MSG2);
    p->remove_Listener(l1);
    p->update(MSG1);
    p->update(MSG2);
    TEST_ASSERT_EQUAL_INT(0, l1->msgCount);
}

void test_remove_listener_one() {
    p->add_Listener(l1 , MSG1);
    p->add_Listener(l2 , MSG1);
    p->remove_Listener(l1);
    p->update(MSG1);
    TEST_ASSERT_EQUAL_INT(1, l2->msgCount);
}





int main_notify()
{

    RUN_TEST(test_notify_match);
    RUN_TEST(test_notify_no_match);
    RUN_TEST(test_notify_2match);
    RUN_TEST(test_notify_1match);
    RUN_TEST(test_notify_0match);
    RUN_TEST(test_remove_listener);
    RUN_TEST(test_notify_match_doulbe);
    RUN_TEST(test_remove_listener_doulbe);
    RUN_TEST(test_remove_listener_all);
    RUN_TEST(test_notify_match_multible);
    RUN_TEST(test_remove_listener_one);
    
    return 0;
}

TestClass notfiyTest(&setUp_notify, &tearDown_notify, &main_notify);
