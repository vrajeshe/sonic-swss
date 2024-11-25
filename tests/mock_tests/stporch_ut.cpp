#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stporch.h"
#include "mock_sai_stp.h"

using namespace std;
using namespace swss;

// Global mock object for SAI STP
MockSaiStp* mock_sai_stp = nullptr;

class StpOrchTest : public ::testing::Test {
protected:
    unique_ptr<StpOrch> stpOrch;

    void SetUp() override {
        // Set up mock SAI STP
        mock_sai_stp = new MockSaiStp();

        // Initialize StpOrch with mock dependencies
        vector<string> tableNames = {"STP_TABLE"};
        stpOrch = make_unique<StpOrch>(nullptr, nullptr, tableNames);

    }

    void TearDown() override {
        delete mock_sai_stp;
        mock_sai_stp = nullptr;

        stpOrch.reset();
    }
};

TEST_F(StpOrchTest, TestAddStpPort) {
    Port port;
    port.m_alias = "Ethernet0";
    port.m_port_id = 12345;
    sai_object_id_t stp_port_id = 67890;

    sai_attribute_t attrs[2] = {};
    attrs[0].id = SAI_STP_PORT_ATTR_BRIDGE_PORT_ID;
    attrs[0].value.oid = port.m_port_id;

    attrs[1].id = SAI_STP_PORT_ATTR_STATE;
    attrs[1].value.u32 = SAI_STP_PORT_STATE_FORWARDING;

    EXPECT_CALL(*mock_sai_stp, create_stp_port(_, _, 2, _))
        .WillOnce(::testing::DoAll(::testing::SetArgPointee<0>(stp_port_id),
                                    ::testing::Return(SAI_STATUS_SUCCESS)));

    bool result = stpOrch->create_stp_port(stp_port_id);

    EXPECT_TRUE(result);
}

TEST_F(StpOrchTest, TestRemoveStpPort) {
    sai_object_id_t stp_port_id = 67890;

    EXPECT_CALL(*mock_sai_stp, remove_stp_port(stp_port_id))
        .WillOnce(::testing::Return(SAI_STATUS_SUCCESS));
  
    bool result = stpOrch->removeStpPort(stp_port_id);

    EXPECT_TRUE(result);
}

TEST_F(StpOrchTest, TestUpdateMaxStpInstance) {
    uint32_t max_stp_instance = 10;
    sai_object_id_t stp_instance_id;

    sai_attribute_t attr;
    attr.id = SAI_SWITCH_ATTR_MAX_STP_INSTANCE;
    attr.value.u32 = max_stp_instance;

    EXPECT_CALL(*mock_sai_stp, create_stp_instance(_, _, _, _))
        .WillOnce(::testing::Return(SAI_STATUS_SUCCESS));

    bool result = stpOrch->updateMaxStpInstance(max_stp_instance);

    // Assert
    EXPECT_TRUE(result);
}

TEST_F(StpOrchTest, TestStpVlanFdbFlush) {
    string vlan_alias = "Vlan100";

    EXPECT_CALL(*mock_sai_stp, set_stp_instance_attribute(_, _)).Times(0);

    bool result = stpOrch->stpVlanFdbFlush(vlan_alias);

    EXPECT_TRUE(result);
}

TEST_F(StpOrchTest, TestStpVlanPortFdbFlush) {
    string port_alias = "Ethernet0";
    string vlan_alias = "Vlan100";

    EXPECT_CALL(*mock_sai_stp, set_stp_instance_attribute(_, _)).Times(0);

    bool result = stpOrch->stpVlanPortFdbFlush(port_alias, vlan_alias);

    EXPECT_TRUE(result);
}