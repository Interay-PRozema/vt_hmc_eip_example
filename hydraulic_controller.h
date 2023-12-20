/**
 * @brief	Seti
 * @brief	Hydraulic Controller - VT-HMC
 * @author	Pieter Rozema - p.rozema@interay.com
 * @date	december 2023
 */

#ifndef HYDRAULIC_CONTROLLER_H
#define HYDRAULIC_CONTROLLER_H

/* Includes */

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QPointer>
#include <QEventLoop>
#include <QTimer>

/* Class */

class HydraulicController : public QObject
{

    Q_OBJECT

    public:

        // Constructors & Destructors
        HydraulicController();
        ~HydraulicController();

        // Connection
        void connectDevice(const QHostAddress address, const quint16 port, const QString deviceAddress);
        void disconnectDevice();

        bool connected(){
            return m_connected;
        }

        // Logging
        void setLogging(bool logEnabled){
            m_logEnabled = logEnabled;
        }

        int sendCommand(const QByteArray command);
        int readResponse(QByteArray *response);


    private:
        QPointer<QTcpSocket> m_tcpSocket = nullptr;

        const int CONNECTION_TIMEOUT = 2500;

        bool m_logEnabled = true;

        QByteArray m_readBuffer = QByteArrayLiteral("");
        qint64 m_nrOfBytesWritten = 0;
        qint64 m_currentCmdSize = 0;

    protected:
        bool m_connected = false;

        QHostAddress m_address;
        quint16 m_port = 0;
        QString m_deviceAddress = "";



    public slots:

    private slots:

        void slot_deviceConnected();
        void slot_deviceDisconnected();

        void slot_errorOccurred(QAbstractSocket::SocketError error);

        void slot_readyRead();
        void slot_bytesWritten(qint64 bytes);

    signals:

        void signal_deviceConnected();
        void signal_deviceDisconnected();

        void signal_commandSend();
        void signal_responseReceived();

};

#endif // HYDRAULIC_CONTROLLER_H
