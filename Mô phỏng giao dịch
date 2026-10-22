#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

// Struct quan ly quy tac lap lich giao dich
struct ScheduledRule {
    std::shared_ptr<RecurringExpenseTransaction> templateTransaction; 
    Account* targetAccount;                                             
    std::string lastProcessedMonth;                                     
};

// Lop dieu phoi va mo phong lap lich giao dich dinh ky
class TransactionScheduler {
private:
    std::vector<ScheduledRule> scheduledList;
    int autoIdCounter;

public:
    TransactionScheduler() : autoIdCounter(100) {}

    // Dang ky mot khoan chi phi dinh ky vao he thong
    void registerRecurringTransaction(const std::shared_ptr<RecurringExpenseTransaction>& rExpense, Account* acc) {
        if (!rExpense || !acc) {
            std::cout << "[Loi Scheduler] Du lieu dang ky khong hop le (Null Pointer)!\n";
            return;
        }

        ScheduledRule rule;
        rule.templateTransaction = rExpense;
        rule.targetAccount = acc;
        rule.lastProcessedMonth = "";

        scheduledList.push_back(rule);
        std::cout << "[Scheduler] Da dang ky lich: '" << rExpense->getMoTa() 
                  << "' cho tai khoan " << acc->getTenTK() << "\n";
    }

    // Process co truyen Ngan sach
    void processMonthlyTransactions(const std::string& yearMonth, std::map<std::string, Budget>& budgets) {
        std::cout << "\n======================================================\n";
        std::cout << " MO PHONG KICH HOAT CHI PHI DINH KY THANG: " << yearMonth << "\n";
        std::cout << "======================================================\n";

        if (scheduledList.empty()) {
            std::cout << "(Chua co lich trinh chi phi dinh ky nao trong he thong)\n";
            return;
        }

        for (auto& rule : scheduledList) {
            // 1. Kiem tra chong trung lap trong cung mot thang
            if (rule.lastProcessedMonth == yearMonth) {
                std::cout << "- Khoan '" << rule.templateTransaction->getMoTa() 
                          << "' da duoc xu ly trong thang " << yearMonth << " roi.\n";
                continue;
            }

            // 2. Sinh ma giao dich va ngay hach toan
            std::string newId = "AUTO" + std::to_string(++autoIdCounter);
            std::string actualDate = yearMonth + "-01"; 

            // 3. Khoi tao giao dich chi phi thuc te
            auto actualTransaction = std::make_shared<ExpenseTransaction>(
                newId,
                rule.templateTransaction->getSoTien(),
                rule.templateTransaction->getDanhMuc(),
                "[Dinh ky] " + rule.templateTransaction->getMoTa(),
                actualDate
            );

            std::cout << "* Kich hoat khoan chi dinh ky: " << rule.templateTransaction->getMoTa() << "...\n";
            
            // 4. Thuc hien tru tien trong Tai khoan (Phan 2)
            bool success = rule.targetAccount->thucHienGiaoDich(actualTransaction);

            if (success) {
                rule.lastProcessedMonth = yearMonth; 

                // 5. Tu dong cap nhat vao Ngan sach (Phan 3)
                std::string category = rule.templateTransaction->getDanhMuc();
                if (budgets.find(category) != budgets.end()) {
                    budgets[category].addExpense(rule.templateTransaction->getSoTien());
                    std::cout << "  -> [Cap nhat ngan sach] Danh muc '" << category 
                              << "' | Trang thai: " << budgets[category].getStatus() << "\n";
                }
            }
        }
    }

    // Overload giúp goi ham voi 1 tham so (khong truyen budgets)
    void processMonthlyTransactions(const std::string& yearMonth) {
        std::map<std::string, Budget> emptyBudgets;
        processMonthlyTransactions(yearMonth, emptyBudgets);
    }

    size_t getScheduledCount() const {
        return scheduledList.size();
    }
};

int main() {
    // 1. Khoi tao tai khoan
    Account mb("TK02", "MB Bank", "Ngan hang", 5000000.0);
    Account vmm("TK03", "Vi MoMo", "Vi dien tu", 800000.0);

    std::cout << "--- TRANG THAI TAI KHOAN BAN DAU ---\n";
    mb.inThongTin();
    vmm.inThongTin();
    std::cout << "\n";

    // 2. Khai bao cac mau chi phi dinh ky
    auto tienTro = std::make_shared<RecurringExpenseTransaction>(
        "REC01", 2500000, "Tien nha", "Hang thang", 5, "Tro co dinh hang thang"
    );
    
    auto spotify = std::make_shared<RecurringExpenseTransaction>(
        "REC02", 59000, "Giai tri", "Hang thang", 1, "Goi Spotify Sinh Vien"
    );

    // 3. Khoi tao bo lap lich va dang ky cac khoan chi
    TransactionScheduler scheduler;
    scheduler.registerRecurringTransaction(tienTro, &mb);
    scheduler.registerRecurringTransaction(spotify, &vmm);

    // 4. Mo phong kich hoat lap lich tu dong cho Thang 09/2026
    scheduler.processMonthlyTransactions("2026-09");

    // 5. Kiem tra ket qua so du
    std::cout << "\n--- SO DU SAU KHI TU DONG TRU CHI PHI THANG 09/2026 ---\n";
    mb.inThongTin();
    vmm.inThongTin();

    // 6. Mo phong chay tiep cho Thang 10/2026
    scheduler.processMonthlyTransactions("2026-10");

    std::cout << "\n--- SO DU SAU KHI TU DONG TRU CHI PHI THANG 10/2026 ---\n";
    mb.inThongTin();
    vmm.inThongTin();

    return 0;
}

