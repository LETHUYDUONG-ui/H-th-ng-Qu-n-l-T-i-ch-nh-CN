#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// ==========================================
// HỢP NHẤT: CÁC CLASS GIAO DỊCH (Dùng chung)
// ==========================================
class BaseTransaction {
protected:
    string maGD;
    double soTien;
    string danhMuc;
    string moTa;
    string ngayTao;

public:
    BaseTransaction(string id, double tien, string dm, string desc = "", string dt = "2026-09-14")
        : maGD(id), soTien(tien), danhMuc(dm), moTa(desc), ngayTao(dt) {}

    virtual ~BaseTransaction() = default;

    virtual double laySoTienThucTe() const = 0;
    virtual string layLoaiGiaoDich() const = 0;

    string getMaGD() const { return maGD; }
    double getSoTien() const { return soTien; }
    string getDanhMuc() const { return danhMuc; }
    string getMoTa() const { return moTa; }
    string getNgayTao() const { return ngayTao; }

    virtual void hienThi() const {
        cout << "[" << ngayTao << "] [" << layLoaiGiaoDich() << "] Mã: " << maGD
             << " | Danh mục: " << danhMuc 
             << " | Số tiền: " << fixed << setprecision(0) << soTien << " VNĐ";
        if (!moTa.empty()) {
            cout << " | Ghi chú: " << moTa;
        }
        cout << endl;
    }
};

class IncomeTransaction : public BaseTransaction {
public:
    IncomeTransaction(string id, double tien, string dm, string desc = "", string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return abs(soTien);
    }
    string layLoaiGiaoDich() const override {
        return "Thu Nhập";
    }
};

class ExpenseTransaction : public BaseTransaction {
public:
    ExpenseTransaction(string id, double tien, string dm, string desc = "", string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return -abs(soTien);
    }
    string layLoaiGiaoDich() const override {
        return "Chi Phí";
    }
};

class RecurringExpenseTransaction : public ExpenseTransaction {
private:
    string chuKy;

public:
    RecurringExpenseTransaction(string id, double tien, string dm, string ck = "Hàng tháng", string desc = "", string dt = "2026-09-14")
        : ExpenseTransaction(id, tien, dm, desc, dt), chuKy(ck) {}

    string getChuKy() const { return chuKy; }

    string layLoaiGiaoDich() const override {
        return "Chi Phí Định Kỳ";
    }

    void hienThi() const override {
        cout << "[" << ngayTao << "] [" << layLoaiGiaoDich() << "] Mã: " << maGD
             << " | Danh mục: " << danhMuc 
             << " | Số tiền: " << fixed << setprecision(0) << soTien << " VNĐ"
             << " | Chu kỳ: " << chuKy;
        if (!moTa.empty()) {
            cout << " | Ghi chú: " << moTa;
        }
        cout << endl;
    }
};

// ==========================================
// HỢP NHẤT: CLASS TÀI KHOẢN
// ==========================================
class Account {
private:
    string maTK;
    string tenTK;
    string loaiTK;
    double soDu;
    vector<shared_ptr<BaseTransaction>> dsgd;

public:
    Account(string i, string n, string t, double b = 0.0)
        : maTK(i), tenTK(n), loaiTK(t), soDu(b) {}

    string getMaTK() const { return maTK; }
    string getTenTK() const { return tenTK; }
    string getLoaiTK() const { return loaiTK; }
    double getSoDu() const { return soDu; }
    const vector<shared_ptr<BaseTransaction>>& getDanhSachGiaoDich() const { return dsgd; }

    bool thucHienGiaoDich(shared_ptr<BaseTransaction> g) {
        double td = g->laySoTienThucTe();
        if (soDu + td < 0) {
            cout << " -> [Thất bại] Tài khoản '" << tenTK << "' không đủ tiền!\n";
            return false;
        }
        soDu += td;
        dsgd.push_back(g);
        cout << " -> [Thành công] TK '" << tenTK << "' Số dư mới: " 
             << fixed << setprecision(0) << soDu << " VNĐ\n";
        return true;
    }

    void inThongTin() const {
        cout << "Mã TK: " << maTK << " | Tên: " << setw(12) << left << tenTK 
             << " | Loại: " << setw(10) << left << loaiTK 
             << " | Số dư: " << fixed << setprecision(0) << soDu << " VNĐ\n";
    }

    void inLichSuGiaoDich() const {
        cout << "\n--- LỊCH SỬ GIAO DỊCH TÀI KHOẢN: " << tenTK << " ---\n";
        if(dsgd.empty()){
            cout << "(Chưa có giao dịch nào)\n";
        } else {
            for(const auto& g : dsgd) { g->hienThi(); }
        }
    }
};

// ==========================================
// CÁC CLASS QUẢN LÝ TỪ CÁC PHẦN (Giữ nguyên)
// ==========================================
class Budget {
private:
    string category;
    double limit;
    double spent;

public:
    Budget() : limit(0), spent(0) {}
    Budget(const string& categoryName, double budgetLimit) : category(categoryName), limit(budgetLimit), spent(0) {}

    bool addExpense(double amount) {
        if (amount <= 0) return false;
        spent += amount;
        return true;
    }

    bool isOverBudget() const { return spent > limit; }
    bool isAtLimit() const { return spent == limit; }
    double getRemaining() const { return limit - spent; }

    string getStatus() const {
        if (isOverBudget()) return "CANH BAO: da vuot ngan sach";
        if (isAtLimit()) return "CANH BAO: da cham nguong ngan sach";
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

struct ScheduledRule {
    shared_ptr<RecurringExpenseTransaction> templateTransaction; 
    Account* targetAccount;                                      
    string lastProcessedMonth;                                     
};

class TransactionScheduler {
private:
    vector<ScheduledRule> scheduledList;
    int autoIdCounter;

public:
    TransactionScheduler() : autoIdCounter(100) {}

    void registerRecurringTransaction(const shared_ptr<RecurringExpenseTransaction>& rExpense, Account* acc) {
        if (!rExpense || !acc) {
            cout << "[Loi Scheduler] Du lieu dang ky khong hop le (Null Pointer)!\n";
            return;
        }
        ScheduledRule rule;
        rule.templateTransaction = rExpense;
        rule.targetAccount = acc;
        rule.lastProcessedMonth = "";
        scheduledList.push_back(rule);
        cout << "[Scheduler] Da dang ky lich: '" << rExpense->getMoTa() 
             << "' cho tai khoan " << acc->getTenTK() << "\n";
    }

    void processMonthlyTransactions(const string& yearMonth, map<string, Budget>& budgets) {
        cout << "\n======================================================\n";
        cout << " MO PHONG KICH HOAT CHI PHI DINH KY THANG: " << yearMonth << "\n";
        cout << "======================================================\n";

        if (scheduledList.empty()) {
            cout << "(Chua co lich trinh chi phi dinh ky nao trong he thong)\n";
            return;
        }

        for (auto& rule : scheduledList) {
            if (rule.lastProcessedMonth == yearMonth) {
                cout << "- Khoan '" << rule.templateTransaction->getMoTa() 
                     << "' da duoc xu ly trong thang " << yearMonth << " roi.\n";
                continue;
            }

            string newId = "AUTO" + to_string(++autoIdCounter);
            string actualDate = yearMonth + "-01"; 

            auto actualTransaction = make_shared<ExpenseTransaction>(
                newId,
                rule.templateTransaction->getSoTien(),
                rule.templateTransaction->getDanhMuc(),
                "[Dinh ky] " + rule.templateTransaction->getMoTa(),
                actualDate
            );

            cout << "* Kich hoat khoan chi dinh ky: " << rule.templateTransaction->getMoTa() << "...\n";
            
            bool success = rule.targetAccount->thucHienGiaoDich(actualTransaction);

            if (success) {
                rule.lastProcessedMonth = yearMonth; 
                string category = rule.templateTransaction->getDanhMuc();
                if (budgets.find(category) != budgets.end()) {
                    budgets[category].addExpense(rule.templateTransaction->getSoTien());
                    cout << "  -> [Cap nhat ngan sach] Danh muc '" << category 
                         << "' | Trang thai: " << budgets[category].getStatus() << "\n";
                }
            }
        }
    }

    void processMonthlyTransactions(const string& yearMonth) {
        map<string, Budget> emptyBudgets;
        processMonthlyTransactions(yearMonth, emptyBudgets);
    }
};

class FinanceManager {
private:
    vector<Account> dsTaiKhoan;

public:
    void themTaiKhoan(Account tk) {
        dsTaiKhoan.push_back(tk);
    }

    Account* timTaiKhoan(string ma) {
        for (size_t i = 0; i < dsTaiKhoan.size(); i++) {
            if (dsTaiKhoan[i].getMaTK() == ma) {
                return &dsTaiKhoan[i];
            }
        }
        return nullptr;
    }

    void hienThiDanhSachTaiKhoan() const {
        cout << "\n--- DANH SACH TAI KHOAN ---\n";
        for (const auto& tk : dsTaiKhoan) {
            tk.inThongTin();
        }
    }

    bool chuyenTien(string maNguon, string maDich, double soTien) {
        Account* tkNguon = timTaiKhoan(maNguon);
        Account* tkDich = timTaiKhoan(maDich);

        if (tkNguon == nullptr || tkDich == nullptr) {
            cout << " -> Ma tai khoan khong ton tai!\n";
            return false;
        }

        auto gdRut = make_shared<ExpenseTransaction>("CT_OUT", soTien, "Chuyen tien", "Sang " + tkDich->getTenTK());
        auto gdNap = make_shared<IncomeTransaction>("CT_IN", soTien, "Nhan tien", "Từ " + tkNguon->getTenTK());

        if (tkNguon->thucHienGiaoDich(gdRut)) {
            tkDich->thucHienGiaoDich(gdNap);
            return true;
        }
        return false;
    }

    void locTheoDanhMuc(string danhMuc) const {
        cout << "\n--- KET QUA LOC DANH MUC: " << danhMuc << " ---\n";
        bool timThay = false;
        for (const auto& tk : dsTaiKhoan) {
            for (const auto& gd : tk.getDanhSachGiaoDich()) {
                if (gd->getDanhMuc() == danhMuc) {
                    gd->hienThi();
                    timThay = true;
                }
            }
        }
        if (!timThay) cout << "(Khong tim thay giao dich nao)\n";
    }
};

class ExpenseChart {
private:
    vector<shared_ptr<BaseTransaction>> danhSachGiaoDich;

public:
    void themGiaoDich(shared_ptr<BaseTransaction> giaoDich) {
        if (giaoDich != nullptr) {
            danhSachGiaoDich.push_back(giaoDich);
        }
    }

    map<string, double> tinhChiTieuTheoThang() const {
        map<string, double> chiTieuTheoThang;
        for (const auto& giaoDich : danhSachGiaoDich) {
            if (giaoDich->laySoTienThucTe() < 0) {
                string ngay = giaoDich->getNgayTao();
                if (ngay.length() >= 7) {
                    string thang = ngay.substr(0, 7);
                    double soTienChi = abs(giaoDich->laySoTienThucTe());
                    chiTieuTheoThang[thang] += soTienChi;
                }
            }
        }
        return chiTieuTheoThang;
    }

    void hienThiBieuDo() const {
        map<string, double> chiTieuTheoThang = tinhChiTieuTheoThang();
        cout << "\n==============================================\n";
        cout << "       BIEU DO CHI TIEU HANG THANG\n";
        cout << "==============================================\n";

        if (chiTieuTheoThang.empty()) {
            cout << "Chua co du lieu chi tieu.\n";
            return;
        }

        double chiTieuLonNhat = 0;
        for (const auto& item : chiTieuTheoThang) {
            if (item.second > chiTieuLonNhat) {
                chiTieuLonNhat = item.second;
            }
        }

        for (const auto& item : chiTieuTheoThang) {
            string thang = item.first;
            double soTien = item.second;
            int soCot = 0;
            if (chiTieuLonNhat > 0) {
                soCot = (int)(soTien / chiTieuLonNhat * 40);
            }
            if (soTien > 0 && soCot == 0) {
                soCot = 1;
            }
            cout << thang << " | ";
            for (int i = 0; i < soCot; i++) cout << "■";
            cout << " " << fixed << setprecision(0) << soTien << " VNĐ\n";
        }
        cout << "==============================================\n";
        cout << "Chu thich: Moi cot dai toi da 40 ky tu.\n";
    }
};

struct Transaction {
    int id;
    string type;
    double amount;
    string category;
    string account;
    string date;
    string description;
};

class FinanceStorage {
public:
    void saveToCSV(vector<Transaction> transactions, string filename) {
        ofstream file(filename.c_str());
        if (!file.is_open()) {
            cout << "Khong mo duoc file de ghi!" << endl;
            return;
        }
        file << "ID,Type,Amount,Category,Account,Date,Description\n";
        for (int i = 0; i < (int)transactions.size(); i++) {
            file << transactions[i].id << ","
                 << transactions[i].type << ","
                 << transactions[i].amount << ","
                 << transactions[i].category << ","
                 << transactions[i].account << ","
                 << transactions[i].date << ","
                 << transactions[i].description << "\n";
        }
        file.close();
        if (file.fail()) cout << "Co loi khi ghi file CSV!" << endl;
        else cout << "Da xuat du lieu thanh cong vao " << filename << "!" << endl;
    }

    vector<Transaction> loadFromCSV(string filename) {
        vector<Transaction> transactions;
        ifstream file(filename.c_str());
        if (!file.is_open()) {
            cout << "Khong mo duoc file " << filename << "!" << endl;
            return transactions;
        }
        string line;
        if (!getline(file, line)) {
            cout << "File CSV rong!" << endl;
            file.close();
            return transactions;
        }
        while (getline(file, line)) {
            stringstream ss(line);
            string id, type, amount, category, account, date, description;
            if (!getline(ss, id, ',') || !getline(ss, type, ',') || !getline(ss, amount, ',') ||
                !getline(ss, category, ',') || !getline(ss, account, ',') || !getline(ss, date, ',')) {
                continue;
            }
            getline(ss, description);
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
        cout << "Da tai " << transactions.size() << " giao dich tu file CSV!" << endl;
        return transactions;
    }
};

int getNextId(vector<Transaction> transactions) {
    int maxId = 0;
    for (int i = 0; i < (int)transactions.size(); i++) {
        if (transactions[i].id > maxId) maxId = transactions[i].id;
    }
    return maxId + 1;
}

// ==========================================
// CÁC HÀM CHẠY DEMO ĐƯỢC TÁCH RA TỪ 7 FILE CŨ
// ==========================================
void demoPhan1() {
    cout << "\n--- DEMO PHAN 1: GIAO DICH CO BAN ---\n";
    IncomeTransaction gd1("GD01", 5000000, "Luong", "Luong tháng 9");
    ExpenseTransaction gd2("GD02", 150000, "An uống", "An tối");
    RecurringExpenseTransaction gd3("GD03", 2500000, "Tiền nhà", "Hàng tháng", "Trả tháng 9");

    gd1.hienThi();
    gd2.hienThi();
    gd3.hienThi();
}

void demoPhan2() {
    cout << "\n--- DEMO PHAN 2: TAI KHOAN ---\n";
    Account vtm("TK01", "Ví Tiền Mặt", "Tiền mặt", 5e5);
    Account mb("TK02", "MB Bank", "Ngân hàng", 1e7);
    Account vmm("TK03", "Ví MoMo", "Ví điện tử", 3e5);
    
    // Gọi theo hàm trả về ban đầu trong code
    (void)(vtm.inThongTin(), mb.inThongTin(), vmm.inThongTin());
}

void demoPhan3() {
    map<string, Budget> budgets;
    int choice;
    cout << "\n=== QUAN LY HAN MUC NGAN SACH THEO DANH MUC ===\n";
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
            cout << "Nhap danh muc: "; cin >> category;
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
            cout << "Nhap danh muc: "; cin >> category;
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
            cout << budget->second.getStatus() << ". Con lai: " << fixed << setprecision(2) << budget->second.getRemaining() << '\n';
        } else if (choice == 3) {
            if (budgets.empty()) {
                cout << "Chua co danh muc nao.\n";
                continue;
            }
            cout << left << setw(18) << "Danh muc" << right << setw(14) << "Han muc"
                 << setw(14) << "Da chi" << setw(14) << "Con lai" << "  Trang thai\n";
            cout << string(80, '-') << '\n';
            map<string, Budget>::const_iterator item;
            for (item = budgets.begin(); item != budgets.end(); ++item) {
                item->second.printSummary();
            }
        }
    } while (choice != 0);
}

void demoPhan4() {
    cout << "\n--- DEMO PHAN 4: LAP LICH GIAO DICH ---\n";
    Account mb("TK02", "MB Bank", "Ngan hang", 5000000.0);
    Account vmm("TK03", "Vi MoMo", "Vi dien tu", 800000.0);

    cout << "--- TRANG THAI TAI KHOAN BAN DAU ---\n";
    mb.inThongTin();
    vmm.inThongTin();
    cout << "\n";

    auto tienTro = make_shared<RecurringExpenseTransaction>(
        "REC01", 2500000, "Tien nha", "Hang thang", "Tro co dinh hang thang"
    );
    auto spotify = make_shared<RecurringExpenseTransaction>(
        "REC02", 59000, "Giai tri", "Hang thang", "Goi Spotify Sinh Vien"
    );

    TransactionScheduler scheduler;
    scheduler.registerRecurringTransaction(tienTro, &mb);
    scheduler.registerRecurringTransaction(spotify, &vmm);

    scheduler.processMonthlyTransactions("2026-09");

    cout << "\n--- SO DU SAU KHI TU DONG TRU CHI PHI THANG 09/2026 ---\n";
    mb.inThongTin();
    vmm.inThongTin();

    scheduler.processMonthlyTransactions("2026-10");

    cout << "\n--- SO DU SAU KHI TU DONG TRU CHI PHI THANG 10/2026 ---\n";
    mb.inThongTin();
    vmm.inThongTin();
}

void demoPhan5() {
    cout << "\n--- DEMO PHAN 5: FINANCE MANAGER ---\n";
    FinanceManager ql;
    ql.themTaiKhoan(Account("TK01", "Vi Tien Mat", "Tien mat", 1000000));
    ql.themTaiKhoan(Account("TK02", "MB Bank", "Ngan hang", 5000000));
    ql.themTaiKhoan(Account("TK03", "Ví MoMo", "Ví điện tử", 500000));

    int chon = -1;
    do {
        cout << "\n1. Xem danh sach tai khoan & so du\n";
        cout << "2. Them giao dich (Thu/Chi)\n";
        cout << "3. Chuyen tien giua 2 tai khoan\n";
        cout << "4. Loc giao dich theo danh muc\n";
        cout << "0. Thoat\n";
        cout << "Chon chuc nang (0-4): ";
        cin >> chon;

        if (chon == 1) {
            ql.hienThiDanhSachTaiKhoan();
        } else if (chon == 2) {
            string maTK, maGD, danhMuc, ghiChu;
            double soTien; int loaiGD;
            ql.hienThiDanhSachTaiKhoan();
            cout << "Nhap ma tai khoan thuc hien: "; cin >> maTK;
            Account* tk = ql.timTaiKhoan(maTK);
            if (tk != nullptr) {
                cout << "Chon loai (1: Thu nhap, 2: Chi phi): "; cin >> loaiGD;
                cout << "Nhap ma GD: "; cin >> maGD;
                cout << "Nhap so tien: "; cin >> soTien;
                cout << "Nhap danh muc (VD: anuong, luong): "; cin >> danhMuc;
                cout << "Nhap ghi chu: "; 
                cin.ignore();
                getline(cin, ghiChu);
                if (loaiGD == 1) {
                    auto gd = make_shared<IncomeTransaction>(maGD, soTien, danhMuc, ghiChu);
                    tk->thucHienGiaoDich(gd);
                } else {
                    auto gd = make_shared<ExpenseTransaction>(maGD, soTien, danhMuc, ghiChu);
                    tk->thucHienGiaoDich(gd);
                }
            } else {
                cout << "Khong tim thay tai khoan!\n";
            }
        } else if (chon == 3) {
            string maNguon, maDich; double soTien;
            ql.hienThiDanhSachTaiKhoan();
            cout << "Nhap ma TK nguon (rut): "; cin >> maNguon;
            cout << "Nhap ma TK dich (nhan): "; cin >> maDich;
            cout << "Nhap so tien chuyen: "; cin >> soTien;
            ql.chuyenTien(maNguon, maDich, soTien);
        } else if (chon == 4) {
            string danhMuc;
            cout << "Nhap danh muc can loc (VD: anuong, luong, chuyentien): ";
            cin >> danhMuc;
            ql.locTheoDanhMuc(danhMuc);
        }
    } while (chon != 0);
}

void demoPhan6() {
    cout << "\n--- DEMO PHAN 6: BIEU DO CHI TIEU ---\n";
    ExpenseChart bieuDo;
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD01", 500000, "An uong", "An toi", "2026-07-05"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD02", 1000000, "Tien nha", "Tien tro", "2026-07-15"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD03", 800000, "An uong", "An trua", "2026-08-03"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD04", 1500000, "Tien nha", "Tien tro", "2026-08-15"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD05", 1200000, "An uong", "An toi", "2026-09-02"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD06", 1800000, "Tien nha", "Tien tro", "2026-09-10"));
    bieuDo.themGiaoDich(make_shared<RecurringExpenseTransaction>("GD07", 500000, "Giai tri", "Hang thang", "Goi dich vu", "2026-09-20"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD08", 700000, "An uong", "An uong", "2026-10-03"));
    bieuDo.themGiaoDich(make_shared<ExpenseTransaction>("GD09", 900000, "Di lai", "Xang xe", "2026-10-12"));
    
    bieuDo.hienThiBieuDo();
}

void demoPhan7() {
    cout << "\n--- DEMO PHAN 7: LUU TRU CSV ---\n";
    FinanceStorage storage;
    vector<Transaction> transactions;
    string filename = "finance.csv";
    int choice;

    do {
        cout << "\n===== QUAN LY DU LIEU TAI CHINH =====" << endl;
        cout << "1. Tai du lieu tu file CSV" << endl;
        cout << "2. Nhap giao dich moi" << endl;
        cout << "3. Xuat du lieu ra file CSV" << endl;
        cout << "4. Hien thi danh sach giao dich" << endl;
        cout << "0. Thoat ve menu chinh" << endl;
        cout << "Lua chon cua ban: ";
        cin >> choice;

        if (choice == 1) {
            transactions = storage.loadFromCSV(filename);
        } else if (choice == 2) {
            char again = 'y';
            while (again == 'y' || again == 'Y') {
                Transaction t;
                t.id = getNextId(transactions);
                cout << "\nNhap loai (Income/Expense/RecurringExpense): "; cin >> t.type;
                if (t.type != "Income" && t.type != "Expense" && t.type != "RecurringExpense") {
                    cout << "Loai giao dich khong hop le!" << endl;
                    continue;
                }
                cout << "Nhap so tien: "; cin >> t.amount;
                cin.ignore(10000, '\n');
                cout << "Nhap danh muc: "; getline(cin, t.category);
                cout << "Nhap tai khoan: "; getline(cin, t.account);
                cout << "Nhap ngay (YYYY-MM-DD): "; getline(cin, t.date);
                cout << "Nhap mo ta: "; getline(cin, t.description);
                transactions.push_back(t);
                cout << "Da them giao dich thanh cong!" << endl;
                cout << "Nhap tiep giao dich? (y/n): "; cin >> again;
            }
        } else if (choice == 3) {
            storage.saveToCSV(transactions, filename);
        } else if (choice == 4) {
            if (transactions.empty()) {
                cout << "Danh sach rong. Hay tai du lieu hoac nhap giao dich!" << endl;
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
        } else if (choice != 0) {
            cout << "Lua chon khong hop le!" << endl;
        }
    } while (choice != 0);
}

// ==========================================
// HÀM MAIN() MENU TỔNG HỢP CHUNG CHO TOÀN APP
// ==========================================
int main() {
    int choice = -1;
    do {
        cout << "\n=======================================================\n";
        cout << "       HE THONG QUAN LY TAI CHINH (BAN GOP APP)        \n";
        cout << "=======================================================\n";
        cout << "1. Demo Phan 1: Giao dich co ban (Thu/Chi)\n";
        cout << "2. Demo Phan 2: Tai khoan (Hien thi & Giao dich)\n";
        cout << "3. Demo Phan 3: Quan ly Ngan sach (Budget)\n";
        cout << "4. Demo Phan 4: Lap lich Giao dich (Scheduler)\n";
        cout << "5. Demo Phan 5: Quan ly Tai chinh (Them, Loc, Chuyen)\n";
        cout << "6. Demo Phan 6: Ve bieu do chi tieu (Chart)\n";
        cout << "7. Demo Phan 7: Nhap/Xuat du lieu CSV (Storage)\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "Moi ban chon (0-7): ";
        
        if (!(cin >> choice)) {
            ignoreInvalidInput();
            continue;
        }

        switch (choice) {
            case 1: demoPhan1(); break;
            case 2: demoPhan2(); break;
            case 3: demoPhan3(); break;
            case 4: demoPhan4(); break;
            case 5: demoPhan5(); break;
            case 6: demoPhan6(); break;
            case 7: demoPhan7(); break;
            case 0: cout << "Da thoat He Thong Quan Ly Tai Chinh!" << endl; break;
            default: cout << "Lua chon khong hop le, vui long thu lai!" << endl; break;
        }
    } while (choice != 0);

    return 0;
}
