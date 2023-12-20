#include <QCoreApplication>
#include <QThread>
#include "hydraulic_controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HydraulicController vtHmc;
    std::string ipAddress = "192.168.127.60";
    int port = 44818; // Standard TCP port for Ethernet/IP

    // Make the connection
    vtHmc.connectDevice(ipAddress, port);

    // Vendor id
    uint16_t vendorID = 0;
    vtHmc.getVendorID(&vendorID);
    QString hexStr = QString::number(vendorID, 16).rightJustified(4, '0');
    qInfo() << "VendorID:" << ("0x" + hexStr);

    // Product name
    std::string productName = "";
    vtHmc.getProductName(&productName);
    qInfo() << "ProductName: " << QString::fromStdString(productName);

    return a.exec();
}
