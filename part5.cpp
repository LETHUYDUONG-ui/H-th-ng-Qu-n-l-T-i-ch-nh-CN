#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>

using namespace std;

class BaseTransaction {
protected:
    string maGD;
    double soTien;
    string danhMuc;
    string moTa;
    string ngayTao;

public:
    BaseTransaction(string id, double tien, string dm, string desc, string dt) {
        maGD = id;
        soTien = tien;
        danhMuc = dm;
        moTa = desc;
        ngayTao = dt;
    }

    virtual ~BaseTransaction() {}

    virtual double laySoTienThucTe() const = 0;
    virtual string layLoaiGiaoDich() const = 0;

    string getMaGD() const { return maGD; }
    double getSoTien() const { return soTien; }
    string getDanhMuc() const { return danhMuc; }
    string getMoTa() const { return moTa; }
    string getNgayTao() const { return ngayTao; }

    virtual void hienThi() const {
        cout << "[" << ngayTao << "] [" << layLoaiGiaoDich() << "] Ma: " << maGD
             << " | DM: " << danhMuc 
             << " | So tien: " << fixed << setprecision(0) << soTien << " VNÐ";
        if (!moTa.empty()) {
            cout << " | Ghi chu: " << moTa;
        }
        cout << endl;
    }
};

class IncomeTransaction : public BaseTransaction {
public:
    IncomeTransaction(string id, double tien, string dm, string desc = "", string dt = "2026-09-16")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return abs(soTien); // Thu nh?p là s? duong
    }

    string layLoaiGiaoDich() const override {
        return "Thu Nhap";
    }
};

class ExpenseTransaction : public BaseTransaction {
public:
    ExpenseTransaction(string id, double tien, string dm, string desc = "", string dt = "2026-09-16")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return -abs(soTien); // Chi phí là s? âm
    }

    string layLoaiGiaoDich() const override {
        return "Chi Phi";
    }
};

class Account {
private:
    string maTK;
    string tenTK;
    string loaiTK;
    double soDu;
    vector<shared_ptr<BaseTransaction>> dsgd;

public:
    Account(string id, string name, string type, double balance) {
        maTK = id;
        tenTK = name;
        loaiTK = type;
        soDu = balance;
    }

    string getMaTK() const { return maTK; }
    string getTenTK() const { return tenTK; }
    double getSoDu() const { return soDu; }
    const vector<shared_ptr<BaseTransaction>>& getDanhSachGiaoDich() const { return dsgd; }

    // Phuong th?c x? lý c?ng/tr? ti?n don gi?n
    bool thucHienGiaoDich(shared_ptr<BaseTransaction> g) {
        double thayDoi = g->laySoTienThucTe();

        // Ki?m tra n?u tài kho?n không d? ti?n chi tr?
        if (soDu + thayDoi < 0) {
            cout << " -> [That bai] Tai khoan '" << tenTK << "' không du so du!\n";
            return false;
        }

        soDu += thayDoi;
        dsgd.push_back(g);
        cout << " -> [Thanh cong] TK '" << tenTK << "' So du moi: " 
             << fixed << setprecision(0) << soDu << " VNÐ\n";
        return true;
    }

    void inThongTin() const {
        cout << "Ma TK: " << maTK << " | Ten: " << setw(12) << left << tenTK 
             << " | Loai: " << setw(10) << left << loaiTK 
             << " | So du: " << fixed << setprecision(0) << soDu << " VNÐ\n";
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
        auto gdNap = make_shared<IncomeTransaction>("CT_IN", soTien, "Nhan tien", "T? " + tkNguon->getTenTK());

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

int main() {
    FinanceManager ql;

    // Kh?i t?o s?n m?t s? tài kho?n m?u
    ql.themTaiKhoan(Account("TK01", "Vi Tien Mat", "Tien mat", 1000000));
    ql.themTaiKhoan(Account("TK02", "MB Bank", "Ngan hang", 5000000));
    ql.themTaiKhoan(Account("TK03", "Ví MoMo", "Ví di?n t?", 500000));

    int chon = -1;
    do {
        cout << "1. Xem danh sach tai khoan & so du\n";
        cout << "2. Them giao dich (Thu/Chi)\n";
        cout << "3. Chuyen tien giua 2 tai khoan\n";
        cout << "4. Loc giao dich theo danh muc\n";
        cout << "0. Thoat\n";
        cout << "Chon chuc nang (0-4): ";
        cin >> chon;

        if (chon == 1) {
            ql.hienThiDanhSachTaiKhoan();
        } 
        else if (chon == 2) {
            string maTK, maGD, danhMuc, ghiChu;
            double soTien;
            int loaiGD;

            ql.hienThiDanhSachTaiKhoan();
            cout << "Nhap ma tai khoan thuc hien: ";
            cin >> maTK;

            Account* tk = ql.timTaiKhoan(maTK);
            if (tk != nullptr) {
                cout << "Chon loai (1: Thu nhap, 2: Chi phi): ";
                cin >> loaiGD;
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
        } 
        else if (chon == 3) {
            string maNguon, maDich;
            double soTien;

            ql.hienThiDanhSachTaiKhoan();
            cout << "Nhap ma TK nguon (rut): "; cin >> maNguon;
            cout << "Nhap ma TK dich (nhan): "; cin >> maDich;
            cout << "Nhap so tien chuyen: "; cin >> soTien;

            ql.chuyenTien(maNguon, maDich, soTien);
        } 
        else if (chon == 4) {
            string danhMuc;
            cout << "Nhap danh muc can loc (VD: anuong, luong, chuyentien): ";
            cin >> danhMuc;
            ql.locTheoDanhMuc(danhMuc);
        }

    } while (chon != 0);

    cout << "\nDa thoat chuong trinh. Cam on!\n";
    return 0;
}
