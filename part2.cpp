#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>

struct BaseTransaction;

#define PRINT std::cout
#define FMT std::fixed << std::setprecision(0)

class Account {
private:
    std::string mtk;
    std::string ttk;
    std::string ltk;
    double sd;
    std::vector<std::shared_ptr<BaseTransaction>> dsgd;

public:
    Account(std::string i, std::string n, std::string t, double b = 0.0)
        : mtk(i), ttk(n), ltk(t), sd(b) {}

    auto getMaTK() const -> std::string { return mtk; }
    auto getTenTK() const -> std::string { return ttk; }
    auto getLoaiTK() const -> std::string { return ltk; }
    auto getSoDu() const -> double { return sd; }
    auto getDanhSachGiaoDich() const -> const std::vector<std::shared_ptr<BaseTransaction>>& { return dsgd; }

    bool thucHienGiaoDich(const std::shared_ptr<BaseTransaction>& g) {
        double td = g->laySoTienThucTe();
        return (sd + td < 0) 
            ? (PRINT << " [Thất bại] Tài khoản '" << ttk << "' không đủ tiền thực hiện giao dịch " << g->getMaGD() << "!\n", false)
            : (sd += td, dsgd.push_back(g), PRINT << " [Thành công] Tài khoản '" << ttk << "' " << (td >= 0 ? "+" : "") << FMT << td << " VNĐ | Số dư mới: " << sd << " VNĐ\n", true);
    }

    void inThongTin() const {
        PRINT << "Tài khoản: " << ttk << " (" << ltk << ") | Số dư: " << FMT << sd << " VNĐ\n";
    }

    void inLichSuGiaoDich() const {
        PRINT << "\n--- LỊCH SỬ GIAO DỊCH TÀI KHOẢN: " << ttk << " ---\n",
        dsgd.empty() 
            ? (void)(PRINT << "(Chưa có giao dịch nào)\n") 
            : std::for_each(dsgd.begin(), dsgd.end(), [](const auto& g) { g->hienThi(); });
    }
};

int main() {
    Account vtm("TK01", "Ví Tiền Mặt", "Tiền mặt", 5e5);
    Account mb("TK02", "MB Bank", "Ngân hàng", 1e7);
    Account vmm("TK03", "Ví MoMo", "Ví điện tử", 3e5);

    return (vtm.inThongTin(), mb.inThongTin(), vmm.inThongTin(), 0);
}
