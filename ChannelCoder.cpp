// ChannelCoder.cpp
#include "ChannelCoder.hpp"
#include <stdexcept>
#include <iostream>

ParityCoder::ParityCoder(bool evenParity) 
    : ChannelCoder(evenParity ? "Even Parity Coder" : "Odd Parity Coder"), 
      isEvenParity(evenParity) {}

// Encode: Adds the redundancy bit
DataStream ParityCoder::encode(const DataStream& data) {
    std::string payload = data.getPayload();
    int onesCount = 0;

    // Count the 1s and check for invalid characters
    for (char bit : payload) {
        if (bit == '1') {
            onesCount++;
        } else if (bit != '0') {
            throw std::invalid_argument("DataStream contains non-binary characters.");
        }
    }

    // Calculate what the parity bit should be
    char parityBit;
    if (isEvenParity) {
        parityBit = (onesCount % 2 == 0) ? '0' : '1';
    } else {
        parityBit = (onesCount % 2 == 0) ? '1' : '0';
    }

    // Append the parity bit to the end of the payload and return the new stream
    return DataStream(payload + parityBit);
}

// Decode: Checks the redundancy bit for errors
DataStream ParityCoder::decode(const DataStream& data) {
    std::string payload = data.getPayload();
    
    if (payload.empty()) {
        throw std::invalid_argument("Cannot decode an empty DataStream.");
    }

    int onesCount = 0;
    for (char bit : payload) {
        if (bit == '1') onesCount++;
    }

    // Verify if the parity rule holds true
    bool isValid = false;
    if (isEvenParity && (onesCount % 2 == 0)) isValid = true;
    if (!isEvenParity && (onesCount % 2 != 0)) isValid = true;

    if (!isValid) {
        throw std::runtime_error("Transmission Error Detected: Parity check failed.");
    }

    // If valid, strip the parity bit off the end to return the original clean message
    std::string originalMessage = payload.substr(0, payload.length() - 1);
    return DataStream(originalMessage);
}


HammingCoder::HammingCoder() : ChannelCoder("Hamming (7,4) Coder") {}

std::string HammingCoder::encodeBlock(const std::string& d) {
    // Extract the bits
    int d1 = d[0] - '0';
    int d2 = d[1] - '0';
    int d3 = d[2] - '0';
    int d4 = d[3] - '0';

    // Calculate Parity Bits using XOR (^)
    int p1 = d1 ^ d2 ^ d4;
    int p2 = d1 ^ d3 ^ d4;
    int p3 = d2 ^ d3 ^ d4;

    // Construct the 7-bit Hamming block: p1, p2, d1, p3, d2, d3, d4
    std::string block = "";
    block += std::to_string(p1);
    block += std::to_string(p2);
    block += std::to_string(d1);
    block += std::to_string(p3);
    block += std::to_string(d2);
    block += std::to_string(d3);
    block += std::to_string(d4);

    return block;
}

DataStream HammingCoder::encode(const DataStream& data) {
    std::string payload = data.getPayload();
    std::string encodedPayload = "";

    // Pad the string with '0's so its length is a perfect multiple of 4
    while (payload.length() % 4 != 0) {
        payload += "0";
    }

    // Process the data in 4-bit chunks
    for (size_t i = 0; i < payload.length(); i += 4) {
        std::string chunk = payload.substr(i, 4);
        encodedPayload += encodeBlock(chunk);
    }

    return DataStream(encodedPayload);
}

std::string HammingCoder::decodeBlock(std::string b) {
    // Extract the bits
    int p1 = b[0] - '0';
    int p2 = b[1] - '0';
    int d1 = b[2] - '0';
    int p3 = b[3] - '0';
    int d2 = b[4] - '0';
    int d3 = b[5] - '0';
    int d4 = b[6] - '0';

    // Calculate the Syndrome Bits
    int s1 = p1 ^ d1 ^ d2 ^ d4;
    int s2 = p2 ^ d1 ^ d3 ^ d4;
    int s3 = p3 ^ d2 ^ d3 ^ d4;

    // Calculate the integer value of the syndrome
    int errorPos = (s1 * 1) + (s2 * 2) + (s3 * 4);

    // If syndrome is > 0, an error occurred at exactly that position!
    if (errorPos > 0) {
        std::cout << "  [!] Error detected at bit " << errorPos << ". Correcting...\n";
        // Arrays are 0-indexed, but Hamming positions are 1-indexed, so subtract 1
        b[errorPos - 1] = (b[errorPos - 1] == '1') ? '0' : '1'; 
    }

    // Extract and return just the 4 data bits from the corrected block
    std::string correctedData = "";
    correctedData += b[2]; // d1
    correctedData += b[4]; // d2
    correctedData += b[5]; // d3
    correctedData += b[6]; // d4

    return correctedData;
}

DataStream HammingCoder::decode(const DataStream& data) {
    std::string payload = data.getPayload();
    
    if (payload.length() % 7 != 0) {
        throw std::runtime_error("Hamming decode failed: Data is not a multiple of 7 bits.");
    }

    std::string decodedPayload = "";

    // Process the data in 7-bit chunks
    for (size_t i = 0; i < payload.length(); i += 7) {
        std::string chunk = payload.substr(i, 7);
        decodedPayload += decodeBlock(chunk);
    }

    return DataStream(decodedPayload);
}