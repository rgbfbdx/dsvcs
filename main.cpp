#include <iostream>
#include <string>
#include <vector>

struct Violation {
    std::string description;
    std::string date;

    Violation(const std::string& desc, const std::string& d)
        : description(desc), date(d) {}
};

struct Node {
    std::string carNumber;
    std::vector<Violation> violations;
    Node* left;
    Node* right;

    Node(const std::string& num, const Violation& v)
        : carNumber(num), left(nullptr), right(nullptr) {
        violations.push_back(v);
    }
};

class TrafficFinesDB {
    Node* root;

    Node* insert(Node* node, const std::string& carNum, const Violation& violation) {
        if (!node)
            return new Node(carNum, violation);
        if (carNum < node->carNumber)
            node->left = insert(node->left, carNum, violation);
        else if (carNum > node->carNumber)
            node->right = insert(node->right, carNum, violation);
        else
            node->violations.push_back(violation);
        return node;
    }

    void printNode(const Node* node) const {
        if (!node) return;
        std::cout << "Номер машини: " << node->carNumber << "\n";
        for (const auto& v : node->violations) {
            std::cout << "  Порушення: " << v.description << ", дата: " << v.date << "\n";
        }
    }

    void printInOrder(const Node* node) const {
        if (!node) return;
        printInOrder(node->left);
        printNode(node);
        printInOrder(node->right);
    }

    const Node* search(const Node* node, const std::string& carNum) const {
        if (!node) return nullptr;
        if (carNum == node->carNumber) return node;
        if (carNum < node->carNumber)
            return search(node->left, carNum);
        else
            return search(node->right, carNum);
    }

    void printRange(const Node* node, const std::string& start, const std::string& end) const {
        if (!node) return;
        if (start < node->carNumber)
            printRange(node->left, start, end);
        if (start <= node->carNumber && node->carNumber <= end)
            printNode(node);
        if (node->carNumber < end)
            printRange(node->right, start, end);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    TrafficFinesDB() : root(nullptr) {}
    ~TrafficFinesDB() { destroy(root); }

    void addViolation(const std::string& carNum, const Violation& violation) {
        root = insert(root, carNum, violation);
    }

    void printAll() const {
        printInOrder(root);
    }

    void printByNumber(const std::string& carNum) const {
        const Node* node = search(root, carNum);
        if (node) printNode(node);
        else std::cout << "Дані по номеру " << carNum << " не знайдено.\n";
    }

    void printByRange(const std::string& start, const std::string& end) const {
        printRange(root, start, end);
    }
};


// Приклад
int main() {
    TrafficFinesDB db;
    db.addViolation("AA1234BB", {"Перевищення швидкості", "2025-07-01"});
    db.addViolation("AA1234BB", {"Паркування в забороненому місці", "2025-07-03"});
    db.addViolation("BB5678CC", {"Проїзд на червоне світло", "2025-06-30"});
    db.addViolation("CC9999DD", {"Відсутність страховки", "2025-07-05"});

    std::cout << "--- Всі дані ---\n";
    db.printAll();

    std::cout << "\n--- Дані за номером AA1234BB ---\n";
    db.printByNumber("AA1234BB");

    std::cout << "\n--- Дані за діапазоном AA0000AA - BB9999ZZ ---\n";
    db.printByRange("AA0000AA", "BB9999ZZ");

    return 0;
}
