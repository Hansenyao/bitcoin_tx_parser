#include "transaction.h"
#include "helper.h"
#include <string>
#include <iostream>

using namespace std;

int main() {
    string txHex;
    cout << "Enter transaction hex: ";
    cin >> txHex;
    cout << endl;

    try {
        bitcoin::transaction tx;
        vector<uint8_t> txBytes = bitcoin::hexToBytes(txHex);
        if (!tx.parse(txBytes)) {
            cout << "Failed! Invalid data.";
            return -1;
        }
        tx.print();
    } catch (const exception& e) {
        cerr << "Error parsing transaction: " << e.what() << endl;
    }

    return 0;
}