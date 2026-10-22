#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class FinanceStorage {
public:

    // 1. XUAT DANH SACH GIAO DICH RA FILE CSV
    void saveToCSV(vector<BaseTransaction*> transactions,
                   string filename) {

        ofstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "Khong mo duoc file de ghi!\n";
            return;
        }

        // GHI TEN CAC COT
        file << "ID,Type,Amount,Category,Account,Date,Description\n";

        // GHI THONG TIN CAC GIAO DICH
        for (int i = 0; i < transactions.size(); i++) {
            BaseTransaction* t = transactions[i];

            file << t->getId() << ","
                 << t->getType() << ","
                 << t->getAmount() << ","
                 << t->getCategory() << ","
                 << t->getAccount() << ","
                 << t->getDate() << ","
                 << t->getDescription() << "\n";
        }

        file.close();

        cout << "Da xuat du lieu thanh cong!\n";
    }


    // 2. DOC DU LIEU TU FILE CSV
    vector<BaseTransaction*> loadFromCSV(string filename) {

        vector<BaseTransaction*> transactions;
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "Khong tim thay file de doc!\n";
            return transactions;
        }

        string line;

        // BO QUA DONG TIEU DE
        getline(file, line);

        // DOC CAC DONG DU LIEU
        while (getline(file, line)) {

            stringstream ss(line);

            string id, type, amount;
            string category, account, date, description;

            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, amount, ',');
            getline(ss, category, ',');
            getline(ss, account, ',');
            getline(ss, date, ',');
            getline(ss, description);

            // CHUYEN CHUOI THANH SO
            int transactionId = atoi(id.c_str());
            double transactionAmount = atof(amount.c_str());

            BaseTransaction* t = NULL;

            // TAO LAI DOI TUONG THEO LOAI GIAO DICH
            if (type == "Income") {
                t = new Income(transactionId, transactionAmount,
                               category, account, date, description);
            }
            else if (type == "Expense") {
                t = new Expense(transactionId, transactionAmount,
                                category, account, date, description);
            }
            else if (type == "RecurringExpense") {
                t = new RecurringExpense(transactionId, transactionAmount,
                                         category, account, date, description);
            }

            if (t != NULL) {
                transactions.push_back(t);
            }
        }

        file.close();

        cout << "Da tai du lieu thanh cong!\n";

        return transactions;
    }
};
