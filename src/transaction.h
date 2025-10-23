#ifndef _BITCOIN_TRANSACTION_H_
#define _BITCOIN_TRANSACTION_H_

#include <vector>

using namespace std;

namespace bitcoin {

// vin
struct TxInput {
    vector<uint8_t> prevTxHash;
    uint32_t prevTxIndex;
    vector<uint8_t> scriptSig;
    uint32_t sequence;
    vector<vector<uint8_t>> witness;
};

// vout
struct TxOutput {
    uint64_t value;
    vector<uint8_t> scriptPubKey;
};

class transaction {
public:
    transaction(){};
    virtual ~transaction(){};
    bool parseTransaction(const vector<uint8_t>& data);
private:
    uint32_t version_;
    vector<TxInput> vins_;
    vector<TxOutput> vouts_;
    uint32_t locktime_;
    bool hasWitness_;
};

}

#endif // _BT_TX_TRANSACTION_H_
