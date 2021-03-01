#include "mumble_data.h"
#include "test_main.h"
#include <unity.h>
#include <algorithm>
#include "test_dummy.h"


DummyMsgProvider dummy;

MumbleData data(&dummy);
MumbleProto_UserState u1;
MumbleProto_UserState u2;
MumbleProto_UserRemove r_u1;
MumbleProto_ChannelState c1;
MumbleProto_ChannelState c2;



void setUp_data(void) {
    //Prepare Test Data
    c1.channel_id = 0;
    c1.has_channel_id = true;
    c1.has_name = true;
    strncpy(c1.name, "Root", 5);

    c2.channel_id = 1;
    c2.has_channel_id = true;
    c2.has_name = true;
    strncpy(c2.name, "Talk", 5);

    u1.session = 1;
    u1.has_session = true;
    strncpy(u1.name, "Admin", 6);
    u1.has_name = true;
    u1.has_channel_id = true;
    u1.channel_id = 0;

    u2.session = 2;
    u2.has_session = true;
    strncpy(u2.name, "User", 5);
    u2.has_name = true;
    u2.has_channel_id = true;
    u2.channel_id = 1;

    r_u1.session = u1.session;
}

void tearDown_data(void) {
    data.clear();
}




void test_channel_insert(void) {

    dummy.send(&c1);
    MumbleChannel _c = data.getChannel(0);

    TEST_ASSERT_EQUAL_INT(c1.channel_id, _c.id);
    TEST_ASSERT_EQUAL_INT(0, _c.userCount);
    TEST_ASSERT_EQUAL_STRING(c1.name, _c.name);
}

void test_user_insert(void) {
    
    dummy.send(&u1);

    MumbleUser _u = data.getUser(u1.session);

    TEST_ASSERT_EQUAL_INT(u1.session, _u.id);
    TEST_ASSERT_EQUAL_STRING(u1.name, _u.name);
    TEST_ASSERT_EQUAL_INT(u1.channel_id, _u.channel);
}

void test_user_not_exits(void) {
    // test min and max values
    int cases[4] = {0,1,-1,-2};
    
    for (size_t i = 0; i < 4; i++)
    {
        MumbleUser _u = data.getUser(cases[i]);
        // unknown id
        TEST_ASSERT_EQUAL_INT(-1, _u.id);
    }
}

void test_user_remove(void) {

    dummy.send(&u1);
    dummy.send(&r_u1);

    MumbleUser _u = data.getUser(u1.session);
    // unknown id
    TEST_ASSERT_EQUAL_INT(-1, _u.id);

}

void test_user_default_channel(void) {
    
    dummy.send(&c1);
    dummy.send(&u1);

    MumbleChannel _c = data.getChannel(0);

    TEST_ASSERT_EQUAL_INT(1, _c.userCount);

}

void test_channel_count_zero(){
    dummy.send(&c1);

    MumbleChannel _c = data.getChannel(c1.channel_id);
    TEST_ASSERT_EQUAL_INT(0, _c.userCount);
}

void test_channel_count_increment() {
    MumbleChannel _c;
    u1.channel_id = c1.channel_id;
    u2.channel_id = c1.channel_id;

    dummy.send(&c1);  // insert channel first

    dummy.send(&u1);     // insert first user
    
    _c = data.getChannel(c1.channel_id);
    TEST_ASSERT_EQUAL_INT(1, _c.userCount); //Check First Increment

    dummy.send(&u2);

    _c = data.getChannel(c1.channel_id); // insert second user
    TEST_ASSERT_EQUAL_INT(2, _c.userCount); //Check Second increment
}

void test_user_update_channel() {
    dummy.send(&u1);
    u1.channel_id = u1.channel_id+1;
    u1.has_name = false;
    dummy.send(&u1);

    MumbleUser _u = data.getUser(u1.session);

    TEST_ASSERT_EQUAL_INT(u1.channel_id, _u.channel);
}

void test_user_update_name() {
    dummy.send(&u1);
    strncpy(u1.name, "SuperUser", 10);
    u1.has_channel_id = false;
    dummy.send(&u1);

    MumbleUser _u = data.getUser(u1.session);

    TEST_ASSERT_EQUAL_STRING("SuperUser", _u.name);
}

void test_channel_update_name() {
    dummy.send(&c1);
    strncpy(c1.name, "Change", 7);
    dummy.send(&c1);

    MumbleChannel _u = data.getChannel(c1.channel_id);
    TEST_ASSERT_EQUAL_STRING("Change", _u.name);
}

void test_channel_update_skip_name() {

    strncpy(c1.name, "Original", 9);
    dummy.send(&c1);

    strncpy(c1.name, "Change", 7);
    c1.has_name = false;
    dummy.send(&c1);

    MumbleChannel _c = data.getChannel(c1.channel_id);
    TEST_ASSERT_EQUAL_STRING("Original", _c.name);
}

void test_find_user_empty() {
    int id = data.findUserName("Admin");

    TEST_ASSERT_EQUAL_INT(-1, id);
}

void test_find_user_not_found() {
    dummy.send(&u1);
    int id = data.findUserName("Not_A_User");
    TEST_ASSERT_EQUAL_INT(-1, id);
}

void test_find_user_found() {
    dummy.send(&u1);
    int id = data.findUserName("Admin");
    TEST_ASSERT_EQUAL_INT(u1.session, id);
}

void test_find_user_found_second() {
    dummy.send(&u1);
    dummy.send(&u2);
    int id = data.findUserName("User");
    TEST_ASSERT_EQUAL_INT(u2.session, id);
}

void test_find_channel_empty() {
    int id = data.findChannelName("Root");
    TEST_ASSERT_EQUAL_INT(-1, id);
}

void test_find_channel_not_found() {
    dummy.send(&c1);
    int id = data.findChannelName("Not_Root");
    TEST_ASSERT_EQUAL_INT(-1, id);
}

void test_find_channel_found() {
    dummy.send(&c1);
    int id = data.findChannelName("Root"); 
    TEST_ASSERT_EQUAL_INT(c1.channel_id, id);
}

void test_find_channel_found_second() {
    dummy.send(&c1);
    dummy.send(&c2);
    int id = data.findChannelName("Talk"); 
    TEST_ASSERT_EQUAL_INT(c2.channel_id, id);
}



int main_data()
{

    RUN_TEST(test_channel_insert);
    RUN_TEST(test_user_insert);
    RUN_TEST(test_user_not_exits);
    RUN_TEST(test_user_remove);
    RUN_TEST(test_user_default_channel);
    RUN_TEST(test_channel_count_zero);
    RUN_TEST(test_channel_count_increment);
    RUN_TEST(test_user_update_channel);
    RUN_TEST(test_user_update_name);
    RUN_TEST(test_channel_update_name);
    RUN_TEST(test_find_user_empty);
    RUN_TEST(test_find_user_not_found);
    RUN_TEST(test_find_user_found);
    RUN_TEST(test_find_user_found_second);
    RUN_TEST(test_find_channel_empty);
    RUN_TEST(test_find_channel_not_found);
    RUN_TEST(test_find_channel_found);
    RUN_TEST(test_find_channel_found_second);
    
    return 0;
}

TestClass dataTest(&setUp_data, &tearDown_data, &main_data);
