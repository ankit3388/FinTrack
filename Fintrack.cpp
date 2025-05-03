#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <deque>
#include <string>

using namespace std;

const long long MONEY_LIMIT = 1'000'000'000LL;

struct User {
    long long balance = 0;
    long long points = 0;
    deque<pair<long long, long long>> paymentHistory; // Store payment and earned points by month
};

unordered_map<string, User> users;
int validityPeriod; // in months
int redemptionRate; // in percentage

void registerUser(const string& userId) {
    if (users.count(userId)) {
        cout << "register-user: same user ID" << endl;
    } else {
        users[userId] = User();
        users[userId].points = 500;
        cout << "register-user: success. ID = " << userId << endl;
    }
}

void deposit(const string& userId, long long amount) {
    if (!users.count(userId)) {
        cout << "deposit: invalid user ID" << endl;
        return;
    }
    if (users[userId].balance + amount > MONEY_LIMIT) {
        cout << "deposit: exceed balance limit" << endl;
        return;
    }
    users[userId].balance += amount;
    cout << "deposit: success. balance = " << users[userId].balance << endl;
}

void payment(const string& userId, long long amount) {
    if (!users.count(userId)) {
        cout << "payment: invalid user ID" << endl;
        return;
    }

    long long initialPoints = users[userId].points;
    long long totalMoney = users[userId].balance + users[userId].points;

    if (totalMoney < amount) {
        cout << "payment: insufficient balance" << endl;
    } else {
        if (initialPoints >= amount) {
            users[userId].points -= amount;
            long long extraPoints = (amount * redemptionRate) / 100;
            users[userId].points += extraPoints;
            users[userId].paymentHistory.push_front({0, amount}); // 0 money spent, all points used
        } else {
            long long remainingAmount = amount - initialPoints;
            users[userId].points = 0;
            users[userId].balance -= remainingAmount;
            long long extraPoints = (amount * redemptionRate) / 100;
            users[userId].points += extraPoints;
            users[userId].paymentHistory.push_front({remainingAmount, initialPoints}); // money spent and points used
        }

        cout << "payment: success." << endl;
    }

    // Limit history to the last 12 months
    if (users[userId].paymentHistory.size() > 12) {
        users[userId].paymentHistory.pop_back();
    }
}

void sendMoney(const string& sourceId, const string& destId, long long amount) {
    if (!users.count(sourceId)) {
        cout << "send-money: invalid source user ID" << endl;
        return;
    }
    if (!users.count(destId)) {
        cout << "send-money: invalid destination user ID" << endl;
        return;
    }
    if (users[sourceId].balance < amount) {
        cout << "send-money: insufficient balance" << endl;
        return;
    }
    if (users[destId].balance + amount > MONEY_LIMIT) {
        cout << "send-money: exceed balance limit" << endl;
        return;
    }
    users[sourceId].balance -= amount;
    users[destId].balance += amount;
    cout << "send-money: success. source balance = " << users[sourceId].balance << ", destination balance = " << users[destId].balance << endl;
}

void showBalance(const string& userId) {
    if (!users.count(userId)) {
        cout << "show-balance: invalid user ID" << endl;
        return;
    }
    cout << "show-balance: money = " << users[userId].balance << ", point = " << users[userId].points << ", expiration month = ";
    if (validityPeriod > 0 && users[userId].points > 0) {
        cout << "2099-12";
    } else {
        cout << "----/--";
    }
    cout << endl;
}

void changeReturn(int newRate) {
    redemptionRate = newRate;
    cout << "change-return: success." << endl;
}

void showSpent(const string& userId) {
    if (!users.count(userId)) {
        cout << "show-spent: invalid user ID" << endl;
        return;
    }

    cout << "show-spent: " << userId << endl;
    // Iterate through the last 12 months (or less if not available)
    for (int i = 0; i < users[userId].paymentHistory.size(); ++i) {
        long long paymentAmount = users[userId].paymentHistory[i].first;
        long long earnedPoints = users[userId].paymentHistory[i].second;
        cout << i << ". " << paymentAmount << " " << earnedPoints << endl;
    }
    // Fill the remaining months with 0 if less than 12 months of history
    int monthsLeft = 12 - users[userId].paymentHistory.size();
    for (int i = 0; i < monthsLeft; ++i) {
        cout << "0 0" << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin >> validityPeriod >> redemptionRate;
    
    string queryType, time, userId, destId;
    long long amount;
    int newRate;

    for (int i = 0; i < n; ++i) {
        cin >> queryType >> time;

        if (queryType == "register-user:") {
            cin >> userId;
            registerUser(userId);
        }
        else if (queryType == "deposit:") {
            cin >> userId >> amount;
            deposit(userId, amount);
        }
        else if (queryType == "payment:") {
            cin >> userId >> amount;
            payment(userId, amount);
        }
        else if (queryType == "send-money:") {
            cin >> userId >> destId >> amount;
            sendMoney(userId, destId, amount);
        }
        else if (queryType == "show-balance:") {
            cin >> userId;
            showBalance(userId);
        }
        else if (queryType == "change-return:") {
            cin >> newRate;
            changeReturn(newRate);
        }
        else if (queryType == "show-spent:") {
            cin >> userId;
            showSpent(userId);
        }
    }
    
    return 0;
}
