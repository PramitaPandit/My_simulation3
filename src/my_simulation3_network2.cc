/*
 * my_simulation_network2.cc
 *
 *  Created on: Mar 8, 2024
 *      Author: pramita
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <fstream>
#include <vector>
#include <numeric> // Add this header for accumulate function

using namespace omnetpp;

#include "SimpleKalmanFilter.h"

class Hub_node1 : public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();
    virtual void finish() override; // Added for data collection and plotting

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variables for data collection and plotting
    std::vector<double> predictionErrors;
    cOutVector predictionErrorVector;

    // Kalman filter for Node_11 and Node_12 inputs
    SimpleKalmanFilter kf_node11;
    SimpleKalmanFilter kf_node12;

public:
    Hub_node1() : nodeId(0), kf_node11(2.0, 2.0, 0.01), kf_node12(2.0, 2.0, 0.01) {} // Default constructor
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval
};

Define_Module(Hub_node1);

void Hub_node1::initialize() {
    nodeId = atoi(getName());
    EV << "Hub_node1 " << nodeId << " initialized\n";

    // Initialize x from a parameter
    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);

    // Registering the vector for data collection
    predictionErrorVector.setName("PredictionError");
}

void Hub_node1::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
    if (strcmp(msg->getSenderModule()->getName(), "OBN") == 0) {
        EV << "Received 'Hello There!' message from the OBN. Starting message transmission.\n";
        // Start transmitting messages
        transmitMessage();
    } else {
        // Handle other messages here
        EV << "Hub_node1 " << getName() << " received a message, but waiting for 'Hello There!' from the OBN.\n";
        // Store the received message and handle it later
        // Example: storeMessage(msg);
    }

    if (msg == decrementXMsg) {
        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        double receivedValue = atof(msg->getName());
        double filteredValue;
        bool transmitData = false;

        // Perform Kalman filtering on the input
        if (strcmp(msg->getSenderModule()->getName(), "Node_11") == 0) {
            // Filter input from Node_11
            filteredValue = kf_node11.updateEstimate(receivedValue);
            EV << "Received value from Node_11: " << receivedValue << ", Predicted value: " << filteredValue << endl;
            EV << "Node 11 data received at Hub_1.\n";
        } else if (strcmp(msg->getSenderModule()->getName(), "Node_12") == 0) {
            // Filter input from Node_12
            filteredValue = kf_node12.updateEstimate(receivedValue);
            EV << "Received value from Node_12: " << receivedValue << ", Predicted value: " << filteredValue << endl;
            EV << "Node 12 data received at Hub_1.\n";
        } else {
            // Handle other messages here
            EV << "Received a message from unexpected sender: " << msg->getSenderModule()->getName() << ".\n";
            // Handle the message according to your application logic
        }

        // Logic for data transmission based on Kalman Filter output
        double predictionError = std::abs(filteredValue - receivedValue);
        predictionErrors.push_back(predictionError);
        predictionErrorVector.record(predictionError);

        if (predictionError == 0 || std::abs(predictionError) == 10) {
            transmitData = true;
            EV << "Data transmitted from " << msg->getSenderModule()->getName() << " to OBN node.\n";
        } else {
            EV << "Data not transmitted from " << msg->getSenderModule()->getName() << " to OBN node.\n";
        }

        if (transmitData) {
            // Forward the message to OBN_node
            send(msg, "output_gate", 2); // Assuming output_gate[2] is the gate connected to OBN_node
        }
    }
}

void Hub_node1::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Hub_node1 " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    send(msg1, "output_gate", gateIndex);
}

void Hub_node1::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Hub_node1 " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}

void Hub_node1::finish()
{
    // At the end of the simulation, calculate statistics on the collected data
    // For example, mean, standard deviation, etc.
    double sum = std::accumulate(predictionErrors.begin(), predictionErrors.end(), 0.0);
    double mean = sum / predictionErrors.size();

    EV << "Mean Prediction Error: " << mean << endl;

    // Plotting the data using built-in OMNeT++ capabilities
    // Make sure to enable result recording in the ini file with "record-eventlog = true"
    EV << "Plotting Prediction Error data...\n";
    cHistogram hist("Prediction Error");
    // hist.setRangeAutoUpper(0); // Remove this line as it's causing an error
    for (double error : predictionErrors) {
        hist.collect(error);
    }
    hist.recordAs("PredictionError");
}

// Hub_node2:

class Hub_node2 : public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();
    virtual void finish() override; // Added for data collection and plotting

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variables for data collection and plotting
    std::vector<double> predictionErrors;
    cOutVector predictionErrorVector;

    // Kalman filter for Node_21 and Node_22 inputs
    SimpleKalmanFilter kf_node21;
    SimpleKalmanFilter kf_node22;

public:
    Hub_node2() : nodeId(0), kf_node21(2.0, 2.0, 0.01), kf_node22(2.0, 2.0, 0.01) {} // Default constructor
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval
};

Define_Module(Hub_node2);

void Hub_node2::initialize() {
    nodeId = atoi(getName());
    EV << "Hub_node2 " << nodeId << " initialized\n";

    // Initialize x from a parameter
    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);

    // Registering the vector for data collection
    predictionErrorVector.setName("PredictionError");
}

void Hub_node2::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
    if (strcmp(msg->getSenderModule()->getName(), "OBN") == 0) {
        EV << "Received 'Hello There!' message from the OBN. Starting message transmission.\n";
        // Start transmitting messages
        transmitMessage();
    } else {
        // Handle other messages here
        EV << "Hub_node2 " << getName() << " received a message, but waiting for 'Hello There!' from the OBN.\n";
        // Store the received message and handle it later
        // Example: storeMessage(msg);
    }

    if (msg == decrementXMsg) {
        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        double receivedValue = atof(msg->getName());
        double filteredValue;
        bool transmitData = false;

        // Perform Kalman filtering on the input
        if (strcmp(msg->getSenderModule()->getName(), "Node_21") == 0) {
            // Filter input from Node_21
            filteredValue = kf_node21.updateEstimate(receivedValue);
            EV << "Received value from Node_21: " << receivedValue << ", Predicted value: " << filteredValue << endl;
            EV << "Node 21 data received at Hub_2.\n";
        } else if (strcmp(msg->getSenderModule()->getName(), "Node_22") == 0) {
            // Filter input from Node_22
            filteredValue = kf_node22.updateEstimate(receivedValue);
            EV << "Received value from Node_22: " << receivedValue << ", Predicted value: " << filteredValue << endl;
            EV << "Node 22 data received at Hub_2.\n";
        } else {
            // Handle other messages here
            EV << "Received a message from unexpected sender: " << msg->getSenderModule()->getName() << ".\n";
            // Handle the message according to your application logic
        }

        // Logic for data transmission based on Kalman Filter output
        double predictionError = std::abs(filteredValue - receivedValue);
        predictionErrors.push_back(predictionError);
        predictionErrorVector.record(predictionError);

        if (predictionError == 0 || std::abs(predictionError) == 10) {
            transmitData = true;
            EV << "Data transmitted from " << msg->getSenderModule()->getName() << " to OBN node.\n";
        } else {
            EV << "Data not transmitted from " << msg->getSenderModule()->getName() << " to OBN node.\n";
        }

        if (transmitData) {
            // Forward the message to OBN_node
            send(msg, "output_gate", 2); // Assuming output_gate[2] is the gate connected to OBN_node
        }
    }
}

void Hub_node2::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Hub_node2 " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    send(msg1, "output_gate", gateIndex);
}

void Hub_node2::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Hub_node2 " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}

void Hub_node2::finish()
{
    // At the end of the simulation, calculate statistics on the collected data
    // For example, mean, standard deviation, etc.
    double sum = std::accumulate(predictionErrors.begin(), predictionErrors.end(), 0.0);
    double mean = sum / predictionErrors.size();

    EV << "Mean Prediction Error: " << mean << endl;

    // Plotting the data using built-in OMNeT++ capabilities
    // Make sure to enable result recording in the ini file with "record-eventlog = true"
    EV << "Plotting Prediction Error data...\n";
    cHistogram hist("Prediction Error");
    // hist.setRangeAutoUpper(0); // Remove this line as it's causing an error
    for (double error : predictionErrors) {
        hist.collect(error);
    }
    hist.recordAs("PredictionError");
}
//Hub_node3:

class Hub_node3 : public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();
    virtual void finish() override; // Added for data collection and plotting

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variables for data collection and plotting
    std::vector<double> predictionErrors;
    cOutVector predictionErrorVector;

    // Kalman filter for Node_31 and Node_32 inputs
    SimpleKalmanFilter kf_node31;
    SimpleKalmanFilter kf_node32;

public:
    Hub_node3() : nodeId(0), kf_node31(0.01, 0.01, 0.01), kf_node32(0.5, 0.5, 0.01) {} // Default constructor
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval
};

Define_Module(Hub_node3);

void Hub_node3::initialize() {
    nodeId = atoi(getName());
    EV << "Hub_node3 " << nodeId << " initialized\n";

    // Initialize x from a parameter
    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);

    // Registering the vector for data collection
    predictionErrorVector.setName("PredictionError");
}

void Hub_node3::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
    if (strcmp(msg->getSenderModule()->getName(), "OBN") == 0) {
        EV << "Received 'Hello There!' message from the OBN. Starting message transmission.\n";
        // Start transmitting messages
        transmitMessage();
    } else {
        // Handle other messages here
        EV << "Hub_node3 " << getName() << " received a message, but waiting for 'Hello There!' from the OBN.\n";
        // Store the received message and handle it later
        // Example: storeMessage(msg);
    }

    if (msg == decrementXMsg) {
        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        double receivedValue = atof(msg->getName());
        double filteredValue;
        bool transmitData = false;

        // Perform Kalman filtering on the input
        if (strcmp(msg->getSenderModule()->getName(), "Node_31") == 0) {
            // Filter input from Node_31
            filteredValue = kf_node31.updateEstimate(static_cast<int>(receivedValue));
            EV << "Received value from Node_31: " << receivedValue << ", Predicted value: " << filteredValue << endl;
            EV << "Node 31 data received at Hub_3.\n";
        } else if (strcmp(msg->getSenderModule()->getName(), "Node_32") == 0) {
            // Filter input from Node_32
            filteredValue = kf_node32.updateEstimate(static_cast<int>(receivedValue));
            EV << "Received value from Node_32: " << receivedValue << ", Predicted value: " << filteredValue << endl;
            EV << "Node 32 data received at Hub_3.\n";
        } else {
            // Handle other messages here
            EV << "Received a message from unexpected sender: " << msg->getSenderModule()->getName() << ".\n";
            // Handle the message according to your application logic
        }

        // Logic for data transmission based on Kalman Filter output
        double predictionError = std::abs(filteredValue - receivedValue);
        predictionErrors.push_back(predictionError);
        predictionErrorVector.record(predictionError);

        if (predictionError == 0 || std::abs(predictionError) == 10) {
            transmitData = true;
            EV << "Data transmitted from " << msg->getSenderModule()->getName() << " to OBN node.\n";
        } else {
            EV << "Data not transmitted from " << msg->getSenderModule()->getName() << " to OBN node.\n";
        }

        if (transmitData) {
            // Forward the message to OBN_node
            send(msg, "output_gate", 2); // Assuming output_gate[2] is the gate connected to OBN_node
        }
    }
}

void Hub_node3::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Hub_node3 " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    send(msg1, "output_gate", gateIndex);
}

void Hub_node3::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Hub_node3 " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}

void Hub_node3::finish()
{
    // At the end of the simulation, calculate statistics on the collected data
    // For example, mean, standard deviation, etc.
    double sum = std::accumulate(predictionErrors.begin(), predictionErrors.end(), 0.0);
    double mean = sum / predictionErrors.size();

    EV << "Mean Prediction Error: " << mean << endl;

    // Plotting the data using built-in OMNeT++ capabilities
    // Make sure to enable result recording in the ini file with "record-eventlog = true"
    EV << "Plotting Prediction Error data...\n";
    cHistogram hist("Prediction Error");
    // hist.setRangeAutoUpper(0); // Remove this line as it's causing an error
    for (double error : predictionErrors) {
        hist.collect(error);
    }
    hist.recordAs("PredictionError");
}
