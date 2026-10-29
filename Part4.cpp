
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

class BaseTransaction {
protected:
    string maGD;
    double soTien;
    string danhMuc;
    string moTa;
    string ngayTao;

public:
    BaseTransaction(string id, double tien, string dm,
                    string desc = "", string date = "")
        : maGD(id), soTien(tien), danhMuc(dm),
          moTa(desc), ngayTao(date) {}

    virtual ~BaseTransaction() {}

    virtual double laySoTienThucTe() const = 0;

    string getMaGD() const { return maGD; }
    double getSoTien() const { return soTien; }
    string getDanhMuc() const { return danhMuc; }
    string getMoTa() const { return moTa; }
};

class ExpenseTransaction : public BaseTransaction {
public:
    ExpenseTransaction(string id, double tien, string dm,
                       string desc = "", string date = "")
        : BaseTransaction(id, tien, dm, desc, date) {}

    double laySoTienThucTe() const override {
        return -soTien;
    }
};

class RecurringExpenseTransaction : public ExpenseTransaction {
private:
    string chuKy;

public:
    RecurringExpenseTransaction(string id, double tien, string dm,
                                string ck = "Hang thang",
                                string desc = "")
        : ExpenseTransaction(id, tien, dm, desc),
          chuKy(ck) {}

    string getChuKy() const {
        return chuKy;
    }
};

class Account {
private:
    string maTK;
    string tenTK;
    string loaiTK;
    double soDu;

public:
    Account(string id, string ten, string loai, double balance)
        : maTK(id), tenTK(ten), loaiTK(loai), soDu(balance) {}

    string getMaTK() const { return maTK; }
    string getTenTK() const { return tenTK; }

    bool thucHienGiaoDich(shared_ptr<BaseTransaction> gd) {
        if (!gd || soDu + gd->laySoTienThucTe() < 0) {
            cout << " -> [That bai] Tai khoan khong du tien!\n";
            return false;
        }

        soDu += gd->laySoTienThucTe();

        cout << " -> [Thanh cong] Tai khoan: " << tenTK
             << " | So du moi: "
             << fixed << setprecision(0) << soDu
             << " VND\n";

        return true;
    }

    void inThongTin() const {
        cout << "Ma TK: " << maTK
             << " | Ten: " << tenTK
             << " | Loai: " << loaiTK
             << " | So du: " << fixed << setprecision(0)
             << soDu << " VND\n";
    }
};

class Budget {
private:
    string category;
    double limit;
    double spent;

public:
    Budget() : limit(0), spent(0) {}

    Budget(string name, double maxAmount)
        : category(name), limit(maxAmount), spent(0) {}

    bool addExpense(double amount) {
        if (amount <= 0) return false;
        spent += amount;
        return true;
    }

    string getStatus() const {
        if (spent > limit)
            return "CANH BAO: da vuot ngan sach";

        if (spent == limit)
            return "CANH BAO: da cham nguong ngan sach";

        return "Trong han muc";
    }
};

// ==========================================
//  KIEM TRA THANG VA XU LY NHAP SAI
// ==========================================

bool isValidYearMonth(const string& month) {
    if (month.size() != 7 || month[4] != '-')
        return false;

    for (size_t i = 0; i < month.size(); i++) {
        if (i == 4) continue;

        if (!isdigit(static_cast<unsigned char>(month[i])))
            return false;
    }

    int year = stoi(month.substr(0, 4));
    int m = stoi(month.substr(5, 2));

    return year >= 1 && m >= 1 && m <= 12;
}

void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ==========================================
//  LUU THONG TIN LICH GIAO DICH
// ==========================================

struct ScheduledRule {
    shared_ptr<RecurringExpenseTransaction> templateTransaction;
    Account* targetAccount;
    string lastProcessedMonth;
};

// ==========================================
//  CLASS QUAN LY GIAO DICH DINH KY
// ==========================================

class TransactionScheduler {
private:
    vector<ScheduledRule> scheduledList;
    int autoIdCounter;

public:
    TransactionScheduler() : autoIdCounter(100) {}

    void registerRecurringTransaction(
        const shared_ptr<RecurringExpenseTransaction>& rExpense,
        Account* acc
    ) {
        if (!rExpense || !acc) {
            cout << "[Loi] Du lieu dang ky khong hop le!\n";
            return;
        }

        for (const auto& rule : scheduledList) {
            if (rule.templateTransaction->getMaGD() == rExpense->getMaGD()
                && rule.targetAccount->getMaTK() == acc->getMaTK()) {
                cout << "[Scheduler] Lich da duoc dang ky truoc do.\n";
                return;
            }
        }

        scheduledList.push_back({rExpense, acc, ""});

        cout << "[Scheduler] Da dang ky lich: "
             << rExpense->getMoTa()
             << " cho tai khoan "
             << acc->getTenTK() << "\n";
    }

    void showScheduledTransactions() const {
        cout << "\n--- DANH SACH CHI PHI DINH KY ---\n";

        if (scheduledList.empty()) {
            cout << "Chua co lich nao.\n";
            return;
        }

        for (const auto& rule : scheduledList) {
            cout << "Ma: "
                 << rule.templateTransaction->getMaGD()
                 << " | Khoan chi: "
                 << rule.templateTransaction->getMoTa()
                 << " | Danh muc: "
                 << rule.templateTransaction->getDanhMuc()
                 << " | So tien: "
                 << fixed << setprecision(0)
                 << rule.templateTransaction->getSoTien()
                 << " VND | Tai khoan: "
                 << rule.targetAccount->getTenTK()
                 << " | Chu ky: "
                 << rule.templateTransaction->getChuKy()
                 << '\n';
        }
    }

    void processMonthlyTransactions(
        const string& yearMonth,
        map<string, Budget>& budgets
    ) {
        if (!isValidYearMonth(yearMonth)) {
            cout << "[Loi] Thang khong hop le. "
                 << "Hay nhap YYYY-MM, vi du 2026-09.\n";
            return;
        }

        cout << "\n--- CHI PHI DINH KY THANG "
             << yearMonth << " ---\n";

        if (scheduledList.empty()) {
            cout << "Chua co lich chi phi dinh ky nao.\n";
            return;
        }

        for (auto& rule : scheduledList) {
            if (rule.lastProcessedMonth == yearMonth) {
                cout << "- Khoan '"
                     << rule.templateTransaction->getMoTa()
                     << "' da xu ly trong thang nay, bo qua.\n";
                continue;
            }

            string newId = "AUTO" + to_string(++autoIdCounter);

            auto actualTransaction = make_shared<ExpenseTransaction>(
                newId,
                rule.templateTransaction->getSoTien(),
                rule.templateTransaction->getDanhMuc(),
                "[Dinh ky] " + rule.templateTransaction->getMoTa(),
                yearMonth + "-01"
            );

            cout << "* Xu ly khoan chi: "
                 << rule.templateTransaction->getMoTa() << '\n';

            if (rule.targetAccount->thucHienGiaoDich(actualTransaction)) {
                rule.lastProcessedMonth = yearMonth;

                string category =
                    rule.templateTransaction->getDanhMuc();

                auto budget = budgets.find(category);

                if (budget != budgets.end()) {
                    budget->second.addExpense(
                        rule.templateTransaction->getSoTien()
                    );

                    cout << "  -> [Ngan sach] "
                         << budget->second.getStatus() << '\n';
                }
            }
        }
    }
};

// ==========================================
// CHUONG TRINH DEMO PHAN 4
// ==========================================

void demoPhan4() {
    Account mb("TK02", "MB Bank", "Ngan hang", 5000000);
    Account momo("TK03", "Vi MoMo", "Vi dien tu", 800000);

    auto tienTro = make_shared<RecurringExpenseTransaction>(
        "REC01", 2500000, "Tien nha",
        "Hang thang", "Tro co dinh hang thang"
    );

    auto spotify = make_shared<RecurringExpenseTransaction>(
        "REC02", 59000, "Giai tri",
        "Hang thang", "Goi Spotify Sinh Vien"
    );

    TransactionScheduler scheduler;

    scheduler.registerRecurringTransaction(tienTro, &mb);
    scheduler.registerRecurringTransaction(spotify, &momo);

    map<string, Budget> budgets;
    budgets["Tien nha"] = Budget("Tien nha", 5000000);
    budgets["Giai tri"] = Budget("Giai tri", 200000);

    int choice = -1;

    do {
        cout << "\n========================================\n"
             << " MENU PHAN 4 - GIAO DICH DINH KY\n"
             << "========================================\n"
             << "1. Xem tai khoan va so du\n"
             << "2. Xem danh sach chi phi dinh ky\n"
             << "3. Chay lap lich theo thang\n"
             << "4. Chay lai thang de kiem tra trung lap\n"
             << "0. Thoat\n"
             << "Lua chon: ";

        if (!(cin >> choice)) {
            clearBadInput();
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        if (choice == 1) {
            mb.inThongTin();
            momo.inThongTin();
        }
        else if (choice == 2) {
            scheduler.showScheduledTransactions();
        }
        else if (choice == 3 || choice == 4) {
            string month;

            cout << "Nhap thang (YYYY-MM): ";
            cin >> month;

            scheduler.processMonthlyTransactions(month, budgets);
        }
        else if (choice != 0) {
            cout << "Vui long chon tu 0 den 4.\n";
        }

    } while (choice != 0);
}

// ==========================================
//  HAM MAIN
// ==========================================

int main() {
    demoPhan4();
    return 0;
}
