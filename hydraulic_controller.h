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

// If a windows system, include the winsock library
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

/* Class */

class HydraulicController : public QObject
{
    Q_OBJECT

    public:
        // Constructors & Destructors
        HydraulicController();
        ~HydraulicController();

        // Connection
        void connectDevice(const std::string address, const int port);

        // Logging
        void setLogging(bool logEnabled){
            m_logEnabled = logEnabled;
        }

        // Methods
        int getVendorID(uint16_t *vendorId);
        int getProductName(std::string *productName);

    private:
        std::string m_address;
        int m_port = 0;

        std::shared_ptr<eipScanner::SessionInfo> si;
        std::shared_ptr<eipScanner::MessageRouter> messageRouter;

        bool m_logEnabled = true;

    protected:


    public slots:

    private slots:

    signals:
        void signal_deviceConnected();
        void signal_deviceDisconnected();

};


#endif // HYDRAULIC_CONTROLLER_H
