#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <iomanip>

using namespace std;

// ==========================================
// 1. LOP GIAO DICH CO BAN
// ==========================================
class BaseTransaction {
protected:
    string maGD;
    double soTien;
    string danhMuc;
    string moTa;
    string ngayTao;

public:
    BaseTransaction(string id, double tien, string dm,
                    string desc = "", string dt = "2026-09-14")
        : maGD(id), soTien(tien), danhMuc(dm),
          moTa(desc), ngayTao(dt) {}

    virtual ~BaseTransaction() {}

    virtual double laySoTienThucTe() const = 0;
    virtual string layLoaiGiaoDich() const = 0;

    string getMaGD() const {
        return maGD;
    }

    double getSoTien() const {
        return soTien;
    }

    string getDanhMuc() const {
        return danhMuc;
    }

    string getMoTa() const {
        return moTa;
    }

    string getNgayTao() const {
        return ngayTao;
    }
};


// ==========================================
// 2. GIAO DICH THU NHAP
// ==========================================
class IncomeTransaction : public BaseTransaction {
public:
    IncomeTransaction(string id, double tien, string dm,
                      string desc = "", string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return abs(soTien);
    }

    string layLoaiGiaoDich() const override {
        return "Thu Nhap";
    }
};


// ==========================================
// 3. GIAO DICH CHI PHI
// ==========================================
class ExpenseTransaction : public BaseTransaction {
public:
    ExpenseTransaction(string id, double tien, string dm,
                       string desc = "", string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return -abs(soTien);
    }

    string layLoaiGiaoDich() const override {
        return "Chi Phi";
    }
};


// ==========================================
// 4. CHI PHI DINH KY
// ==========================================
class RecurringExpenseTransaction : public ExpenseTransaction {
private:
    string chuKy;

public:
    RecurringExpenseTransaction(string id, double tien, string dm,
                                string ck = "Hang thang",
                                string desc = "",
                                string dt = "2026-09-14")
        : ExpenseTransaction(id, tien, dm, desc, dt),
          chuKy(ck) {}

    string getChuKy() const {
        return chuKy;
    }

    string layLoaiGiaoDich() const override {
        return "Chi Phi Dinh Ky";
    }
};


// ==========================================
// 5. LOP VE BIEU DO CHI TIEU
// ==========================================
class ExpenseChart {
private:
    vector<shared_ptr<BaseTransaction>> danhSachGiaoDich;

public:

    // Them mot giao dich vao danh sach
    void themGiaoDich(shared_ptr<BaseTransaction> giaoDich) {
        if (giaoDich != nullptr) {
            danhSachGiaoDich.push_back(giaoDich);
        }
    }


    // Tinh tong chi tieu theo tung thang
    map<string, double> tinhChiTieuTheoThang() const {

        map<string, double> chiTieuTheoThang;

        for (const auto& giaoDich : danhSachGiaoDich) {

            // Chi lay cac giao dich chi phi
            if (giaoDich->laySoTienThucTe() < 0) {

                string ngay = giaoDich->getNgayTao();

                // Lay phan YYYY-MM tu ngay YYYY-MM-DD
                if (ngay.length() >= 7) {

                    string thang = ngay.substr(0, 7);

                    // Chuyen so tien chi thanh so duong
                    double soTienChi =
                        abs(giaoDich->laySoTienThucTe());

                    chiTieuTheoThang[thang] += soTienChi;
                }
            }
        }

        return chiTieuTheoThang;
    }


    // Hien thi bieu do cot
    void hienThiBieuDo() const {

        map<string, double> chiTieuTheoThang =
            tinhChiTieuTheoThang();

        cout << "\n==============================================\n";
        cout << "       BIEU DO CHI TIEU HANG THANG\n";
        cout << "==============================================\n";

        if (chiTieuTheoThang.empty()) {
            cout << "Chua co du lieu chi tieu.\n";
            return;
        }

        // Tim muc chi tieu lon nhat
        double chiTieuLonNhat = 0;

        for (const auto& item : chiTieuTheoThang) {
            if (item.second > chiTieuLonNhat) {
                chiTieuLonNhat = item.second;
            }
        }


        // Hien thi tung thang
        for (const auto& item : chiTieuTheoThang) {

            string thang = item.first;
            double soTien = item.second;

            // Moi bieu do co toi da 40 ky tu ¦
            int soCot = 0;

            if (chiTieuLonNhat > 0) {
                soCot = (int)(soTien / chiTieuLonNhat * 40);
            }

            // Neu co chi tieu thi it nhat hien thi 1 cot
            if (soTien > 0 && soCot == 0) {
                soCot = 1;
            }

            cout << thang << " | ";

            for (int i = 0; i < soCot; i++) {
                cout << "¦";
            }

            cout << " " << fixed << setprecision(0)
                 << soTien << " VNÐ\n";
        }

        cout << "==============================================\n";
        cout << "Chu thich: Moi cot dai toi da 40 ky tu.\n";
    }
};


// ==========================================
// 6. CHUONG TRINH CHINH
// ==========================================
int main() {

    ExpenseChart bieuDo;


    // ------------------------------------------
    // Tao cac giao dich mau
    // ------------------------------------------

    // Thang 07/2026
    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD01",
            500000,
            "An uong",
            "An toi",
            "2026-07-05"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD02",
            1000000,
            "Tien nha",
            "Tien tro",
            "2026-07-15"
        )
    );


    // Thang 08/2026
    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD03",
            800000,
            "An uong",
            "An trua",
            "2026-08-03"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD04",
            1500000,
            "Tien nha",
            "Tien tro",
            "2026-08-15"
        )
    );


    // Thang 09/2026
    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD05",
            1200000,
            "An uong",
            "An toi",
            "2026-09-02"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD06",
            1800000,
            "Tien nha",
            "Tien tro",
            "2026-09-10"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<RecurringExpenseTransaction>(
            "GD07",
            500000,
            "Giai tri",
            "Hang thang",
            "Goi dich vu",
            "2026-09-20"
        )
    );


    // Thang 10/2026
    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD08",
            700000,
            "An uong",
            "An uong",
            "2026-10-03"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD09",
            900000,
            "Di lai",
            "Xang xe",
            "2026-10-12"
        )
    );

    // ------------------------------------------
    // Hien thi bieu do
    // ------------------------------------------

    bieuDo.hienThiBieuDo();


    return 0;
}

