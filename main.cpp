#include <QCoreApplication>
#include "hydraulic_controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

/*
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
*/
    return a.exec();
}
