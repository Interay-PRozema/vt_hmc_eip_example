/**
 * @brief	Seti
 * @brief	Hydraulic Controller - VT-HMC
 * @author	Pieter Rozema - p.rozema@interay.com
 * @date	december 2023
 */

/* Includes */

#include "hydraulic_controller.h"

/* Definitions */


/* Variables */


/* Methods */

// Constructors & Destructors
HydraulicController::HydraulicController(){

    // Create new tcp socket
    m_tcpSocket = new QTcpSocket();
    m_tcpSocket->setParent(nullptr);

    // Connections to the socket
    connect(m_tcpSocket, &QTcpSocket::connected, this, &HydraulicController::slot_deviceConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &HydraulicController::slot_deviceDisconnected);
    connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, &HydraulicController::slot_errorOccurred);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &HydraulicController::slot_readyRead);
    connect(m_tcpSocket, &QTcpSocket::bytesWritten, this, &HydraulicController::slot_bytesWritten);
}

HydraulicController::~HydraulicController(){
    disconnectDevice();
    delete m_tcpSocket;
}

void HydraulicController::connectDevice(const QHostAddress address, const quint16 port, const QString deviceAddress){

    // Check if not already connected
    if(m_connected){
        return;
    }

    // Save data in class
    m_address = address;
    m_port = port;
    m_deviceAddress = deviceAddress;

    // Attempt to connect
    m_tcpSocket->connectToHost(m_address, m_port);

    // Create local event loop to give the device to connect
    QEventLoop loop;
    connect(this, &HydraulicController::signal_deviceConnected, &loop, &QEventLoop::quit);

    QTimer timeout;
    timeout.setSingleShot(true);
    connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);

    timeout.start(CONNECTION_TIMEOUT);
    loop.exec();

    // If connection not made in time -> abort connecting
    if(timeout.remainingTime() < 50){
        m_tcpSocket->abort();
        if(m_logEnabled){
            qWarning() << "VT-HMC: Unable to connect to device with ip: " << address.toString() << " and port: " << port;
        }
    }

}

void HydraulicController::disconnectDevice(){

    // Check if connected
    if(!m_connected){
        return;
    }

    // If tcp socket exists, then disconnect
    if(m_tcpSocket != nullptr){
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->disconnect();
    }

    // Set connect to false
    m_connected = false;

}

void HydraulicController::slot_deviceConnected(){

    if(m_logEnabled){
        qInfo() << "VT-HMC: connected";
    }
    m_connected = true;
    emit signal_deviceConnected();

}

void HydraulicController::slot_deviceDisconnected(){

    if(m_logEnabled){
        qWarning() << "VT-HMC: disconnected";
    }
    m_connected = false;
    emit signal_deviceDisconnected();

}

void HydraulicController::slot_errorOccurred(QAbstractSocket::SocketError error){

    if(m_logEnabled){
        qWarning() << "VT-HMC: tcp Socket error: " << m_tcpSocket->error() << " - " << m_tcpSocket->errorString();
    }

}

void HydraulicController::slot_bytesWritten(qint64 bytes){

    m_nrOfBytesWritten = m_nrOfBytesWritten + bytes;

    if(m_nrOfBytesWritten >= m_currentCmdSize){
        emit signal_commandSend();
    }

}


int HydraulicController::sendCommand(const QByteArray command){

    // Check if tcpsocket exists and is connected
    if(m_connected == false || m_tcpSocket == nullptr){
        return -1;
    }

    // Check if message can be send
    if(m_tcpSocket->isOpen() && m_tcpSocket->isWritable()){

        // Log
        if(m_logEnabled){

            // Copy command
            QByteArray commandLog = command;

            // Replace newline and carraige return for clean log file
            commandLog.replace('\n', ' ');
            commandLog.replace('\r', ' ');

            qInfo() << "VT-HMC: command: " << QString(commandLog) << " send to: " << m_address.toString();
        }

        // Reset values for the onBytesWritten method
        m_nrOfBytesWritten = 0;
        m_currentCmdSize = command.size();

        // Write message
        m_tcpSocket->write(command);

        // Create local event loop to wait for the command to be fully send
        QEventLoop loop;
        connect(this, &HydraulicController::signal_commandSend, &loop, &QEventLoop::quit);

        QTimer timeout;
        timeout.setSingleShot(true);
        connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);

        timeout.start(1000);
        loop.exec();

        // Succes
        return 0;
    }

    // Failure
    return -1;

}

void HydraulicController::slot_readyRead(){

    m_readBuffer.append(m_tcpSocket->readAll());

    qWarning() << "Vt-HMC: readbuffer: " << m_readBuffer;

    if(m_readBuffer.contains(QString("\r").toUtf8())){
        emit signal_responseReceived();
    }

}

int HydraulicController::readResponse(QByteArray *response){

    // Clear the readbuffer in case there is still data in
    m_readBuffer.clear();

    // Check connection
    if(m_connected == false || m_tcpSocket == nullptr){
        return -1;
    }

    // Create local event loop to wait for the response to be fully received
    QEventLoop loop;
    connect(this, &HydraulicController::signal_responseReceived, &loop, &QEventLoop::quit);

    QTimer timeout;
    timeout.setSingleShot(true);
    connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);

    timeout.start(2500);
    loop.exec();

    // Log
    if(m_logEnabled){

        // Copy response
        QByteArray responseLog = m_readBuffer;

        // Replace newline and carraige return for clean log file
        responseLog.replace('\n', ' ');
        responseLog.replace('\r', ' ');

        qInfo() << "VT-HMC: response: " << QString(responseLog) << " from: " << m_address.toString();
    }

    // Return the response
    if(response != nullptr){
        *response = m_readBuffer;
    }
    return 0;
}

