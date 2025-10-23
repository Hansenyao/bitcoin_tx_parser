#include "helper.h"
#include <sstream>
#include <iomanip>

namespace bitcoin {

// Convert a char to a byte
uint8_t hexCharToByte(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    throw runtime_error("Invalid hex character");
}

// Convert a hex string to a byte array
vector<uint8_t> hexToBytes(const string& hex) {
    if (hex.size() % 2 != 0) throw runtime_error("Hex string length must be even");
    vector<uint8_t> bytes;
    bytes.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        bytes.push_back((hexCharToByte(hex[i]) << 4) | hexCharToByte(hex[i + 1]));
    }
    return bytes;
}

// Convert a byte array to a hex string
string bytesToHex(const vector<uint8_t>& bytes) {
    stringstream ss;
    ss << hex << setfill('0');
    for (uint8_t b : bytes) 
        ss << setw(2) << (int)b;
    return ss.str();
}

// Read a variable int value
uint64_t readVarInt(const vector<uint8_t>& data, size_t& offset) {
    if (offset >= data.size()) throw runtime_error("Offset out of range");
    uint8_t first = data[offset++];
    if (first < 0xfd) return first;
    if (first == 0xfd) {
        if (offset + 2 > data.size()) throw runtime_error("VarInt too short");
        uint64_t val = data[offset] | 
                       (data[offset + 1] << 8);
        offset += 2;
        return val;
    }
    if (first == 0xfe) {
        if (offset + 4 > data.size()) throw runtime_error("VarInt too short");
        uint64_t val = data[offset] | 
                      (data[offset + 1] << 8) | 
                      (data[offset + 2] << 16) | 
                      (data[offset + 3] << 24);
        offset += 4;
        return val;
    }
    if (first == 0xff) {
        if (offset + 8 > data.size()) throw runtime_error("VarInt too short");
        uint64_t val = 0;
        for (int i = 0; i < 8; ++i) val |= (uint64_t)data[offset + i] << (8 * i);
        offset += 8;
        return val;
    }
    throw runtime_error("Invalid VarInt");
}

// Read a uint32 integer in little-end
uint32_t readUInt32LE(const vector<uint8_t>& data, size_t& offset) {
    if (offset + 4 > data.size()) throw runtime_error("readUInt32LE out of range");
    uint32_t val = data[offset] | 
                  (data[offset + 1] << 8) | 
                  (data[offset + 2] << 16) | 
                  (data[offset + 3] << 24);
    offset += 4;
    return val;
}

// Read a uint64 integer in little-end
uint64_t readUInt64LE(const vector<uint8_t>& data, size_t& offset) {
    if (offset + 8 > data.size()) throw runtime_error("readUInt64LE out of range");
    uint64_t val = 0;
    for (int i = 0; i < 8; ++i) val |= (uint64_t)data[offset + i] << (8 * i);
    offset += 8;
    return val;
}

// Parse script data
void parseScript(const vector<uint8_t>& script) {
    size_t i = 0;
    cout << "    Script (hex): " << bytesToHex(script) << endl;
    while (i < script.size()) {
        uint8_t opcode = script[i++];
        if (opcode <= 0x4b) { // 小于等于75，表示push数据
            vector<uint8_t> data(script.begin() + i, script.begin() + i + opcode);
            cout << "      OP_PUSHDATA: " << bytesToHex(data) << endl;
            i += opcode;
        } else if (opcode == 0x00) cout << "      OP_0" << endl;
        else if (opcode == 0xac) cout << "      OP_CHECKSIG" << endl;
        else if (opcode == 0xad) cout << "      OP_CHECKSIGVERIFY" << endl;
        else if (opcode == 0xae) cout << "      OP_CHECKMULTISIG" << endl;
        else cout << "      OPCODE: 0x" << hex << (int)opcode << dec << endl;
    }
}

}