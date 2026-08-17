#include "Channel.hpp"
#include <stdexcept>
#include <random>

Channel::Channel(double probability) : errorProbability(probability) {
    // A probability must be between 0% and 100%
    if (probability < 0.0 || probability > 1.0) {
        throw std::invalid_argument("Probability must be between 0.0 and 1.0");
    }
}

// The Simulation Method
DataStream Channel::transmit(const DataStream& inputData) {
    std::string payload = inputData.getPayload();
    std::string corruptedPayload = "";

    // random number generator
    std::random_device rd; 
    std::mt19937 gen(rd()); // Standard mersenne twister engine
    
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (char bit : payload) {
        double diceRoll = dis(gen);

        if (diceRoll < errorProbability) {
            if (bit == '1') {
                corruptedPayload += '0';
            } else {
                corruptedPayload += '1';
            }
        } else {
            corruptedPayload += bit;
        }
    }

    return DataStream(corruptedPayload);
}