#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

// LUU THONG TIN MOT GIAO DICH
struct Transaction {
    int id;
    string type;
    double amount;
    string category;
    string account;
    string date;
    string description;
};

// LOP QUAN LY VIEC XUAT VA TAI DU LIEU CSV
class FinanceStorage {
public:

    // 1. XUAT DANH SACH GIAO DICH RA FILE CSV
    void saveToCSV(vector<Transaction> transactions, string filename) {

        // MO FILE DE GHI DU LIEU
        ofstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "Khong mo duoc file de ghi!" << endl;
            return;
        }

        // GHI TEN CAC COT
        file << "ID,Type,Amount,Category,Account,Date,Description\n";

        // GHI THONG TIN TUNG GIAO DICH
        for (int i = 0; i < (int)transactions.size(); i++) {
            file << transactions[i].id << ","
                 << transactions[i].type << ","
                 << transactions[i].amount << ","
                 << transactions[i].category << ","
                 << transactions[i].account << ","
                 << transactions[i].date << ","
                 << transactions[i].description << "\n";
        }

        // DONG FILE VA KIEM TRA KET QUA
        file.close();

        if (file.fail()) {
            cout << "Co loi khi ghi file CSV!" << endl;
        } else {
            cout << "Da xuat du lieu thanh cong vao "
                 << filename << "!" << endl;
        }
    }

    // 2. TAI DANH SACH GIAO DICH TU FILE CSV
    vector<Transaction> loadFromCSV(string filename) {

        // TAO DANH SACH LUU CAC GIAO DICH DOC DUOC
        vector<Transaction> transactions;

        // MO FILE DE DOC DU LIEU
        ifstream file(filename.c_str());

        // KIEM TRA FILE CO MO THANH CONG KHONG
        if (!file.is_open()) {
            cout << "Khong mo duoc file " << filename << "!" << endl;
            return transactions;
        }

        string line;

        // BO QUA DONG TIEU DE
        if (!getline(file, line)) {
            cout << "File CSV rong!" << endl;
            file.close();
            return transactions;
        }

        // DOC TUNG DONG DU LIEU
        while (getline(file, line)) {

            stringstream ss(line);

            string id, type, amount;
            string category, account, date, description;

            // TACH CAC TRUONG DU LIEU THEO DAU PHAY
            if (!getline(ss, id, ',') ||
                !getline(ss, type, ',') ||
                !getline(ss, amount, ',') ||
                !getline(ss, category, ',') ||
                !getline(ss, account, ',') ||
                !getline(ss, date, ',')) {
                continue;
            }

            // DOC PHAN MO TA CON LAI CUA DONG
            getline(ss, description);

            // CHUYEN DU LIEU SANG DANG SO
            Transaction t;
            t.id = atoi(id.c_str());
            t.type = type;
            t.amount = atof(amount.c_str());
            t.category = category;
            t.account = account;
            t.date = date;
            t.description = description;

            // THEM GIAO DICH VAO DANH SACH
            transactions.push_back(t);
        }

        // DONG FILE SAU KHI DOC
        file.close();

        cout << "Da tai " << transactions.size()
             << " giao dich tu file CSV!" << endl;

        return transactions;
    }
};

// HAM TIM MA GIAO DICH TIEP THEO
int getNextId(vector<Transaction> transactions) {
    int maxId = 0;

    // TIM MA GIAO DICH LON NHAT
    for (int i = 0; i < (int)transactions.size(); i++) {
        if (transactions[i].id > maxId) {
            maxId = transactions[i].id;
        }
    }

    return maxId + 1;
}

int main() {

    // TAO DOI TUONG QUAN LY DU LIEU
    FinanceStorage storage;

    // KHAI BAO DANH SACH GIAO DICH
    vector<Transaction> transactions;

    string filename = "finance.csv";
    int choice;

    // HIEN THI MENU CHUC NANG
    do {
        cout << "\n===== QUAN LY DU LIEU TAI CHINH =====" << endl;
        cout << "1. Tai du lieu tu file CSV" << endl;
        cout << "2. Nhap giao dich moi" << endl;
        cout << "3. Xuat du lieu ra file CSV" << endl;
        cout << "4. Hien thi danh sach giao dich" << endl;
        cout << "0. Thoat chuong trinh" << endl;
        cout << "Lua chon cua ban: ";
        cin >> choice;

        // 1. TAI DU LIEU TU FILE CSV
        if (choice == 1) {
            transactions = storage.loadFromCSV(filename);
        }

        // 2. NHAP CAC GIAO DICH MOI
        else if (choice == 2) {

            char again = 'y';

            while (again == 'y' || again == 'Y') {

                Transaction t;

                // TU DONG TAO MA GIAO DICH TIEP THEO
                t.id = getNextId(transactions);

                // NHAP LOAI GIAO DICH
                cout << "\nNhap loai (Income/Expense/RecurringExpense): ";
                cin >> t.type;

                // KIEM TRA LOAI GIAO DICH
                if (t.type != "Income" &&
                    t.type != "Expense" &&
                    t.type != "RecurringExpense") {
                    cout << "Loai giao dich khong hop le!" << endl;
                    continue;
                }

                // NHAP SO TIEN
                cout << "Nhap so tien: ";
                cin >> t.amount;

                // XOA KY TU XUONG DONG CON LAI
                cin.ignore(10000, '\n');

                // NHAP DANH MUC
                cout << "Nhap danh muc: ";
                getline(cin, t.category);

                // NHAP TAI KHOAN
                cout << "Nhap tai khoan: ";
                getline(cin, t.account);

                // NHAP NGAY GIAO DICH
                cout << "Nhap ngay (YYYY-MM-DD): ";
                getline(cin, t.date);

                // NHAP MO TA
                cout << "Nhap mo ta: ";
                getline(cin, t.description);

                // THEM GIAO DICH VAO DANH SACH
                transactions.push_back(t);

                cout << "Da them giao dich thanh cong!" << endl;

                // HOI CO MUON NHAP TIEP KHONG
                cout << "Nhap tiep giao dich? (y/n): ";
                cin >> again;
            }
        }

        // 3. XUAT DU LIEU RA FILE CSV
        else if (choice == 3) {
            storage.saveToCSV(transactions, filename);
        }

        // 4. HIEN THI DANH SACH GIAO DICH
        else if (choice == 4) {

            if (transactions.empty()) {
                cout << "Danh sach rong. Hay tai du lieu hoac nhap giao dich!"
                     << endl;
            } else {
                for (int i = 0; i < (int)transactions.size(); i++) {
                    cout << "\nMa giao dich: " << transactions[i].id;
                    cout << "\nLoai giao dich: " << transactions[i].type;
                    cout << "\nSo tien: " << transactions[i].amount;
                    cout << "\nDanh muc: " << transactions[i].category;
                    cout << "\nTai khoan: " << transactions[i].account;
                    cout << "\nNgay giao dich: " << transactions[i].date;
                    cout << "\nMo ta: " << transactions[i].description << endl;
                }
            }
        }

        // KIEM TRA LUA CHON
        else if (choice != 0) {
            cout << "Lua chon khong hop le!" << endl;
        }

    } while (choice != 0);

    cout << "Da thoat chuong trinh!" << endl;

    return 0;
}
