#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> // For _getch() to mask password input
#include <vector>
#include <regex>  // For email validation
#include <ctime>  // For date and time

using namespace std;

// Constants
const float FD_INTEREST_RATE = 0.06;   // 6% annual interest for FD
const float LOAN_INTEREST_RATE = 0.08; // 8% annual interest for loans
const float MIN_ANNUAL_INCOME_FOR_LOAN = 30000.0; // Minimum income for loan eligibility
const string ADMIN_USERNAME = "admin"; // Hardcoded admin username
const string ADMIN_PASSWORD = "admin123"; // Hardcoded admin password

// Function to get the current date and time as a string
string getCurrentDateTime() {
    time_t now = time(0);           // Get the current time
    tm *ltm = localtime(&now);      // Convert to local time
    char dateTime[20];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", ltm); // Format: YYYY-MM-DD HH:MM:SS
    return string(dateTime);
}

// Function to mask password input
string getPasswordInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // Read until Enter key is pressed
        if (ch == '\b') { // Handle backspace
            if (password.length() > 0) {
                password.pop_back();
                cout << "\b \b"; // Erase last character
            }
        } else if (ch >= 32 && ch <= 126) { // Printable characters
            password.push_back(ch);
            cout << '*'; // Display asterisk for each character
        }
    }
    cout << endl;
    return password;
}

// Function to validate mobile number
bool isValidMobileNumber(const string& mobileNumber) {
    if (mobileNumber.length() != 10) return false;
    for (char c : mobileNumber) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to validate Aadhaar number
bool isValidAadhaar(const string& aadhaar) {
    if (aadhaar.length() != 12) return false;
    for (char c : aadhaar) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to validate email address
bool isValidEmail(const string& email) {
    const regex pattern("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    return regex_match(email, pattern);
}

// Function to generate a unique account number (8 digits)
string generateAccountNumber() {
    static int accountNumber = 10000000; // Start from 10000000 to ensure 8 digits
    return to_string(accountNumber++);
}

// Function to generate IFSC code based on branch
string generateIFSC(string branch) {
    if (branch == "Amritsar") return "ABC001";
    if (branch == "Delhi") return "DEF002";
    if (branch == "Chandigarh") return "GHI003";
    if (branch == "Patna") return "JKL004";
    if (branch == "Bangalore") return "MNO005";
    return "UNK000"; // Unknown branch
}

// Function to generate a unique card number (16 digits)
string generateCardNumber() {
    static long long cardNumber = 4000090000300000; // Start from a sample 16-digit number
    return to_string(cardNumber++);
}

// Class for user accounts
class User {
private:
    string fullName;
    string fatherName;
    string motherName;
    string username;
    string password;
    string accountNumber;
    string mobileNumber;
    string email;
    string aadhaar;
    string dob;
    string accountType;
    string ifscCode;
    float balance;
    float loanAmount;
    int loanPeriod;  // Loan period in months (new)
    float fdAmount;
    int fdDuration;
    time_t fdStartDate;   // Start date of FD
    time_t loanStartDate; // Start date of loan
    vector<string> statement; // To store transaction statements

    // Card details
    string cardNumber;     // Stores card number
    bool isCardActive;     // Tracks whether the card is active

public:
    // Default constructor
    User()
        : fullName(""), fatherName(""), motherName(""), username(""), password(""), accountNumber(""),
          mobileNumber(""), email(""), aadhaar(""), dob(""), accountType(""), ifscCode(""),
          balance(0), loanAmount(0), loanPeriod(0), fdAmount(0), fdDuration(0), fdStartDate(0), loanStartDate(0),
          cardNumber(""), isCardActive(false) {}

    // Parameterized constructor
    User(string fName, string faName, string moName, string user, string pass, string accNum,
         string mobile, string email, string aadhaar, string dob, string accountType, string ifsc,
         float bal = 0)
        : fullName(fName), fatherName(faName), motherName(moName), username(user), password(pass),
          accountNumber(accNum), mobileNumber(mobile), email(email), aadhaar(aadhaar), dob(dob),
          accountType(accountType), ifscCode(ifsc), balance(bal), loanAmount(0), loanPeriod(0), fdAmount(0), fdDuration(0), 
          fdStartDate(0), loanStartDate(0), cardNumber(""), isCardActive(false) {}

    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getAccountNumber() const { return accountNumber; }
    float getBalance() { return balance + calculateFDInterest(); } // FD interest included in balance
    float getLoanAmount() { return loanAmount + calculateLoanInterest(); } // Loan interest added to amount
    int getLoanPeriod() const { return loanPeriod; } // Get loan period
    float getFDAmount() const { return fdAmount; }
    int getFDDuration() const { return fdDuration; }
    string getMobileNumber() const { return mobileNumber; }
    string getEmail() const { return email; }
    string getAadhaar() const { return aadhaar; }
    string getDOB() const { return dob; }
    string getAccountType() const { return accountType; }
    string getIFSC() const { return ifscCode; }
    string getFullName() const { return fullName; }
    string getFatherName() const { return fatherName; }
    string getMotherName() const { return motherName; }
    string getCardNumber() const { return cardNumber; }
    bool getCardStatus() const { return isCardActive; }

    // Setters
    void setPassword(string pass) { password = pass; }
    void setEmail(string newEmail) { email = newEmail; }
    void setMobileNumber(string newMobile) { mobileNumber = newMobile; }

    // Admin can directly set account balance (for testing or correcting balance issues)
    void setBalance(float newBalance) { balance = newBalance; }

    // Card Management
    void applyCard() {
        if (isCardActive) {
            cout << "You already have an active card. Card Number: " << cardNumber << endl;
            return;
        }
        cardNumber = generateCardNumber();
        isCardActive = true;
        statement.push_back(getCurrentDateTime() + " - Card Applied: " + cardNumber);
        cout << "Card applied successfully! Your new card number is: " << cardNumber << endl;
    }

    void blockCard() {
        if (!isCardActive) {
            cout << "No active card to block.\n";
            return;
        }
        isCardActive = false;
        statement.push_back(getCurrentDateTime() + " - Card Blocked: " + cardNumber);
        cout << "Card blocked successfully. Card Number: " << cardNumber << endl;
    }

    void viewCardStatus() {
        if (isCardActive) {
            cout << "Your card is active. Card Number: " << cardNumber << endl;
        } else {
            cout << "You have no active card.\n";
        }
    }

   

    // Methods for deposit, withdraw, and transfer
    void deposit(float amount) {
        balance += amount;
        statement.push_back(getCurrentDateTime() + " - Deposited: " + to_string(amount));
    }

    bool withdraw(float amount) {
        if (amount > balance) return false;
        balance -= amount;
        statement.push_back(getCurrentDateTime() + " - Withdrawn: " + to_string(amount));
        return true;
    }

    bool transfer(float amount, User& recipient) {
        if (amount > balance) return false;
        balance -= amount;
        recipient.balance += amount;
        statement.push_back(getCurrentDateTime() + " - Transferred: " + to_string(amount) + " to Account " + recipient.accountNumber);
        recipient.statement.push_back(getCurrentDateTime() + " - Received: " + to_string(amount) + " from Account " + accountNumber);
        return true;
    }

    // Interest Calculations
    float calculateFDInterest() {
        if (fdAmount == 0) return 0;
        time_t currentTime = time(nullptr);
        double monthsPassed = difftime(currentTime, fdStartDate) / (60 * 60 * 24 * 30); // Convert seconds to months
        double interest = (fdAmount * FD_INTEREST_RATE / 12) * monthsPassed; // Simple interest
        return interest;
    }

    float calculateLoanInterest() {
        if (loanAmount == 0) return 0;
        time_t currentTime = time(nullptr);
        double monthsPassed = difftime(currentTime, loanStartDate) / (60 * 60 * 24 * 30); // Convert seconds to months
        double interest = (loanAmount * LOAN_INTEREST_RATE / 12) * monthsPassed; // Simple interest
        return interest;
    }

    // Loan operations with annual income and loan period check
    void applyForLoan(float loanAmount, float annualIncome, int period) {
        if (annualIncome < MIN_ANNUAL_INCOME_FOR_LOAN) {
            cout << "You are not eligible for a loan. Minimum annual income required is $" << MIN_ANNUAL_INCOME_FOR_LOAN << ".\n";
            return;
        }
        if (loanAmount <= 0) {
            cout << "Invalid loan amount.\n";
            return;
        }
        this->loanAmount += loanAmount;
        this->loanPeriod = period;  // Set the loan period
        loanStartDate = time(nullptr); // Mark the loan start date
        statement.push_back(getCurrentDateTime() + " - Loan Applied: " + to_string(loanAmount) + " for " + to_string(loanPeriod) + " months");
        cout << "Loan of " << loanAmount << " approved for " << loanPeriod << " months. Total Loan: " << getLoanAmount() << endl;
    }

    void repayLoan(float amount) {
        float totalLoanWithInterest = getLoanAmount();
        if (amount <= 0 || amount > balance) {
            cout << "Invalid amount or insufficient balance.\n";
            return;
        }
        if (amount > totalLoanWithInterest) {
            cout << "You cannot repay more than your loan amount.\n";
            return;
        }
        balance -= amount;
        loanAmount -= amount; // Decrease principal
        statement.push_back(getCurrentDateTime() + " - Loan Repaid: " + to_string(amount));
        cout << "Loan repaid. Remaining Loan: " << getLoanAmount() << ", Balance: " << balance << endl;
    }

    // Fixed Deposit (FD) operations
    void applyForFD(float amount, int duration) {
        if (amount <= 0 || duration <= 0) {
            cout << "Invalid amount or duration.\n";
            return;
        }
        if (amount > balance) {
            cout << "Insufficient balance to create FD.\n";
            return;
        }
        balance -= amount;
        fdAmount += amount;
        fdDuration = duration;
        fdStartDate = time(nullptr); // Mark the FD start date
        statement.push_back(getCurrentDateTime() + " - FD Created: " + to_string(amount) + " for " + to_string(duration) + " months");
        cout << "FD created. Amount: " << amount << ", Duration: " << duration << " months.\n";
    }

    void closeFD() {
        if (fdAmount == 0) {
            cout << "No FD to close.\n";
            return;
        }
        float interest = calculateFDInterest();
        balance += (fdAmount + interest);
        statement.push_back(getCurrentDateTime() + " - FD Closed: Amount Returned: " + to_string(fdAmount + interest) + " (Interest: " + to_string(interest) + ")");
        cout << "FD closed. Amount returned: " << fdAmount + interest << " (Interest: " << interest << ")\n";
        fdAmount = 0;
        fdDuration = 0;
    }

    // View and update user details
    void viewUserDetails() {
        cout << "\nUser Details:\n";
        cout << "Full Name: " << fullName << endl;
        cout << "Father's Name: " << fatherName << endl;
        cout << "Mother's Name: " << motherName << endl;
        cout << "Username: " << username << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Mobile Number: " << mobileNumber << endl;
        cout << "Email: " << email << endl;
        cout << "Aadhaar: " << aadhaar << endl;
        cout << "Date of Birth: " << dob << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "IFSC Code: " << ifscCode << endl;
        cout << "Balance: " << balance << endl;
        cout << "Loan Amount: " << loanAmount << endl;
        cout << "Loan Period: " << loanPeriod << " months\n"; // Display loan period
        cout << "FD Amount: " << fdAmount << endl;
        cout << "FD Duration: " << fdDuration << " months\n";
        cout << "Card Number: " << (isCardActive ? cardNumber : "No Active Card") << endl;
    }

    void updateUserDetails() {
        int choice;
        cout << "\nWhat would you like to update?\n1. Mobile Number\n2. Email\n3. Password\nChoose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string newMobile;
                cout << "Enter new mobile number: ";
                cin >> newMobile;
                setMobileNumber(newMobile);
                cout << "Mobile number updated successfully!\n";
                break;
            }
            case 2: {
                string newEmail;
                cout << "Enter new email: ";
                cin >> newEmail;
                setEmail(newEmail);
                cout << "Email updated successfully!\n";
                break;
            }
            case 3: {
                string newPassword;
                cout << "Enter new password: ";
                newPassword = getPasswordInput();
                setPassword(newPassword);
                cout << "Password updated successfully!\n";
                break;
            }
            default:
                cout << "Invalid option.\n";
        }
    }

    // File handling: Save user data to file
    void saveToFile(ofstream& ofs) {
        ofs << fullName << "|" << fatherName << "|" << motherName << "|" << username << "|" << password << "|"
            << accountNumber << "|" << mobileNumber << "|" << email << "|" << aadhaar << "|" << dob << "|"
            << accountType << "|" << ifscCode << "|" << balance << "|" << loanAmount << "|" << loanPeriod << "|" << fdAmount << "|" 
            << fdDuration << "|" << fdStartDate << "|" << loanStartDate << "|" << cardNumber << "|" << isCardActive << "\n";
    }

    // File handling: Load user data from file
    void loadFromFile(ifstream& ifs) {
        getline(ifs, fullName, '|');
        getline(ifs, fatherName, '|');
        getline(ifs, motherName, '|');
        getline(ifs, username, '|');
        getline(ifs, password, '|');
        getline(ifs, accountNumber, '|');
        getline(ifs, mobileNumber, '|');
        getline(ifs, email, '|');
        getline(ifs, aadhaar, '|');
        getline(ifs, dob, '|');
        getline(ifs, accountType, '|');
        getline(ifs, ifscCode, '|');
        ifs >> balance;
        ifs.ignore(1, '|');
        ifs >> loanAmount;
        ifs.ignore(1, '|');
        ifs >> loanPeriod;  // Load loan period
        ifs.ignore(1, '|');
        ifs >> fdAmount;
        ifs.ignore(1, '|');
        ifs >> fdDuration;
        ifs.ignore(1, '|');
        ifs >> fdStartDate;
        ifs.ignore(1, '|');
        ifs >> loanStartDate;
        ifs.ignore(1, '|');
        getline(ifs, cardNumber, '|');
        ifs >> isCardActive;
        ifs.ignore(1, '\n');
    }

    // Function to display transaction statement
    void viewStatement() {
        cout << "\nAccount Statement for " << fullName << " (" << accountNumber << "):\n";
        for (const string& entry : statement) {
            cout << entry << endl;
        }
        if (statement.empty()) {
            cout << "No transactions yet.\n";
        }
    }
};

// Function to save all users to file
void saveAllUsers(User *users, int userCount) {
    ofstream ofs("users.txt", ios::trunc); // Open in trunc mode to overwrite
    if (!ofs) {
        cout << "Error: Unable to open file for saving user data.\n";
        return;
    }
    for (int i = 0; i < userCount; ++i) {
        users[i].saveToFile(ofs);
    }
    ofs.close();
    cout << "All user data saved successfully.\n";
}

// Function to load all users from file
void loadAllUsers(User *users, int &userCount) {
    ifstream ifs("users.txt");
    if (!ifs) {
        cout << "No user data file found, starting fresh.\n";
        return;
    }
    userCount = 0;
    while (!ifs.eof() && ifs.peek() != EOF) {
        users[userCount].loadFromFile(ifs);
        userCount++;
    }
    if (userCount == 0) {
        cout << "Error: Failed to load any user data from file.\n";
    }
    ifs.close();
}

// Function to check if a username is unique
bool isUniqueUsername(User *users, int userCount, const string &username) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].getUsername() == username) {
            return false; // Username already exists
        }
    }
    return true;
}

// Function to create account with a minimum deposit of 1000
void createAccount(User *users, int &userCount) {
    string fullName, fatherName, motherName, username, password, accountNumber;
    string mobileNumber, email, aadhaar, dob, accountType, branch;
    float initialDeposit;

    cout << "Enter full name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, fullName);
    cout << "Enter father's name: ";
    getline(cin, fatherName);
    cout << "Enter mother's name: ";
    getline(cin, motherName);

    // Username validation
    do {
        cout << "Enter username: ";
        cin >> username;
        if (!isUniqueUsername(users, userCount, username)) {
            cout << "Username already taken. Please choose a different username.\n";
        }
    } while (!isUniqueUsername(users, userCount, username));

    cout << "Enter password: ";
    password = getPasswordInput();
    accountNumber = generateAccountNumber();

    // Validate mobile number
    do {
        cout << "Enter mobile number (10 digits): ";
        cin >> mobileNumber;
        if (!isValidMobileNumber(mobileNumber)) {
            cout << "Invalid mobile number. Please enter exactly 10 digits.\n";
        }
    } while (!isValidMobileNumber(mobileNumber));

    // Validate email
    do {
        cout << "Enter email: ";
        cin >> email;
        if (!isValidEmail(email)) {
            cout << "Invalid email. Please enter a valid email address.\n";
        }
    } while (!isValidEmail(email));

    // Validate Aadhaar number
    do {
        cout << "Enter Aadhaar number (12 digits): ";
        cin >> aadhaar;
        if (!isValidAadhaar(aadhaar)) {
            cout << "Invalid Aadhaar number. Please enter exactly 12 digits.\n";
        }
    } while (!isValidAadhaar(aadhaar));

    cout << "Enter date of birth (YYYY-MM-DD): ";
    cin >> dob;
    cout << "Enter account type (Savings/Current): ";
    cin >> accountType;

    cout << "Enter branch name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, branch);
    string ifscCode = generateIFSC(branch);

    cout << "Enter initial deposit (min 1000): ";
    cin >> initialDeposit;

    if (initialDeposit < 1000) {
        cout << "Minimum deposit is 1000. Account not created.\n";
        return;
    }

    users[userCount++] = User(fullName, fatherName, motherName, username, password, accountNumber,
                               mobileNumber, email, aadhaar, dob, accountType, ifscCode, initialDeposit);
    cout << "Account created successfully! Account Number: " << accountNumber << ", IFSC Code: " << ifscCode << endl;
}

// Function to login a user
User* loginUser(User *users, int userCount) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    password = getPasswordInput();

    for (int i = 0; i < userCount; i++) {
        if (users[i].getUsername() == username && users[i].getPassword() == password) {
            return &users[i]; // Return pointer to the logged-in user
        }
    }
    cout << "Invalid username or password.\n";
    return nullptr; // Return nullptr if login fails
}

// Admin login and options
bool adminLogin() {
    string username, password;
    cout << "Admin Login:\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    password = getPasswordInput();

    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        cout << "Admin login successful!\n";
        return true;
    }
    cout << "Invalid admin credentials!\n";
    return false;
}

// Admin options: view all users, update a user, delete a user
void adminOptions(User *users, int &userCount) {
    while (true) {
        cout << "\nAdmin Menu:\n1. View All Users\n2. Update User\n3. Delete User\n4. Logout\nChoose an option: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "All User Accounts:\n";
                for (int i = 0; i < userCount; i++) {
                    cout << "\n--- User " << (i + 1) << " ---\n";
                    users[i].viewUserDetails();
                }
                break;
            }
            case 2: {
                string accNumber;
                cout << "Enter the account number of the user to update: ";
                cin >> accNumber;
                bool found = false;

                for (int i = 0; i < userCount; i++) {
                    if (users[i].getAccountNumber() == accNumber) {
                        cout << "Updating details for user: " << users[i].getFullName() << endl;
                        users[i].updateUserDetails();
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "User with account number " << accNumber << " not found.\n";
                }
                break;
            }
            case 3: {
                string accNumber;
                cout << "Enter the account number of the user to delete: ";
                cin >> accNumber;
                bool found = false;

                for (int i = 0; i < userCount; i++) {
                    if (users[i].getAccountNumber() == accNumber) {
                        // Delete user by shifting array elements
                        for (int j = i; j < userCount - 1; j++) {
                            users[j] = users[j + 1];
                        }
                        userCount--; // Reduce user count
                        cout << "User account deleted successfully.\n";
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "User with account number " << accNumber << " not found.\n";
                }
                break;
            }
            case 4:
                cout << "Logging out of admin mode...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Main function
int main() {
    User users[100]; // Array to store user accounts
    int userCount = 0;

    // Load existing user data from file
    loadAllUsers(users, userCount);

    // Example menu-driven interface
    while (true) {
        cout << "\n1. Create Account\n2. Login as User\n3. Admin Login\n4. Exit\nChoose an option: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount(users, userCount);
                 saveAllUsers(users, userCount);
                break;
            case 2: {
                User* loggedInUser = loginUser(users, userCount);
                if (loggedInUser) {
                    cout << "Login successful! Welcome, " << loggedInUser->getFullName() << endl;

                    // Options for logged-in user
                    while (true) {
                        cout << "\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Apply for Loan\n"
                             << "6. Repay Loan\n7. Apply for FD\n8. Close FD\n9. View Statement\n10. View User Details\n"
                             << "11. Update User Details\n12. Apply for Card\n13. Block Card\n14. View Card Status\n"
                             << "15.Logout\nChoose an option: ";
                        int userChoice;
                        cin >> userChoice;

                        switch (userChoice) {
                            case 1:
                                cout << "Your balance is: " << loggedInUser->getBalance() << endl;
                                break;
                            case 2: {
                                float depositAmount;
                                cout << "Enter amount to deposit: ";
                                cin >> depositAmount;
                                loggedInUser->deposit(depositAmount);
                                break;
                            }
                            case 3: {
                                float withdrawAmount;
                                cout << "Enter amount to withdraw: ";
                                cin >> withdrawAmount;
                                if (loggedInUser->withdraw(withdrawAmount)) {
                                    cout << "Withdrawal successful.\n";
                                } else {
                                    cout << "Insufficient balance.\n";
                                }
                                break;
                            }
                            case 4: {
                                string recipientAccount;
                                float transferAmount;
                                cout << "Enter recipient account number: ";
                                cin >> recipientAccount;
                                cout << "Enter amount to transfer: ";
                                cin >> transferAmount;

                                User* recipient = nullptr;
                                for (int i = 0; i < userCount; i++) {
                                    if (users[i].getAccountNumber() == recipientAccount) {
                                        recipient = &users[i];
                                        break;
                                    }
                                }

                                if (recipient && loggedInUser->transfer(transferAmount, *recipient)) {
                                    cout << "Transfer successful.\n";
                                } else {
                                    cout << "Transfer failed. Please check recipient account and balance.\n";
                                }
                                break;
                            }
                            case 5: {
                                float loanAmount, annualIncome;
                                int loanPeriod;
                                cout << "Enter your annual income: ";
                                cin >> annualIncome;
                                cout << "Enter loan amount to apply: ";
                                cin >> loanAmount;
                                cout << "Enter loan period (in months): ";
                                cin >> loanPeriod;
                                loggedInUser->applyForLoan(loanAmount, annualIncome, loanPeriod);
                                break;
                            }
                            case 6: {
                                float repayAmount;
                                cout << "Enter loan amount to repay: ";
                                cin >> repayAmount;
                                loggedInUser->repayLoan(repayAmount);
                                break;
                            }
                            case 7: {
                                float fdAmount;
                                int duration;
                                cout << "Enter FD amount: ";
                                cin >> fdAmount;
                                cout << "Enter FD duration (in months): ";
                                cin >> duration;
                                loggedInUser->applyForFD(fdAmount, duration);
                                break;
                            }
                            case 8:
                                loggedInUser->closeFD();
                                break;
                            case 9:
                                loggedInUser->viewStatement();
                                break;
                            case 10:
                                loggedInUser->viewUserDetails();
                                break;
                            case 11:
                                loggedInUser->updateUserDetails();
                                break;
                            case 12:
                                loggedInUser->applyCard();
                                break;
                            case 13:
                                loggedInUser->blockCard();
                                break;
                            case 14:
                                loggedInUser->viewCardStatus();
                                break;
                            case 15:
                               
                                cout << "Logging out...\n";
                                goto endLogin; // Exit the inner loop
                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                    }
                }
                endLogin: // Label to exit the inner loop
                break;
            }
            case 3: // Admin login
                if (adminLogin()) {
                    adminOptions(users, userCount); // Go to admin menu
                }
                break;
            case 4:
                // Save all users to file before exiting
                saveAllUsers(users, userCount);
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    }
}
