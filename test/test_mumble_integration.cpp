
#include "mumble_channelcounter.h"
#include "mumble_data.h"
#include "mumble_notify.h"
#include "test_main.h"
#include <unity.h>


#include "test_dummy.h"



class ListenBox : public EventListener
{
public:
    int msgCount = 0;
    int msgCountTHRES;
    int msgCountUNDER;
    virtual void notify(int msg) {
        msgCount++; 
        if(msg==MumbleChannelThreasholdProvider::OVEREQU){
            msgCountTHRES++;
        } else if (msg == MumbleChannelThreasholdProvider::UNDER){
            msgCountUNDER++;
        }  
    }
};

extern DummyMsgProvider dummy;

MumbleData data_i(&dummy);
MumbleProto_ChannelState c1_i;
MumbleProto_ChannelState c2_i;
MumbleProto_UserState u1_i;
MumbleProto_UserState u2_i;
MumbleProto_UserState u3_i;
MumbleProto_UserRemove r_u1_i;
MumbleProto_UserRemove r_u2_i;
MumbleProto_UserRemove r_u3_i;

ListenBox under;
ListenBox over;


void setUp_cint(void) {
    //Prepare Test Data
    c1_i.channel_id = 0;
    c1_i.has_channel_id = true;
    c1_i.has_name = true;
    strncpy(c1_i.name, "Root", 5);

    c2_i.channel_id = 1;
    c2_i.has_channel_id = true;
    c2_i.has_name = true;
    strncpy(c2_i.name, "Talk", 5);

    dummy.send(&c1_i);
    dummy.send(&c2_i);

    u1_i.session = 1;
    u1_i.has_session = true;
    strncpy(u1_i.name, "Admin", 6);
    u1_i.has_name = true;
    u1_i.has_channel_id = true;
    u1_i.channel_id = 0;

    u2_i.session = 2;
    u2_i.has_session = true;
    strncpy(u2_i.name, "User", 5);
    u2_i.has_name = true;
    u2_i.has_channel_id = true;
    u2_i.channel_id = 1;

    u3_i.session = 3;
    u3_i.has_session = true;
    strncpy(u2_i.name, "User2", 6);
    u3_i.has_name = true;
    u3_i.has_channel_id = true;
    u3_i.channel_id = 1;

    r_u1_i.session = u1_i.session;
    r_u2_i.session = u2_i.session;
    r_u3_i.session = u3_i.session;

}

void tearDown_cint(void) {
    under.msgCount = 0;
    over.msgCount = 0;
    data_i.clear();
}

void test_connect_name_ok() {
    MumbleChannelThreasholdProvider p(&data_i,"Root",2);
    TEST_ASSERT_TRUE(p.connect());
}

void test_connect_name_missing() {
    MumbleChannelThreasholdProvider p(&data_i,"Not_a_Channel",2);
    TEST_ASSERT_FALSE(p.connect());
}

void test_connect_id_allways_ok() {
    MumbleChannelThreasholdProvider p(&data_i, 1 ,2); //existing channel
    TEST_ASSERT_TRUE(p.connect());

    MumbleChannelThreasholdProvider q(&data_i, 99 ,2); //missing channel
    TEST_ASSERT_TRUE(q.connect());
}

void test_full_normal_connect_case() {
    u1_i.channel_id = c1_i.channel_id;
    u2_i.channel_id = c1_i.channel_id;
    u3_i.channel_id = c1_i.channel_id;

    MumbleChannelThreasholdProvider p(&data_i,c1_i.name,2);
    p.connect();
    p.add_Listener(&over, MumbleChannelThreasholdProvider::OVEREQU);
    p.add_Listener(&under, MumbleChannelThreasholdProvider::UNDER);


    dummy.send(&u1_i);
    TEST_ASSERT_EQUAL_INT(1, under.msgCount);
    TEST_ASSERT_EQUAL_INT(0, over.msgCount);

    dummy.send(&u2_i);
    TEST_ASSERT_EQUAL_INT(1, under.msgCount);
    TEST_ASSERT_EQUAL_INT(1, over.msgCount);

    dummy.send(&u3_i);
    TEST_ASSERT_EQUAL_INT(1, under.msgCount);
    TEST_ASSERT_EQUAL_INT(2, over.msgCount);
}

int main_cint() {
    RUN_TEST(test_connect_name_ok);
    RUN_TEST(test_connect_name_missing);
    RUN_TEST(test_connect_id_allways_ok);
    RUN_TEST(test_full_normal_connect_case);
    return 0;
}

TestClass test_it(&setUp_cint, &tearDown_cint, &main_cint);