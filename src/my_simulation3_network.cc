/*
 * my_simulation_network.cc
 *
 *  Created on: Feb 1, 2024
 *      Author: pramita
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "SimpleKalmanFilter.h"

using namespace omnetpp;

class OBN_node : public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

    // Kalman filters for Hub_Node1, Hub_Node2, and Hub_Node3 inputs
    SimpleKalmanFilter kf_hub1;
    SimpleKalmanFilter kf_hub2;
    SimpleKalmanFilter kf_hub3;

public:
    OBN_node()
        : x(5.0),
          // Kalman filter parameters for Hub_Node1
          kf_hub1(2.0, 2.0, 0.01),
          // Kalman filter parameters for Hub_Node2
          kf_hub2(2.0, 2.0, 0.01),
          // Kalman filter parameters for Hub_Node3
          kf_hub3(0.5, 0.5, 0.01) {} // Default constructor with x initialized to 5

    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval
};

Define_Module(OBN_node);

void OBN_node::initialize() {
    nodeId = atoi(getName());
    EV << "OBN " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        // Schedule transmission of "Hello There!" message at time 0.0
        cMessage *msg1 = new cMessage("Hello There!");
        int gateIndex = intuniform(0, gateSize("output_gate") - 1);
        EV << "OBN " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
        bubble("Message Transmitted from OBN!");
        send(msg1, "output_gate", gateIndex);
    }

    // Schedule the self-message for decrementing x
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}


void OBN_node::handleMessage(cMessage *msg) {
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "OBN " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Perform Kalman filtering based on the source of the message
        if (strcmp(msg->getSenderModule()->getName(), "Hub_1") == 0) {
            // Filter input from Hub_Node1
            int receivedValue = atoi(msg->getName());
            int filteredValue1 = static_cast<int>(kf_hub1.updateEstimate(receivedValue));
            int measurementError1 = static_cast<int>(kf_hub1.getEstimateError());
            EV << "Received value from Hub_Node1: " << receivedValue << ", Predicted value: " << filteredValue1
               << ", Measurement Error: " << measurementError1 << endl;
            bubble("Message Received from Hub_Node1!");
            if (filteredValue1 == receivedValue || std::abs(filteredValue1 - receivedValue) == 10) {
                transmitMessage();
            }
        } else if (strcmp(msg->getSenderModule()->getName(), "Hub_2") == 0) {
            // Filter input from Hub_Node2
            int receivedValue = atoi(msg->getName());
            int filteredValue2 = static_cast<int>(kf_hub2.updateEstimate(receivedValue));
            int measurementError2 = static_cast<int>(kf_hub2.getEstimateError());
            EV << "Received value from Hub_Node2: " << receivedValue << ", Predicted value: " << filteredValue2
               << ", Measurement Error: " << measurementError2 << endl;
            bubble("Message Received from Hub_Node2!");
            if (filteredValue2 == receivedValue || std::abs(filteredValue2 - receivedValue) == 10) {
                transmitMessage();
            }
        } else if (strcmp(msg->getSenderModule()->getName(), "Hub_3") == 0) {
            // Filter input from Hub_Node3
            int receivedValue = atoi(msg->getName());
            int filteredValue3 = static_cast<int>(kf_hub3.updateEstimate(receivedValue));
            int measurementError3 = static_cast<int>(kf_hub3.getEstimateError());
            EV << "Received value from Hub_Node3: " << receivedValue << ", Predicted value: " << filteredValue3
               << ", Measurement Error: " << measurementError3 << endl;
            bubble("Message Received from Hub_Node3!");
            if (filteredValue3 == receivedValue || std::abs(filteredValue3 - receivedValue) == 10) {
                transmitMessage();
            }
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
}

void OBN_node::transmitMessage() {
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "OBN " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from OBN!");
    send(msg1, "output_gate", gateIndex);
}

void OBN_node::backoff() {
    // Simple backoff mechanism
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "OBN " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}
