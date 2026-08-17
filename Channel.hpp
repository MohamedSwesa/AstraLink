#ifndef ASTRALINK_CHANNEL_H
#define ASTRALINK_CHANNEL_H

#include "DataStream.hpp"

class Channel {
private:
    double errorProbability; // A number between 0.0 (0%) and 1.0 (100%)

public:
    Channel(double probability);

    DataStream transmit(const DataStream& inputData);
};

#endif // ASTRALINK_CHANNEL_H