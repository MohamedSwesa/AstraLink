#include <iostream>
#include <fstream>
#include <stdexcept>
#include "ChannelCoder.hpp"
#include "Channel.hpp"

int main() {

    ChannelCoder* myCoder = nullptr; 
    
    try {
        std::cout << "=================================\n";
        std::cout << "   AstraLink Telecom Simulator   \n";
        std::cout << "=================================\n";
        std::cout << "1. Parity Coder (Even Parity)\n";
        std::cout << "2. Hamming (7,4) FEC Coder\n";
        std::cout << "---------------------------------\n";
        std::cout << "Select your coding scheme (1 or 2): ";
        
        int choice;
        std::cin >> choice;

        // dynamic memory allocation based on user input
        if (choice == 1) {
            myCoder = new ParityCoder(true);
        } else if (choice == 2) {
            myCoder = new HammingCoder();
        } else {
            throw std::invalid_argument("Invalid menu selection. Please restart and enter 1 or 2.");
        }

        std::cout << "\n>>> Booting pipeline using: " << myCoder->getName() << "\n\n";

        std::ifstream inputFile("input.txt");
        
        if (!inputFile.is_open()) {
            throw std::runtime_error("Critical Error: Could not locate 'input.txt'.");
        }

        std::string rawData;
        inputFile >> rawData;
        inputFile.close();

        std::cout << "Successfully read from input.txt: " << rawData << "\n";
        DataStream originalData(rawData);

        // Transmission Pipeline
        DataStream encodedData = myCoder->encode(originalData);
        std::cout << "Encoded Payload: " << encodedData.getPayload() << "\n";

        // Simulate a channel with a 3.5% error rate
        Channel myNetwork(0.035); 
        DataStream receivedData = myNetwork.transmit(encodedData);
        std::cout << "Received Payload (Post-Channel): " << receivedData.getPayload() << "\n";
        
        DataStream decodedData = myCoder->decode(receivedData);
        std::cout << "Successfully decoded data:       " << decodedData.getPayload() << "\n";

        std::ofstream outputFile("output.txt");
        
        if (!outputFile.is_open()) {
            throw std::runtime_error("Critical Error: Could not create 'output.txt'.");
        }

        outputFile << decodedData;
        outputFile.close();

        std::cout << "Successfully wrote decoded data to output.txt\n";

    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime Exception: " << e.what() << "\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << "Data Error: " << e.what() << "\n";
    }

    delete myCoder;
    return 0;
}