

// Added by LTH, Lars Breum Hansen, M.Sc, March 2025.

#ifndef APPS_MEC_MECAPPS_PREDICTMECAPP_PREDICTMECAPP_H_
#define APPS_MEC_MECAPPS_PREDICTMECAPP_PREDICTMECAPP_H_

#include "apps/mec/MecApps/MecAppBase.h"

#include <inet/networklayer/common/L3Address.h>

#include "apps/mec/MecApps/MecAppBase.h"
#include <cstdio>    // For popen()
#include <iostream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>


namespace simu5g {

using namespace omnetpp;

class PredictMecApp : public MecAppBase
{

private:
    cMessage *selfMsg; // Pointer for self-message

protected:

    double predictTime; // Time to start predicting

    int numberOfApplications_;    // requests to send in this session
    cMessage *burstTimer = nullptr;
    cMessage *burstPeriod = nullptr;
    bool burstFlag;
    cMessage *sendBurst = nullptr;

    double lambda; // it is the mean, not the rate
    inet::TcpSocket *serviceSocket_ = nullptr;
    inet::TcpSocket *mp1Socket_ = nullptr;

    HttpBaseMessage *mp1HttpMessage = nullptr;
    HttpBaseMessage *serviceHttpMessage = nullptr;

    void initialize(int stage) override;

    /* Method to be implemented for real MEC apps */
    virtual void handleProcessedMessage(cMessage *msg);
    virtual void handleSelfMessage(cMessage *msg) override;
    virtual void handleServiceMessage(int connId) override;
    virtual void handleMp1Message(int connId) override;
    virtual void handleHttpMessage(int connId) override;
    virtual void handleUeMessage(cMessage *msg) override;
    virtual void established(int connId) override;

   // virtual void parsePcap();
    // Master thesis updates
    virtual std::string predictPython();
    virtual void killUe(inet::L3Address node);
    virtual std::vector<inet::L3Address> ipsToNodeIds(std::string);
    virtual std::vector<std::string> parseJsonList(std::string jsonList);






public:
    PredictMecApp();
    virtual ~PredictMecApp();
};

} /* namespace simu5g */

#endif /* APPS_MEC_MECAPPS_PREDICTMECAPP_PREDICTMECAPP_H_ */
