//

// Class developed by EIT, Lund University, Lars Breum Hansen M.Sc

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "../MessageSender/MessageSender.h"

Define_Module(MessageSender);

MessageSender::MessageSender() {
    EV << "MessageSender constructor ended at time: " << simTime() << "\n";

}

MessageSender::~MessageSender() {
    EV << "DataTrafficController Deconstructor ended at time: " << simTime() << "\n";

}

void MessageSender::initialize() {

    int predictTime = par("startPredictTime").intValue();
    predictSignal_ = registerSignal("startPredictTime");
    std::cout << "MessageSender initialize()" << endl;
    std::cout << "predictTime: " << predictTime << endl;

    // Create a self-message
    selfMsg = new cMessage("selfMsg");

    // Schedule the message after 5 simulation time units
    scheduleAt(simTime() + predictTime, selfMsg);

}


void MessageSender::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        int nodeId = msg->getKind();

        // Emit signal to start prediction
        if(simTime() > predictTime) {
            std::cout << "predictTime has passed at time: " << simTime();
            emit(predictSignal_, nodeId);
        }

        delete msg;
    }
}

void MessageSender::finish() {
   // cancelAndDelete(selfMsg);  // Cleanup to prevent memory leaks
}


