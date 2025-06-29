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

#ifndef SRC_DDOSIM5G_MESSAGESENDER_MESSAGESENDER_H_
#define SRC_DDOSIM5G_MESSAGESENDER_MESSAGESENDER_H_

#include <omnetpp.h>
#include <fstream>

#include "inet/networklayer/common/L3AddressResolver.h"
using namespace omnetpp;


class MessageSender : public cSimpleModule {
private:
private:
    cMessage *selfMsg = nullptr;  // Pointer to the self-message
protected:

    int predictTime;
    omnetpp::simsignal_t predictSignal_;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    virtual void finish() override;


public:
    MessageSender();
    virtual ~MessageSender();
};

#endif
