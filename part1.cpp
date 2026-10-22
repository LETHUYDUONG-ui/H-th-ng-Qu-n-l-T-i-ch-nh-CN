#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

class BaseTransaction {
protected:
    std::string maGD;
    double soTien;
    std::string danhMuc;
    std::string moTa;
    std::string ngayTao;

public:
    BaseTransaction(std::string id, double tien, std::string dm, std::string desc = "", std::string dt = "2026-09-14")
        : maGD(id), soTien(tien), danhMuc(dm), moTa(desc), ngayTao(dt) {}

    virtual ~BaseTransaction() = default;

    virtual double laySoTienThucTe() const = 0;
    virtual std::string layLoaiGiaoDich() const = 0;

    std::string getMaGD() const { return maGD; }
    double getSoTien() const { return soTien; }
    std::string getDanhMuc() const { return danhMuc; }
    std::string getMoTa() const { return moTa; }
    std::string getNgayTao() const { return ngayTao; }

    virtual void hienThi() const {
        std::cout << "[" << ngayTao << "] [" << layLoaiGiaoDich() << "] Mã: " << maGD
                  << " | Danh mục: " << danhMuc 
                  << " | Số tiền: " << std::fixed << std::setprecision(0) << soTien << " VNĐ";
        if (!moTa.empty()) {
            std::cout << " | Ghi chú: " << moTa;
        }
        std::cout << std::endl;
    }
};

class IncomeTransaction : public BaseTransaction {
public:
    IncomeTransaction(std::string id, double tien, std::string dm, std::string desc = "", std::string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return std::abs(soTien);
    }

    std::string layLoaiGiaoDich() const override {
        return "Thu Nhập";
    }
};

class ExpenseTransaction : public BaseTransaction {
public:
    ExpenseTransaction(std::string id, double tien, std::string dm, std::string desc = "", std::string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return -std::abs(soTien);
    }

    std::string layLoaiGiaoDich() const override {
        return "Chi Phí";
    }
};

class RecurringExpenseTransaction : public ExpenseTransaction {
private:
    std::string chuKy;

public:
    RecurringExpenseTransaction(std::string id, double tien, std::string dm, std::string ck = "Hàng tháng", std::string desc = "", std::string dt = "2026-09-14")
        : ExpenseTransaction(id, tien, dm, desc, dt), chuKy(ck) {}

    std::string getChuKy() const { return chuKy; }

    std::string layLoaiGiaoDich() const override {
        return "Chi Phí Định Kỳ";
    }

    void hienThi() const override {
        std::cout << "[" << ngayTao << "] [" << layLoaiGiaoDich() << "] Mã: " << maGD
                  << " | Danh mục: " << danhMuc 
                  << " | Số tiền: " << std::fixed << std::setprecision(0) << soTien << " VNĐ"
                  << " | Chu kỳ: " << chuKy;
        if (!moTa.empty()) {
            std::cout << " | Ghi chú: " << moTa;
        }
        std::cout << std::endl;
    }
};

int main() {
    IncomeTransaction gd1("GD01", 5000000, "Lương", "Lương tháng 9");
    ExpenseTransaction gd2("GD02", 150000, "Ăn uống", "Ăn tối");
    RecurringExpenseTransaction gd3("GD03", 2500000, "Tiền nhà", "Hàng tháng", "Trọ tháng 9");

    gd1.hienThi();
    gd2.hienThi();
    gd3.hienThi();

    return 0;
} 
