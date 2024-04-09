/*
 * my_simulation_network.cc
 *
 *  Created on: Feb 1, 2024
 *      Author: pramita
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class node11 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();

    int nodeId;
    double predictedNumber;
    std::vector<int> receivedValues;
    int windowSize = 5;

public:
    node11(int id = 0) : nodeId(id), predictedNumber(0) {}
};

Define_Module(node11);

void node11::initialize()
{
    // Get the nodeId parameter from the parent module
    nodeId = par("nodeId");

    // Initialize the node
    EV << "Node11 " << nodeId << " initialized\n";

    // Start transmitting messages
    transmitMessage();
}

void node11::handleMessage(cMessage *msg)
{
    // Handle incoming messages
    EV << "Node11 " << nodeId << " received a message: " << msg->getName() << "\n";

    // Delete the message after processing
    delete msg;
}

void node11::transmitMessage()
{
    for (int i = 0; i < 100; ++i) {
        // Generate random input values within the specified range
        int randomValue = intuniform(0, 220);
        receivedValues.push_back(randomValue);

        // Keep the window size limited
        if (receivedValues.size() > windowSize) {
            receivedValues.erase(receivedValues.begin());
        }

        // Calculate the predicted number using moving average
        double sum = 0;
        for (int value : receivedValues) {
            sum += value;
        }
        predictedNumber = sum / receivedValues.size();

        // Create and send the message to the hub node
        char msgname[20];
        sprintf(msgname, "%d", randomValue);
        cMessage *msg = new cMessage(msgname);

        // Log message transmission
        EV << "Node11 " << nodeId << " generating value: " << randomValue << "\n";
        EV << "Node11 " << nodeId << " transmitting message: " << msg->getName() << "\n";

        send(msg, "output_gate", 0);
    }
}

class node12 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();

    int nodeId;
    double predictedNumber;
    std::vector<int> receivedValues;
    int windowSize = 5;

public:
    node12(int id = 0) : nodeId(id), predictedNumber(0) {}
};

Define_Module(node12);

void node12::initialize()
{
    // Get the nodeId parameter from the parent module
        nodeId = par("nodeId");


    // Initialize the node
    EV << "Node12 " << nodeId << " initialized\n";

    // Start transmitting messages
    transmitMessage();
}

void node12::handleMessage(cMessage *msg)
{
    // Handle incoming messages
    EV << "Node12 " << nodeId << " received a message: " << msg->getName() << "\n";

    // Delete the message after processing
    delete msg;
}

void node12::transmitMessage()
{
    for (int i = 0; i < 100; ++i) {
        // Generate random input values within the specified range
        int randomValue = intuniform(0, 220);
        receivedValues.push_back(randomValue);

        // Keep the window size limited
        if (receivedValues.size() > windowSize) {
            receivedValues.erase(receivedValues.begin());
        }

        // Calculate the predicted number using moving average
        double sum = 0;
        for (int value : receivedValues) {
            sum += value;
        }
        predictedNumber = sum / receivedValues.size();

        // Create and send the message to the hub node
        char msgname[20];
        sprintf(msgname, "%d", randomValue);
        cMessage *msg = new cMessage(msgname);

        // Log message transmission
        EV << "Node12 " << nodeId << " generating value: " << randomValue << "\n";
        EV << "Node12 " << nodeId << " transmitting message: " << msg->getName() << "\n";

        send(msg, "output_gate", 0);
    }
}


class node21 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();

    int nodeId;
    double predictedNumber;
    std::vector<int> receivedValues;
    int windowSize = 5;

public:
    node21(int id = 0) : nodeId(id), predictedNumber(0) {}
};

Define_Module(node21);

void node21::initialize()
{
    // Get the nodeId parameter from the parent module
        nodeId = par("nodeId");


    // Initialize the node
    EV << "Node21 " << nodeId << " initialized\n";

    // Start transmitting messages
    transmitMessage();
}

void node21::handleMessage(cMessage *msg)
{
    // Handle incoming messages
    EV << "Node21 " << nodeId << " received a message: " << msg->getName() << "\n";

    // Delete the message after processing
    delete msg;
}

void node21::transmitMessage()
{
    for (int i = 0; i < 100; ++i) {
        // Generate random input values within the specified range
        int randomValue = intuniform(0, 220);
        receivedValues.push_back(randomValue);

        // Keep the window size limited
        if (receivedValues.size() > windowSize) {
            receivedValues.erase(receivedValues.begin());
        }

        // Calculate the predicted number using moving average
        double sum = 0;
        for (int value : receivedValues) {
            sum += value;
        }
        predictedNumber = sum / receivedValues.size();

        // Create and send the message to Hub_node2
        char msgname[20];
        sprintf(msgname, "%d", randomValue);
        cMessage *msg = new cMessage(msgname);

        // Log message transmission
        EV << "Node21 " << nodeId << " generating value: " << randomValue << "\n";
        EV << "Node21 " << nodeId << " transmitting message: " << msg->getName() << " to Hub_node2\n";

        send(msg, "output_gate", 0);
    }
}

class node22 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();

    int nodeId;
    double predictedNumber;
    std::vector<int> receivedValues;
    int windowSize = 5;

public:
    node22(int id = 0) : nodeId(id), predictedNumber(0) {}
};

Define_Module(node22);

void node22::initialize()
{
    // Get the nodeId parameter from the parent module
        nodeId = par("nodeId");


    // Initialize the node
    EV << "Node22 " << nodeId << " initialized\n";

    // Start transmitting messages
    transmitMessage();
}

void node22::handleMessage(cMessage *msg)
{
    // Handle incoming messages
    EV << "Node22 " << nodeId << " received a message: " << msg->getName() << "\n";

    // Delete the message after processing
    delete msg;
}

void node22::transmitMessage()
{
    for (int i = 0; i < 100; ++i) {
        // Generate random input values within the specified range
        int randomValue = intuniform(0, 200);
        receivedValues.push_back(randomValue);

        // Keep the window size limited
        if (receivedValues.size() > windowSize) {
            receivedValues.erase(receivedValues.begin());
        }

        // Calculate the predicted number using moving average
        double sum = 0;
        for (int value : receivedValues) {
            sum += value;
        }
        predictedNumber = sum / receivedValues.size();

        // Create and send the message to Hub_node2
        char msgname[20];
        sprintf(msgname, "%d", randomValue);
        cMessage *msg = new cMessage(msgname);

        // Log message transmission
        EV << "Node22 " << nodeId << " generating value: " << randomValue << "\n";
        EV << "Node22 " << nodeId << " transmitting message: " << msg->getName() << " to Hub_node2\n";

        send(msg, "output_gate", 0);
    }
}

class node31 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();

    int nodeId;
    double predictedNumber;
    std::vector<int> receivedValues;
    int windowSize = 5;

public:
    node31(int id = 0) : nodeId(id), predictedNumber(0) {}
};

Define_Module(node31);

void node31::initialize()
{
    // Get the nodeId parameter from the parent module
        nodeId = par("nodeId");


    // Initialize the node
    EV << "Node31 " << nodeId << " initialized\n";

    // Start transmitting messages
    transmitMessage();
}

void node31::handleMessage(cMessage *msg)
{
    // Handle incoming messages
    EV << "Node31 " << nodeId << " received a message: " << msg->getName() << "\n";

    // Delete the message after processing
    delete msg;
}

void node31::transmitMessage()
{
    for (int i = 0; i < 100; ++i) {
        // Generate random input values within the specified range
        int randomValue = intuniform(0, 200);
        receivedValues.push_back(randomValue);

        // Keep the window size limited
        if (receivedValues.size() > windowSize) {
            receivedValues.erase(receivedValues.begin());
        }

        // Calculate the predicted number using moving average
        double sum = 0;
        for (int value : receivedValues) {
            sum += value;
        }
        predictedNumber = sum / receivedValues.size();

        // Create and send the message to Hub_node3
        char msgname[20];
        sprintf(msgname, "%d", randomValue);
        cMessage *msg = new cMessage(msgname);

        // Log message transmission
        EV << "Node31 " << nodeId << " generating value: " << randomValue << "\n";
        EV << "Node31 " << nodeId << " transmitting message: " << msg->getName() << " to Hub_node3\n";

        send(msg, "output_gate", 0);
    }
}

class node32 : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();

    int nodeId;
    double predictedNumber;
    std::vector<int> receivedValues;
    int windowSize = 5;

public:
    node32(int id = 0) : nodeId(id), predictedNumber(0) {}
};

Define_Module(node32);

void node32::initialize()
{
    // Get the nodeId parameter from the parent module
        nodeId = par("nodeId");


    // Initialize the node
    EV << "Node32 " << nodeId << " initialized\n";

    // Start transmitting messages
    transmitMessage();
}

void node32::handleMessage(cMessage *msg)
{
    // Handle incoming messages
    EV << "Node32 " << nodeId << " received a message: " << msg->getName() << "\n";

    // Delete the message after processing
    delete msg;
}

void node32::transmitMessage()
{
    for (int i = 0; i < 100; ++i) {
        // Generate random input values within the specified range
        int randomValue = intuniform(0, 3000);
        receivedValues.push_back(randomValue);

        // Keep the window size limited
        if (receivedValues.size() > windowSize) {
            receivedValues.erase(receivedValues.begin());
        }

        // Calculate the predicted number using moving average
        double sum = 0;
        for (int value : receivedValues) {
            sum += value;
        }
        predictedNumber = sum / receivedValues.size();

        // Create and send the message to Hub_node3
        char msgname[20];
        sprintf(msgname, "%d", randomValue);
        cMessage *msg = new cMessage(msgname);

        // Log message transmission
        EV << "Node32 " << nodeId << " generating value: " << randomValue << "\n";
        EV << "Node32 " << nodeId << " transmitting message: " << msg->getName() << " to Hub_node3\n";

        send(msg, "output_gate", 0);
    }
}
