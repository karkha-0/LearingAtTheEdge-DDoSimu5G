

// Added by LTH, Lars Breum Hansen, M.Sc, March 2025.

#include "apps/mec/MecApps/PredictMecApp/PredictMecApp.h"

#include <string>

#include <inet/common/TimeTag_m.h>
#include <inet/common/packet/chunk/BytesChunk.h>

#include <inet/common/ProtocolTag_m.h>
#include <inet/common/ProtocolGroup.h>
#include <inet/common/Protocol.h>

#include "apps/mec/MecApps/packets/ProcessingTimeMessage_m.h"
#include "nodes/mec/utils/MecCommon.h"
#include "nodes/mec/utils/httpUtils/httpUtils.h"

#include <ctime>
#include <iostream>

namespace simu5g {

using namespace inet;
Define_Module(PredictMecApp);

PredictMecApp::PredictMecApp() {
    cancelAndDelete(sendBurst);
    cancelAndDelete(burstPeriod);
    cancelAndDelete(burstTimer);
    //cancelAndDelete(selfMsg);

}

void PredictMecApp::handleServiceMessage(int connId) {
    EV << "handleServiceMessage in PredictMecApp" << endl;
}

void PredictMecApp::initialize(int stage) {
    if (stage != inet::INITSTAGE_APPLICATION_LAYER)
        return;
    MecAppBase::initialize(stage);
    std::cout << "PredictMecApp initialized in stage: " << stage << endl;
    mp1Socket_ = addNewSocket();
    cMessage *m = new cMessage("connectMp1");
    sendBurst = new cMessage("sendBurst");
    burstPeriod = new cMessage("burstPeriod");
    burstTimer = new cMessage("burstTimer");
    burstFlag = false;
    lambda = par("lambda").doubleValue();
    mecAppId = getId();

    // Master thesis updates
    double predictTime = par("startPredictTime"); // get the predictTime from the NED file
    // Create a self-message
    selfMsg = new cMessage("selfMsg");

    // Master thesis updates
    scheduleAt(simTime() + predictTime, selfMsg);

}


// function that runs a model
// Master thesis updates
std::string PredictMecApp::predictPython(){

    std::array<char, 128> buffer;
    std::string result;

    // python3 mohak-cnn.py --predict_live ./processed_Bonesi-Simu5gBenign/10-bots.pcap  --model ./output/10t-10n-DOS2019-mohak.h5

    // the predictionOffset is the time in the PCAP file the analysis should start.
    int predictionOffset = (int)simTime().dbl()-10;

    std::cout << "Prediction Offset: " << predictionOffset << endl;


    // Open pipe to execute Python script
    //std::string command = "python3 python/model.py";
    //std::string command = "python3 python/Lucid-Mohak-Own/Lucid/lucid_cnn.py --predict_live results/PCAP-output-predict/localRouter.pcap --model python/Lucid-Mohak-Own/Lucid/output/10t-10n-DOS2019-LUCID.h5 --prediction_offset " + std::to_string(predictionOffset);
    //std::string command = "python3 python/Lucid-Mohak-Own/OwnModel/logistic-model.py --predict_live  results/PCAP-output-predict/localRouter.pcap --model python/Lucid-Mohak-Own/OwnModel/own-logistic-model.h5 --prediction_offset " + std::to_string(predictionOffset);
   std::string command = "python3 python/Lucid-Mohak-Own/Mohak/mohak-cnn.py --predict_live results/PCAP-output-predict/localRouter.pcap --model python/Lucid-Mohak-Own/Mohak/output/10t-10n-DOS2019-Mohak.h5 --prediction_offset " + std::to_string(predictionOffset);
    std::cout << command << endl;

    std::time_t predictStartTime = std::time(0);   // get time now


    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read output
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

//    std::string create_command = "rm results/PCAP-output-predict/*.pcap";
//    system(create_command.c_str());
    std::time_t predictEndTime = std::time(0);   // get time now

    std::cout << "Time to predict: " << predictEndTime-predictStartTime << endl;

    return result;
}

// A function that takes a jsonList, cleans it, and returns the elements as strings
std::vector<std::string> PredictMecApp::parseJsonList(std::string jsonList) {

    std::string cleaned = jsonList;
    cleaned.erase(
        std::find_if(cleaned.rbegin(), cleaned.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(),
        cleaned.end()
    );

    // Step 2: Parse JSON and collect strings
    std::vector<std::string> result;
    auto parsed = nlohmann::json::parse(cleaned);
    for (const auto& item : parsed) {
        result.push_back(item.get<std::string>());
    }

    return result;
}

// A function that takes a string formatted as a JSON list object and translates it into a list of NodeIds
std::vector<L3Address> PredictMecApp::ipsToNodeIds(std::string ipList) {

    std::vector<std::string> ipVector = parseJsonList(ipList);
    inet::L3AddressResolver resolver;

    std::vector<L3Address> nodeIdsVector = resolver.resolve(ipVector);

    for(L3Address address : nodeIdsVector) {
        cModule *mod = resolver.findHostWithAddress(address);
        //std::cout << mod << endl;
    }

    return nodeIdsVector;
}

void PredictMecApp::killUe(L3Address node) {

   // std::cout << node << endl;

    Ipv4Address ipv4Addr = node.toIpv4();

    L3AddressResolver resolver;
    cModule *module = resolver.findHostWithAddress(ipv4Addr);


    std::string modulePath = module->getFullPath() + ".app[0]";
    std::cout << modulePath << endl;

    if(modulePath.find("Ue") == std::string::npos) { // if not ue
        return;
    }

    cModule *appModule = findModuleByPath(modulePath.c_str());

    // Dynamically update parameters
    appModule->par("messageLength").setIntValue(8);
    appModule->par("sendInterval").setDoubleValue(1000000);

    // Notify the module to reinitialize these parameters
    cMessage *updateMsg = new cMessage("updateParams");
    sendDirect(updateMsg, appModule, "controlIn");
   // std::cout << "Scheduled updateParams message for module: " << appModule << std::endl;
    //delete updateMsg;


}
// End of Master thesis updates


void PredictMecApp::handleProcessedMessage(cMessage *msg) {
    EV << "ProcessMessage received in PredictMecApp" << endl;
        delete msg;
}

void PredictMecApp::handleSelfMessage(cMessage *msg) {
    EV << "SelfMessage received in PredictMecApp" << endl;


    std::cout << "Self message received at time: " << simTime() << endl;

    if(simTime() < predictTime) {
        EV << "Not time to predict yet" << endl;
        delete msg;
        return;
    }

    EV << "Running Python script..." << endl;

    std::string output = predictPython();

    // if the output is not empty, we shold translate the IPs to nodeIDs
    // and kill those UEs
    // Master thesis updates
    std::cout << "Predictions: " << output << endl;
    if(output.empty()) {
        std::cout << "Nothing predicted" << endl;
        return;
    }

    // translate the output (IPs) to NodeIds
    std::vector<L3Address> nodesToKill = ipsToNodeIds(output);

    for(L3Address nodeId : nodesToKill) {

        killUe(nodeId);
    }


    EV << endl;

    //delete msg;
    scheduleAt(simTime() + 15, msg); //reschedule 100s later
}




void PredictMecApp::handleMp1Message(int connId) {
    EV << "Handling Mp1 message with connection ID: " << connId << endl;
}

void PredictMecApp::handleHttpMessage(int connId) {
    EV << "Handling HTTP message with connection ID: " << connId << endl;
}

void PredictMecApp::handleUeMessage(cMessage *msg) {
    EV << "Handling UE message in PredictMecApp" << endl;
    delete msg;
}

void PredictMecApp::established(int connId) {
    EV << "Connection Established with connection ID: " << connId << endl;
}

//void receiveSignal(cComponent *source, simsignal_t signalID, cObject *value, cObject *details) {
//    // Extract the integer value from the signal
//    int nodeId = *(int *)value;
//    std::cout << "signal received " << nodeId << endl;
//}


PredictMecApp::~PredictMecApp() {
    // TODO Auto-generated destructor stub

}


} /* namespace simu5g */
