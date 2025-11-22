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

// --- to be able to use sha256 algorithm ---
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

    string getHash() {
        return blockHash;
    }

    void transactionLog(string data) {
        ofstream file("transactions.log", ios::app);
        if (file.is_open()) {
            file << data << ", Hash: " << getHash() << endl << endl;
            file.close();
        }
        else {
            cout << "Failed to open the file!";
        }
    }

};

//class User {
//private:
//    float balance,coins;
//    string name;
//public:
//    User(float blnc, float coinx, string namex) {
//        balance = blnc;
//        coins = coinx;
//        name = namex;
//    }
//};

string depositWithdrawMoney(float *initialBalance) {
    float deposit;
    char button;
    cout << "\n1 - Deposit Money\n2 - Withdraw Money\n3 - Menu\n\n";
    cin >> button;

    switch (button)
    {
    case '1': {
        cout << "\nEnter the amount you want to deposit: ";
        cin >> deposit;
        *initialBalance += deposit;
        stringstream message;
        message << "Deposited -> MyAccount " << deposit << " dollars.";
        return message.str();
    }

    case '2': {
        cout << "\nEnter the amount you want to withdraw: ";
        cin >> deposit;
        if (deposit > *initialBalance) {
            cout << "\nNot enough balance!\n";
            break;
        }
        *initialBalance -= deposit;
        stringstream message;
        message << "MyAccount -> Withdrawed " << deposit << " dollars.";
        return message.str();
    }

    case '3':
        return"";

    default:
        return"";
    }
}

string buySellCoin(float *initialBalance, float *initialCoins) {
    float coinPrice = 3.5,amount = 0;
    char button;

    cout << "\nCoin Price: " << coinPrice << "\nBalance: " << *initialBalance << "\nCoins: " << *initialCoins << "\n\n1 - Buy\n2 - Sell\n3 - Menu\n\n";
    cin >> button;

    switch (button)
    {
    case '1': {
        cout << "\nEnter the coin amount you want to buy: ";
        cin >> amount;
        if (amount * coinPrice > *initialBalance) {
            cout << "\nNot enough balance!\n";
            break;
        }
        *initialBalance = *initialBalance - (amount * coinPrice);
        *initialCoins = *initialCoins + amount;
        stringstream message;
        message << "User -> MyAccount " << amount << " coins.";
        return message.str();
    }

    case '2': {
        cout << "\nEnter the coin amount you want to sell: ";
        cin >> amount;
        if (amount > *initialCoins) {
            cout << "\nNot enough coins!\n";
            break;
        }
        *initialBalance = *initialBalance + (amount * coinPrice);
        *initialCoins = *initialCoins - amount;
        stringstream message;
        message << "MyAccount -> User " << amount << " coins.";
        return message.str();
    }

    case'3':
        return"";

    default:
        return"";
    }
}

void saveChanges(float initialBalance, float initialCoins) {

    ofstream fileClear("asset_information.txt", ios::trunc);
    if (fileClear.is_open()) {
        fileClear.clear();
        fileClear.close();
    }
    else {
        cout << "\nFailed to clear old informations!\n";
    }

    ofstream file("asset_information.txt");
    if (file.is_open()) {
        file << initialBalance << endl << initialCoins;
        file.close();
    }
    else {
        cout << "\nFailed to save changes!\n";
    }
}



int main() {

    ofstream clearTransactions("transactions.log", ios::trunc);
    clearTransactions.clear();

    string fileName = "asset_information.txt";
    ifstream control(fileName);

    if (!control.is_open()) {
        ofstream creator(fileName);
        creator << "0" << endl << "0";
        creator.close();
        cout << "New save file created.\n\n\n" << endl;
    }
    else {
        control.close();
    }

    string informations[5];  
    int i = 0;

    ifstream file("asset_information.txt");

    if (file.is_open()) {
        while (getline(file, informations[i])) {
            i++;
        }
        file.close();
    }
    else {
        cout << "\nFailed to read account informations!\n";
    }

    float balance = stof(informations[0]), coins = stof(informations[1]);
    char button;
    int blockIndex = 0;

    //starter block
    Block b0(blockIndex, "Genesis Block", "0");
    cout << "Block 0 Hash: " << b0.calculateHash() << endl;

    vector<Block>blockVector = {b0};

    do {
        cout << "\n__Control Panel__\n\n" << "1 - Deposit/Withdraw Money\n2 - Buy/Sell Coin\n3 - Quit\n\nBalance: " << balance << endl <<"Coins: " << coins << "\n\n";
        cin >> button;

        switch (button)
        {
        case '1': {
            string result = depositWithdrawMoney(&balance);

            //to prevent creating a hash if no operation made
            if (result == "") {
                continue;
            }

            blockIndex++;
            Block currentBlock(blockIndex, result, blockVector.back().getHash());
            blockVector.push_back(currentBlock);
            continue;
        }
        case '2': {
            string result = buySellCoin(&balance, &coins);
            if (result == "") {
                continue;
            }

            blockIndex++;
            Block currentBlock(blockIndex, result, blockVector.back().getHash());
            blockVector.push_back(currentBlock);
            continue;
        }
        case '3': {
            saveChanges(balance, coins);
            cout << "\nChanges Saved. Goodbye.";
            continue;
        }
        default:
            continue;
        }

    } while (button != '3');
}
