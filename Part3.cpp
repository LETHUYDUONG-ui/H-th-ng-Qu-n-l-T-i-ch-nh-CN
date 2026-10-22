#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>

using namespace std;

class Budget {
private:
    string category;
    double limit;
    double spent;

public:
    Budget() : limit(0), spent(0) {}

    Budget(const string& categoryName, double budgetLimit)
        : category(categoryName), limit(budgetLimit), spent(0) {}

    bool addExpense(double amount) {
        if (amount <= 0) {
            return false;
        }
        spent += amount;
        return true;
    }

    bool isOverBudget() const {
        return spent > limit;
    }

    bool isAtLimit() const {
        return spent == limit;
    }

    double getRemaining() const {
        return limit - spent;
    }

    string getStatus() const {
        if (isOverBudget()) {
            return "CANH BAO: da vuot ngan sach";
        }
        if (isAtLimit()) {
            return "CANH BAO: da cham nguong ngan sach";
        }
        return "Trong han muc";
    }

    void printSummary() const {
        cout << left << setw(18) << category
             << right << setw(14) << fixed << setprecision(2) << limit
             << setw(14) << spent
             << setw(14) << getRemaining() << "  "
             << getStatus() << '\n';
    }
};

void ignoreInvalidInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    map<string, Budget> budgets;
    int choice;

    cout << "=== QUAN LY HAN MUC NGAN SACH THEO DANH MUC ===\n";

    do {
        cout << "\n1. Dat han muc cho danh muc\n"
             << "2. Them khoan chi\n"
             << "3. Xem canh bao ngan sach\n"
             << "0. Thoat\n"
             << "Lua chon: ";

        if (!(cin >> choice)) {
            ignoreInvalidInput();
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        if (choice == 1) {
            string category;
            double limit;
            cout << "Nhap danh muc: ";
            cin >> category;
            cout << "Nhap han muc: ";
            if (!(cin >> limit) || limit < 0) {
                ignoreInvalidInput();
                cout << "Han muc phai la so khong am.\n";
                continue;
            }
            budgets[category] = Budget(category, limit);
            cout << "Da dat han muc cho danh muc " << category << ".\n";
        } else if (choice == 2) {
            string category;
            double amount;
            cout << "Nhap danh muc: ";
            cin >> category;
            map<string, Budget>::iterator budget = budgets.find(category);
            if (budget == budgets.end()) {
                cout << "Danh muc chua duoc dat han muc.\n";
                continue;
            }

            cout << "Nhap so tien chi: ";
            if (!(cin >> amount) || !budget->second.addExpense(amount)) {
                ignoreInvalidInput();
                cout << "So tien chi phai lon hon 0.\n";
                continue;
           }
cout << budget->second.getStatus() << ". Con lai: "
                 << fixed << setprecision(2)
                 << budget->second.getRemaining() << '\n';
        } else if (choice == 3) {
            if (budgets.empty()) {
                cout << "Chua co danh muc nao.\n";
                continue;
            }

            cout << left << setw(18) << "Danh muc"
                 << right << setw(14) << "Han muc"
                 << setw(14) << "Da chi"
                 << setw(14) << "Con lai" << "  Trang thai\n";
            cout << string(80, '-') << '\n';
            map<string, Budget>::const_iterator item;
            for (item = budgets.begin(); item != budgets.end(); ++item) {
                item->second.printSummary();
            }
        } else if (choice != 0) {
            cout << "Lua chon khong hop le.\n";
        }
    } while (choice != 0);

    cout << "Da ket thuc chuong trinh.\n";
    return 0;
}
