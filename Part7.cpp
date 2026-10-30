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
//KHOI TAO LOP FINANCESTORAGE
class FinanceStorage {
public:
    void saveToCSV(vector<Transaction> transactions, string filename) {

        ofstream file(filename.c_str());

        if (!file.is_open()) {
            cout<<"Khong mo duoc file de ghi!"<<endl;
            return;
        }

        file<<"ID,Type,Amount,Category,Account,Date,Description\n";

        // GHI THONG TIN CAC GIAO DICH
        for (int i = 0; i < (int)transactions.size(); i++) {
            file<<transactions[i].id<<","
                <<transactions[i].type<<","
                <<transactions[i].amount<<","
                <<transactions[i].category<<","
                <<transactions[i].account<<","
                <<transactions[i].date<<","
                <<transactions[i].description<<"\n";
        }

        file.close();

        if (file.fail()) {
            cout<<"Co loi khi ghi file CSV!"<<endl;
        } else {
            cout<<"Da xuat du lieu thanh cong vao "
                <<filename<<"!"<<endl;
        }
    }

    // 2. TAI DANH SACH GIAO DICH TU FILE CSV
    vector<Transaction> loadFromCSV(string filename) {

        vector<Transaction> transactions;

        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout<<"Khong mo duoc file "<<filename<<"!"<<endl;
            return transactions;
        }

        string line;

        if (!getline(file, line)) {
            cout<<"File CSV rong!"<<endl;
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

            transactions.push_back(t);
        }

        file.close();

        cout<<"Da tai "<<transactions.size()
            <<" giao dich tu file CSV!"<<endl;

        return transactions;
    }
};

// HAM TIM MA GIAO DICH TIEP THEO
int getNextId(vector<Transaction> transactions) {
    int maxId = 0;

    for (int i = 0; i < (int)transactions.size(); i++) {
        if (transactions[i].id > maxId) {
            maxId = transactions[i].id;
        }
    }
    return maxId + 1;
}

int main() {
    FinanceStorage storage;

    vector<Transaction> transactions;

    string filename = "finance.csv";
    int choice;

    //MENU CHUC NANG
    do {
        cout<<"\n===== QUAN LY DU LIEU TAI CHINH ====="<<endl;
        cout<<"1. Tai du lieu tu file CSV"<<endl;
        cout<<"2. Nhap giao dich moi"<<endl;
        cout<<"3. Xuat du lieu ra file CSV"<<endl;
        cout<<"4. Hien thi danh sach giao dich"<<endl;
        cout<<"0. Thoat chuong trinh"<<endl;
        cout<<"Lua chon cua ban: ";
        cin>>choice;

        // 1. TAI DU LIEU TU FILE CSV
        if (choice == 1) {
            transactions = storage.loadFromCSV(filename);
        }

        // 2. NHAP CAC GIAO DICH MOI
        else if (choice == 2) {

            char again = 'y';

            while (again == 'y' || again == 'Y') {

                Transaction t;

                t.id = getNextId(transactions);

                cout<<"\nNhap loai (Income/Expense/RecurringExpense): ";
                cin>>t.type;

                if (t.type != "Income" &&
                    t.type != "Expense" &&
                    t.type != "RecurringExpense") {
                    cout << "Loai giao dich khong hop le!"<<endl;
                    continue;
                }

                while (true){
                cout<<"Nhap so tien ( viet bang so ): ";
                if(cin>>t.amount){
                	cin.ignore(10000, '\n');
                	break;
				}
				cout<<"So tien khong hop le! Vui long nhap lai!"<<endl;
				cin.clear();
                cin.ignore(10000, '\n');
				}

                cin.ignore(10000, '\n');

                cout<<"Nhap danh muc: ";
                getline(cin, t.category);

                cout<<"Nhap tai khoan: ";
                getline(cin, t.account);

                cout<<"Nhap ngay: ";
                getline(cin, t.date);

                cout<<"Nhap mo ta: ";
                getline(cin, t.description);

                transactions.push_back(t);

                cout<<"Da them giao dich thanh cong!"<<endl;

                cout<<"Nhap tiep giao dich? (y/n): ";
                cin>>again;
            }
        }

        // 3. XUAT DU LIEU RA FILE CSV
        else if (choice == 3) {
            storage.saveToCSV(transactions, filename);
        }

        // 4. HIEN THI DANH SACH GIAO DICH
        else if (choice == 4) {

            if (transactions.empty()) {
                cout<<"Danh sach rong. Hay tai du lieu hoac nhap giao dich!"
                    <<endl;
            } else {
                for (int i = 0; i < (int)transactions.size(); i++) {
                    cout<<"\nMa giao dich: "<<transactions[i].id;
                    cout<<"\nLoai giao dich: "<<transactions[i].type;
                    cout<<"\nSo tien: "<<transactions[i].amount;
                    cout<<"\nDanh muc: "<<transactions[i].category;
                    cout<<"\nTai khoan: "<<transactions[i].account;
                    cout<<"\nNgay giao dich: "<<transactions[i].date;
                    cout<<"\nMo ta: "<<transactions[i].description<<endl;
                }
            }
        }

        else if (choice != 0) {
            cout<<"Lua chon khong hop le!"<<endl;
        }

    } while (choice != 0);

    cout<<"Da thoat chuong trinh!"<<endl;
    return 0;
}
