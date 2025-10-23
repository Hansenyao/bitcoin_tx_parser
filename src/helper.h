#ifndef _BITCOIN_HELPER_H_
#define _BITCOIN_HELPER_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace bitcoin {
    // Convert a char to a byte
    uint8_t hexCharToByte(char c);

    // Convert a hex string to a byte array
    vector<uint8_t> hexToBytes(const string& hex);

    // Convert a byte array to a hex string
    string bytesToHex(const vector<uint8_t>& bytes);

    // Read a variable int value
    uint64_t readVarInt(const vector<uint8_t>& data, size_t& offset);

    // Read a uint32 integer in little-end
    uint32_t readUInt32LE(const vector<uint8_t>& data, size_t& offset);

    // Read a uint64 integer in little-end
    uint64_t readUInt64LE(const vector<uint8_t>& data, size_t& offset);
    
    // Parse script data
    void parseScript(const vector<uint8_t>& script);
}
#endif