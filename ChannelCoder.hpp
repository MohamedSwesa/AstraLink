#ifndef ASTRALINK_CHANNEL_CODER_H
#define ASTRALINK_CHANNEL_CODER_H

#include <string>
#include "DataStream.hpp"

class ChannelCoder {
protected:
    std::string coderName;

public:
    ChannelCoder(std::string name) : coderName(name) {}

    virtual ~ChannelCoder() = default;

    virtual DataStream encode(const DataStream& data) = 0;
    virtual DataStream decode(const DataStream& data) = 0;

    std::string getName() const { return coderName; }
};

class ParityCoder : public ChannelCoder {
private:
    bool isEvenParity; // True for Even parity, False for Odd

public:
    ParityCoder(bool evenParity);

    DataStream encode(const DataStream& data) override;
    DataStream decode(const DataStream& data) override;
};

class HammingCoder : public ChannelCoder {
private:
    // Helper functions to keep our main encode/decode clean
    std::string encodeBlock(const std::string& data4);
    std::string decodeBlock(std::string block7);

public:
    HammingCoder();
    DataStream encode(const DataStream& data) override;
    DataStream decode(const DataStream& data) override;
};

#endif // ASTRALINK_CHANNEL_CODER_H