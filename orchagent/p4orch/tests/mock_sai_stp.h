#ifndef MOCK_SAI_STP_H
#define MOCK_SAI_STP_H

#include <gmock/gmock.h>
extern "C"
{
#include "sai.h"
}
// Mock class for SAI STP APIs
class MockSaiStp {
public:
    // Mock method for creating an STP instance
    MOCK_METHOD4(create_stp,
                sai_status_t(_Out_ sai_object_id_t *stp_instance_id, 
                             _In_ sai_object_id_t switch_id, 
                             _In_ uint32_t attr_count, 
                             _In_ const sai_attribute_t *attr_list));

    // Mock method for removing an STP instance
    MOCK_METHOD1(remove_stp, sai_status_t(_In_ sai_object_id_t stp_instance_id));

    // Mock method for setting STP instance attributes
    MOCK_METHOD2(set_stp_attribute,
                sai_status_t(_In_ sai_object_id_t stp_instance_id, 
                             _In_ const sai_attribute_t *attr));

    // Mock method for getting STP instance attributes
    MOCK_METHOD3(get_stp_attribute,
                sai_status_t(_Out_ sai_object_id_t stp_instance_id, 
                             _In_ uint32_t attr_count, 
                             _In_ sai_attribute_t *attr_list));

    // Mock method for creating an STP port
    MOCK_METHOD4(create_stp_port,
                sai_status_t(_Out_ sai_object_id_t *stp_port_id, 
                             _In_ sai_object_id_t switch_id, 
                             _In_ uint32_t attr_count, 
                             _In_ const sai_attribute_t *attr_list));

    // Mock method for removing an STP port
    MOCK_METHOD1(remove_stp_port,
                sai_status_t(_In_ sai_object_id_t stp_port_id));

    // Mock method for setting STP port attributes
    MOCK_METHOD2(set_stp_port_attribute,
                sai_status_t(_Out_ sai_object_id_t stp_port_id, 
                             _In_ const sai_attribute_t *attr));

    // Mock method for getting STP port attributes
    MOCK_METHOD3(get_stp_port_attribute,
                sai_status_t(_Out_ sai_object_id_t stp_port_id, 
                             _In_ uint32_t attr_count, 
                             _In_ sai_attribute_t *attr_list));
};

// Global mock object for SAI STP APIs
extern MockSaiStp* mock_sai_stp;

// Macros to redirect SAI calls to the mock object
#define create_stp mock_sai_stp->create_stp
#define remove_stp mock_sai_stp->remove_stp
#define set_stp_attribute mock_sai_stp->set_stp_attribute
#define get_stp_attribute mock_sai_stp->get_stp_attribute
#define create_stp_port mock_sai_stp->create_stp_port
#define remove_stp_port mock_sai_stp->remove_stp_port
#define set_stp_port_attribute mock_sai_stp->set_stp_port_attribute
#define get_stp_port_attribute mock_sai_stp->get_stp_port_attribute

#endif // MOCK_SAI_STP_H

