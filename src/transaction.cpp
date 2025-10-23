#include "transaction.h"
#include "helper.h"

namespace bitcoin {

bool transaction::parse(const vector<uint8_t>& data) {
    size_t offset = 0;

    // Version
    version_ = readUInt32LE(data, offset);

    // Check SegWit marker and flag
    hasWitness_ = false;
    if (data[offset] == 0x00 && data[offset + 1] == 0x01) {
        hasWitness_ = true;
        offset += 2;
    }

    // Input count
    uint64_t vinCount = readVarInt(data, offset);
    if (vinCount == 0) return false;

    // Each input
    for (uint64_t i = 0; i < vinCount; ++i) {
        TxInput input{};
        input.prevTxHash = vector<uint8_t>(data.begin() + offset, data.begin() + offset + 32);
        offset += 32;
        input.prevTxIndex = readUInt32LE(data, offset);

        uint64_t scriptLen = readVarInt(data, offset);
        input.scriptSig = vector<uint8_t>(data.begin() + offset, data.begin() + offset + scriptLen);
        offset += scriptLen;

        input.sequence = readUInt32LE(data, offset);

        vins_.push_back(input);
    }

    // Output count
    uint64_t voutCount = readVarInt(data, offset);

    // Each output
    for (uint64_t i = 0; i < voutCount; ++i) {
        TxOutput output{};
        output.value = readUInt64LE(data, offset);
        uint64_t scriptLen = readVarInt(data, offset);
        output.scriptPubKey = vector<uint8_t>(data.begin() + offset, data.begin() + offset + scriptLen);
        offset += scriptLen;

        vouts_.push_back(output);
    }

    // parse Witness
    if (hasWitness_) {
        for (size_t i = 0; i < vins_.size(); ++i) {
            uint64_t witCount = readVarInt(data, offset);
            for (uint64_t j = 0; j < witCount; ++j) {
                uint64_t len = readVarInt(data, offset);
                vector<uint8_t> item(data.begin() + offset, data.begin() + offset + len);
                offset += len;
                vins_[i].witness.push_back(item);
            }
        }
    }

    locktime_ = readUInt32LE(data, offset);
    return true;
}

// Print transaction content
void transaction::print() {
    cout << "Version: " << version_ << endl;
    cout << "Has Witness: " << hasWitness_ << endl;

    cout << "--- Inputs ---" << endl;
    for (size_t i = 0; i < vins_.size(); ++i) {
        const TxInput& in = vins_[i];
        cout << "Input #" << i << endl;
        cout << "  PrevTxHash: " << bytesToHex(in.prevTxHash) << endl;
        cout << "  PrevTxIndex: " << in.prevTxIndex << endl;
        parseScript(in.scriptSig);
        cout << "  Sequence: " << in.sequence << endl;

        if (in.witness.size() > 0) {
            cout << "  Witness:" << endl;
            for (size_t j = 0; j < in.witness.size(); ++j) {
                cout << "    " << j << ": " << bytesToHex(in.witness[j]) << endl;
            }
        }
    }

    cout << "--- Outputs ---" << endl;
    for (size_t i = 0; i < vouts_.size(); ++i) {
        const TxOutput& out = vouts_[i];
        cout << "Output #" << i << endl;
        cout << "  Value: " << out.value << " sats" << endl;
        parseScript(out.scriptPubKey);
    }

    cout << "LockTime: " << locktime_ << endl;
}

}