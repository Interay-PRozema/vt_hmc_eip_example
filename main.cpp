#include <QCoreApplication>
#include <QThread>



#include "hydraulic_controller.h"

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <cstdlib>
#include <sstream>
#include <cip/connectionManager/NetworkConnectionParams.h>
#include "SessionInfo.h"
#include "MessageRouter.h"
#include "ConnectionManager.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::ConnectionManager;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if 0
    HydraulicController vtHmc;


    vtHmc.connectDevice(QHostAddress("192.168.127.60"), 44818, "10");


    // Send the command
    QString command = "TESTING\r\n";
    if(vtHmc.sendCommand(QByteArray(command.toUtf8())) != 0){
        return -1;
    }

    // Get response
    QByteArray response;
    if(vtHmc.readResponse(&response) != 0){
        return -1;
    }

    // Check if response is for valid command
    if(response.contains(">")){
        return 0; // Succes
    }

#endif



#if 1
    Logger::setLogLevel(LogLevel::DEBUG);

#if OS_Windows
    WSADATA wsaData;
    int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (winsockStart != 0) {
        Logger(LogLevel::ERROR) << "Failed to start WinSock - error code: " << winsockStart;
        return EXIT_FAILURE;
    }
#endif

    auto si = std::make_shared<SessionInfo>("192.168.127.60", 44818, std::chrono::seconds(10));
    auto messageRouter = std::make_shared<MessageRouter>();

    // Read attributes
    auto response = messageRouter->sendRequest(si, ServiceCodes::GET_ATTRIBUTE_ALL, EPath(0x01, 1, 1), {});
    if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
        Buffer buffer(response.getData());

        QStringList list;
        for (const auto &i : buffer.data()) {
            list << QString::number(i);
        }
        qInfo() << "buffer: [" << list.join(" - ") << "]";

    }
    else {
        qInfo() << "error=0x" << std::hex << response.getGeneralStatusCode();
    }
/*
    //Write attribute
    // See OpenEr EDS 160 line
    Buffer assembly151;
    assembly151 << CipUsint(1)
                << CipUsint(2)
                << CipUsint(3)
                << CipUsint(4)
                << CipUsint(5)
                << CipUsint(6)
                << CipUsint(7)
                << CipUsint(8)
                << CipUsint(9)
                << CipUsint(10);


    response = messageRouter->sendRequest(si, ServiceCodes::SET_ATTRIBUTE_SINGLE,
                                          EPath(0x04, 151, 3),
                                          assembly151.data());

    if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
        Logger(LogLevel::INFO) << "Writing is successful";
    } else {
        Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << response.getGeneralStatusCode();
    }
*/


    QThread::msleep(5000);


#if OS_Windows
    WSACleanup();
#endif

    return EXIT_SUCCESS;

#endif




    return a.exec();
}
