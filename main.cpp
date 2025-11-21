#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "sha256.h"

using namespace std;

// --- to be able to use sha256 ---
string sha256(string text) {
    SHA256_CTX ctx;
    BYTE hash[SHA256_BLOCK_SIZE];

    sha256_init(&ctx);
    sha256_update(&ctx, (const BYTE*)text.c_str(), text.length());
    sha256_final(&ctx, hash);

    stringstream ss;
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}
// ----------------------------------------

class Block {
private:

    int index = 0, nonce = 0;
    long long timestamp;
    string data, previousHash, blockHash;

public:

    Block(int idx, string d, string prevHash) {
        index = idx;
        data = d;
        previousHash = prevHash;
        timestamp = time(0);
        blockHash = calculateHash();
        transactionLog(d);
    }

    string calculateHash() {
        stringstream ss;
        ss << index << previousHash << timestamp << data << nonce;
        return sha256(ss.str());
    }

    void transactionLog(string data) {
        ofstream file("transactions.log", ios::app);
        if (file.is_open()) {
            file << data << endl;
            file.close();
        }
        else {
            cout << "Failed to open the file!";
        }
    }

};

int main() {

    Block b0(0, "Genesis Block", "0");
    cout << "Block 0 Hash: " << b0.calculateHash() << endl;
    
    Block b1(1, "Ahmet -> Mehmet 5 BTC", b0.calculateHash());
    cout << "Block 1 Hash: " << b1.calculateHash() << endl;

    Block b2(2, "Mehmet -> Ahmet 2 BTC", b1.calculateHash());
    cout << "Block 2 Hash: " << b2.calculateHash() << endl;

    Block b3(3, "Ahmet -> Mehmet 15 BTC", b2.calculateHash());
    cout << "Block 3 Hash: " << b3.calculateHash() << endl;

}