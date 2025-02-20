#include <iostream>
#include <string>
#include <fstream>
#include <exception>

using namespace std;

// Exception for insufficient funds
class InsufficientFunds : public exception {
public:
    const char* what() const noexcept override {
        return "Insufficient funds for withdrawal.";
    }
};

// Account class definition
class Account {
private:
    string accountName;
    string accountNumber;
    int accountType;
    float accountBalance;

public:
    // Constructors
    Account() : accountName(""), accountNumber(""), accountType(0), accountBalance(0.0) {}
    Account(string name, string number, int type, float balance)
        : accountName(name), accountNumber(number), accountType(type), accountBalance(balance) {}

    // Getter for balance
    float getBalance() const {
        return accountBalance;
    }

    // Withdraw function
    void withdraw(float amount) {
        if (amount > accountBalance) {
            throw InsufficientFunds();
        }
        accountBalance -= amount;
    }

    // Deposit function
    void deposit(float amount) {
        accountBalance += amount;
    }

    // Print account details
    void printDetail() const {
        cout << "Account Holder's Name: " << accountName << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Current Balance: RM" << accountBalance << endl;
    }

    // Transfer function
    void transfer(Account &other, float amount) {
        if (amount > accountBalance) {
            throw InsufficientFunds();
        }
        this->withdraw(amount);
        other.deposit(amount);
    }

    // Function to get account number
    string getAccountNumber() const {
        return accountNumber;
    }

    // Friend function declarations
    friend void saveAccount(const Account &account);
    friend Account loadAccount(const string &accountNumber);
};

// Save account to a file
void saveAccount(const Account &account) {
    string filename = account.getAccountNumber() + ".bin";
    ofstream outFile(filename, ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(&account), sizeof(Account));
        outFile.close();
    } else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

// Load account from a file
Account loadAccount(const string &accountNumber) {
    Account account;
    string filename = accountNumber + ".bin";
    ifstream inFile(filename, ios::binary);
    if (inFile.is_open()) {
        inFile.read(reinterpret_cast<char*>(&account), sizeof(Account));
        inFile.close();
    } else {
        cerr << "Unable to open file for reading: " << filename << endl;
    }
    return account;
}

int main() {
    // Creating two accounts with hard-coded values
    Account account1;
    Account account2("Razali Ahmad", "123456", 1, 1000.0);

    // Print initial balance of both accounts
    cout << "INITIAL BALANCE" << endl;
    cout << "First Account balance: RM" << account1.getBalance() << endl;
    cout << "Second Account balance: RM" << account2.getBalance() << endl;

    // Deposit money into the first account
    account1.deposit(200.0);
    cout << "\nAFTER DEPOSITING RM200 INTO THE FIRST ACCOUNT" << endl;
    cout << "First Account balance: RM" << account1.getBalance() << endl;

    // Withdraw money from the second account
    try {
        account2.withdraw(200.0);
        cout << "\nAFTER WITHDRAWING RM200 FROM THE SECOND ACCOUNT" << endl;
        cout << "Second Account balance: RM" << account2.getBalance() << endl;
    } catch (const InsufficientFunds &e) {
        cout << e.what() << endl;
    }

    // Transfer money from the second account to the first account
    try {
        account2.transfer(account1, 100.0);
        cout << "\nAFTER TRANSFERING RM100 FROM THE SECOND ACCOUNT TO THE FIRST ACCOUNT" << endl;
        cout << "ACCOUNT INFORMATION" << endl;
        account1.printDetail();
        cout << "\nACCOUNT INFORMATION" << endl;
        account2.printDetail();
    } catch (const InsufficientFunds &e) {
        cout << e.what() << endl;
    }

    // Save both accounts to files
    saveAccount(account1);
    saveAccount(account2);

    // Load the second account into a third account object
    Account account3 = loadAccount("123456");

    // Print the details of the third account
    cout << "\nTHIRD ACCOUNT" << endl;
    cout << "ACCOUNT INFORMATION" << endl;
    account3.printDetail();

    return 0;
}
