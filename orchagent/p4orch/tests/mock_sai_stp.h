#ifndef MOCK_SAI_STP_H
#define MOCK_SAI_STP_H

#include <gmock/gmock.h>
#include "sai.h"

// Mock class for SAI STP APIs
class MockSaiStp {
public:
    // Mock method for creating an STP instance
    MOCK_METHOD(sai_status_t, create_stp_instance,
                (sai_object_id_t *stp_instance_id, 
                 sai_object_id_t switch_id, 
                 uint32_t attr_count, 
                 const sai_attribute_t *attr_list));

    // Mock method for removing an STP instance
    MOCK_METHOD(sai_status_t, remove_stp_instance,
                (sai_object_id_t stp_instance_id));

    // Mock method for setting STP instance attributes
    MOCK_METHOD(sai_status_t, set_stp_instance_attribute,
                (sai_object_id_t stp_instance_id, 
                 const sai_attribute_t *attr));

    // Mock method for getting STP instance attributes
    MOCK_METHOD(sai_status_t, get_stp_instance_attribute,
                (sai_object_id_t stp_instance_id, 
                 uint32_t attr_count, 
                 sai_attribute_t *attr_list));

    // Mock method for creating an STP port
    MOCK_METHOD(sai_status_t, create_stp_port,
                (sai_object_id_t *stp_port_id, 
                 sai_object_id_t switch_id, 
                 uint32_t attr_count, 
                 const sai_attribute_t *attr_list));

    // Mock method for removing an STP port
    MOCK_METHOD(sai_status_t, remove_stp_port,
                (sai_object_id_t stp_port_id));

    // Mock method for setting STP port attributes
    MOCK_METHOD(sai_status_t, set_stp_port_attribute,
                (sai_object_id_t stp_port_id, 
                 const sai_attribute_t *attr));

    // Mock method for getting STP port attributes
    MOCK_METHOD(sai_status_t, get_stp_port_attribute,
                (sai_object_id_t stp_port_id, 
                 uint32_t attr_count, 
                 sai_attribute_t *attr_list));
};

// Global mock object for SAI STP APIs
extern MockSaiStp* mock_sai_stp;

// Macros to redirect SAI calls to the mock object
#define sai_create_stp_instance mock_sai_stp->create_stp_instance
#define sai_remove_stp_instance mock_sai_stp->remove_stp_instance
#define sai_set_stp_instance_attribute mock_sai_stp->set_stp_instance_attribute
#define sai_get_stp_instance_attribute mock_sai_stp->get_stp_instance_attribute
#define sai_create_stp_port mock_sai_stp->create_stp_port
#define sai_remove_stp_port mock_sai_stp->remove_stp_port
#define sai_set_stp_port_attribute mock_sai_stp->set_stp_port_attribute
#define sai_get_stp_port_attribute mock_sai_stp->get_stp_port_attribute

#endif // MOCK_SAI_STP_H

