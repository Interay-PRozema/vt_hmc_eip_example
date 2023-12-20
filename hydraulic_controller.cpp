/**
 * @brief	Seti
 * @brief	Hydraulic Controller - VT-HMC
 * @author	Pieter Rozema - p.rozema@interay.com
 * @date	december 2023
 */

/* Includes */

#include "hydraulic_controller.h"



using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::ConnectionManager;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

/* Definitions */

#define CLASS_ID_IDENTITY               0x01
#define CLASS_ID_MESSAGE_ROUTER         0x02
#define CLASS_ID_ASSEMBLY               0x04
#define CLASS_ID_CONNECTION_MANAGER     0x06
#define CLASS_ID_QOS                    0x48
#define CLASS_ID_PORT                   0xF4
#define CLASS_ID_TCP_IP_INTERFACE       0xF5
#define CLASS_ID_ETHERNET_LINK          0xF6


#define ATTRIBUTE_ID_VENDOR_ID                          1
#define ATTRIBUTE_ID_DEVICE_TYPE                        2
#define ATTRIBUTE_ID_PRODUCT_CODE                       3
#define ATTRIBUTE_ID_REVISION                           4
#define ATTRIBUTE_ID_STATUS                             5
#define ATTRIBUTE_ID_SERIALNUMBER                       6
#define ATTRIBUTE_ID_PRODUCT_NAME                       7
#define ATTRIBUTE_ID_STATE                              8
#define ATTRIBUTE_ID_CONFIGURATION_CONSISTENCY_VALUE    9
#define ATTRIBUTE_ID_HEARTBEAT_INTERVAL                 10


/* Variables */


/* Methods */

// Constructors & Destructors
HydraulicController::HydraulicController(){


#if OS_Windows
    WSADATA wsaData;
    int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (winsockStart != 0) {
        qCritical() << "Failed to start WinSock - error code: " << winsockStart;
    }
#endif


}

HydraulicController::~HydraulicController(){

#if OS_Windows
    WSACleanup();
#endif


}

void HydraulicController::connectDevice(const std::string address, const int port){

    // Keep settings
    m_address = address;
    m_port = port;

    // Create the session info and messagerouter
    si = std::make_shared<SessionInfo>(m_address, m_port);
    messageRouter = std::make_shared<MessageRouter>(false);
}

int HydraulicController::getVendorID(uint16_t *vendorId){

    // Send request
    MessageRouterResponse response = messageRouter->sendRequest(si,
                                                                ServiceCodes::GET_ATTRIBUTE_SINGLE,
                                                                EPath(CLASS_ID_IDENTITY, 1, ATTRIBUTE_ID_VENDOR_ID),
                                                                {});

    // Check response
    if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {

        // Get the data from the response
        Buffer buffer(response.getData());
        buffer >> *vendorId;

        // Success
        return 0;
    }

    // Failure
    qCritical() << "vt-hmc: Error: 0x" << std::hex << response.getGeneralStatusCode();
    return -1;
}

int HydraulicController::getProductName(std::string *productName){

    // Send request
    // Send request
    MessageRouterResponse response = messageRouter->sendRequest(si,
                                                                ServiceCodes::GET_ATTRIBUTE_SINGLE,
                                                                EPath(CLASS_ID_IDENTITY, 1, ATTRIBUTE_ID_PRODUCT_NAME),
                                                                {});
    // Check response
    if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {

        // Get the data from the response
        Buffer buffer(response.getData());

        std::string result;
        for (uint8_t c : buffer.data()) {
            result += static_cast<char>(c);
        }

        *productName = result;

        // Success
        return 0;
    }

    // Failure
    qCritical() << "vt-hmc: Error: 0x" << std::hex << response.getGeneralStatusCode();
    return -1;
}


