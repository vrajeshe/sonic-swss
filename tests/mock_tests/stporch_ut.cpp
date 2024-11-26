#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define private public // make Directory::m_values available to clean it.
#include "directory.h"
#undef private
#define protected public
#include "orch.h"
#undef protected
#include "ut_helper.h"
#include "dbconnector.h"
#include "mock_orchagent_main.h"
#include "mock_sai_api.h"
#include "mock_orch_test.h"
#include "mock_table.h"
#define private public
#include "stporch.h"
#undef private
#include "mock_sai_stp.h"


namespace stporch_test
{
    using namespace std;
    using namespace swss;
    using namespace mock_orch_test;

    using ::testing::_;
    using ::testing::Return;

    // Global mock object for SAI STP
    MockSaiStp* mock_sai_stp = nullptr;

    class StpOrchTest : public MockOrchTest {
    protected:
        unique_ptr<StpOrch> stpOrch;

        void ApplyInitialConfigs()
        {
            Table port_table = Table(m_app_db.get(), APP_PORT_TABLE_NAME);
            Table vlan_table = Table(m_app_db.get(), APP_VLAN_TABLE_NAME);
            Table vlan_member_table = Table(m_app_db.get(), APP_VLAN_MEMBER_TABLE_NAME);

            auto ports = ut_helper::getInitialSaiPorts();
            port_table.set(ETHERNET0, ports[ETHERNET0]);
            port_table.set(ETHERNET4, ports[ETHERNET4]);
            port_table.set(ETHERNET8, ports[ETHERNET8]);
            port_table.set("PortConfigDone", { { "count", to_string(1) } });
            port_table.set("PortInitDone", { {} });

            vlan_table.set(VLAN_1000, { { "admin_status", "up" },
                                        { "mtu", "9100" },
                                        { "mac", "00:aa:bb:cc:dd:ee" } });
            vlan_table.set(VLAN_2000, { { "admin_status", "up" },
                                        { "mtu", "9100" },
                                        { "mac", "aa:11:bb:22:cc:33" } });
            vlan_table.set(VLAN_3000, { { "admin_status", "up" },
                                        { "mtu", "9100" },
                                        { "mac", "99:ff:88:ee:77:dd" } });
            vlan_table.set(VLAN_4000, { { "admin_status", "up" },
                                        { "mtu", "9100" },
                                        { "mac", "99:ff:88:ee:77:dd" } });
            vlan_member_table.set(
                VLAN_1000 + vlan_member_table.getTableNameSeparator() + ETHERNET0,
                { { "tagging_mode", "untagged" } });

            vlan_member_table.set(
                VLAN_2000 + vlan_member_table.getTableNameSeparator() + ETHERNET4,
                { { "tagging_mode", "untagged" } });

            vlan_member_table.set(
                VLAN_3000 + vlan_member_table.getTableNameSeparator() + ETHERNET8,
                { { "tagging_mode", "untagged" } });

            vlan_member_table.set(
                VLAN_4000 + vlan_member_table.getTableNameSeparator() + ETHERNET12,
                { { "tagging_mode", "untagged" } });


            gPortsOrch->addExistingData(&port_table);
            gPortsOrch->addExistingData(&vlan_table);
            gPortsOrch->addExistingData(&vlan_member_table);
            static_cast<Orch *>(gPortsOrch)->doTask();

            // Initialize StpOrch with mock dependencies
            vector<string> tableNames = {"STP_TABLE"};
            stpOrch = make_unique<StpOrch>(nullptr, nullptr, tableNames);
        }
        void PostSetUp() override
        {
            mock_sai_stp = new MockSaiStp();
        }
        void PreTearDown() override
        {
            delete mock_sai_stp;
            mock_sai_stp = nullptr;
            stpOrch.reset();
        }
    };

    TEST_F(StpOrchTest, TestAddRemoveStpPort) {
        Port port;
        sai_uint16_t stp_instance = 1;
        sai_object_id_t stp_port_oid = 67890;
        bool result;

        ASSERT_TRUE(gPortsOrch->getPort(ETHERNET0, port));

        EXPECT_CALL(*mock_sai_stp, 
            create_stp_port(_, _, 2, _)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>(stp_port_oid),
                                        ::testing::Return(SAI_STATUS_SUCCESS)));

        result = stpOrch->updateStpPortState(port, stp_instance, STP_STATE_FORWARDING);

        ASSERT_TRUE(result);

        EXPECT_CALL(*mock_sai_stp, 
            remove_stp_port(stp_port_oid)).WillOnce(::testing::Return(SAI_STATUS_SUCCESS));

        result = stpOrch->removeStpPort(port, stp_instance);

        ASSERT_TRUE(result);
    }
}
#if 0
    TEST_F(StpOrchTest, TestRemoveStpPort) {
        Port port;
        sai_uint16_t stp_instance = 1;

        ASSERT_TRUE(gPortsOrch->getPort(ETHERNET0, port));
        EXPECT_CALL(*mock_sai_stp, 
            remove_stp_port(_)).WillOnce(::testing::Return(SAI_STATUS_SUCCESS));
    
        bool result = stpOrch->removeStpPort(port, stp_instance);

        EXPECT_TRUE(result);
    }
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
#endif