#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MaxProducts = 100;
const int MaxNameLength = 60;

class Product {
public:
    char pid[10];
    char name[MaxNameLength];
    double price;
    int quantity; 
};

void displayingMyProducts(const Product* products, int productCount) {
    cout << "PID\tProduct_name\tPrice" << endl;
    for (int i = 0; i < productCount; ++i) {
        cout << products[i].pid << "\t" << products[i].name << "\t" << products[i].price << endl;
    }
}

bool addNewProduct(Product* products, int& productCount, const char* pid, const char* name, double price) {
    if (productCount >= MaxProducts) {
        cout << "Product catalog is full" << endl;
        return false;
    }

    for (int i = 0; i < productCount; ++i) {
        if (strcmp(products[i].pid, pid) == 0) {
            cout << "Product with the same ID exists alrdy" << endl;
            return false;
        }
    }

    strcpy_s(products[productCount].pid, pid);
    strcpy_s(products[productCount].name, name);
    products[productCount].price = price;
    products[productCount].quantity = 0; 
    productCount++;
    return true;
}

bool updateMyProduct(Product* products, int productCount, const char* pid, const char* name, double price) {
    for (int i = 0; i < productCount; ++i) {
        if (strcmp(products[i].pid, pid) == 0) {
            strcpy_s(products[i].name, name);
            products[i].price = price;
            return true;
        }
    }
    cout << "Product not found" << endl;
    return false;
}

void saveAounsProducts(const Product* products, int productCount) {
    ofstream file("products.txt");
    if (!file.is_open()) {
        cerr << "Unable to save products" << endl;
        return;
    }

    for (int i = 0; i < productCount; ++i) {
        file << products[i].pid << " " << products[i].name << " " << products[i].price << endl;
    }
    file.close();
    cout << "Products updated successfully" << endl;
}

void displayMyShoppingList(const Product* cart, int itemCount) {
    double total = 0.0;
    cout << "PID\tQuantity\tPrice\tTotal" << endl;
    for (int i = 0; i < itemCount; ++i) {
        double itemTotal = cart[i].price * cart[i].quantity; 
        cout << cart[i].pid << "\t" << cart[i].quantity << "\t"<<"\t" << cart[i].price << "\t" << itemTotal << endl;
        total += itemTotal;
    }
    cout << "Total Bill is: " << total << endl;
}

void saveBill(const Product* cart, int itemCount) {
    ofstream file("bill.txt");
    if (!file.is_open()) {
        cerr << "Unable to save bill" << endl;
        return;
    }

    file << "PID\tQuantity\tPrice\tTotal" << endl;
    double total = 0.0;
    for (int i = 0; i < itemCount; ++i) {
        double itemTotal = cart[i].price * cart[i].quantity;
        file << cart[i].pid << "\t" << cart[i].quantity << "\t" << cart[i].price << "\t" << itemTotal << endl;
        total += itemTotal;
    }
    file << "Total Bill is: " << total << endl;
    file.close();
}

int main() {
    Product* products = new Product[MaxProducts];
    int productCount = 0;

    ifstream productFile("products.txt");
    if (!productFile.is_open()) {
        cerr << "Unable to open products file" << endl;
        delete[] products;
        return 1;
    }

    while (productCount < MaxProducts && productFile >> products[productCount].pid >> products[productCount].name >> products[productCount].price) {
        productCount++;
    }
    productFile.close();

    char choice;
    cout << "Are you an Admin(A) or a Customer(C)? ";
    cin >> choice;

    if (toupper(choice) == 'A') {
        cout << "Welcome Admin!" << endl;
        cout << "Choose an option:" << endl;
        cout << "1. Add a new product" << endl;
        cout << "2. Update product details" << endl;

        int option;
        cin >> option;

        if (option == 1) {
            char pid[10];
            char name[MaxNameLength];
            double price;

            cout << "Enter Product ID: ";
            cin >> pid;
            cout << "Enter Product Name: ";
            cin.ignore();
            cin.getline(name, MaxNameLength);
            cout << "Enter Price: ";
            cin >> price;

            if (addNewProduct(products, productCount, pid, name, price)) {
                saveAounsProducts(products, productCount);
            }
        }
        else if (option == 2) {
            char pid[10];
            char name[MaxNameLength];
            double price;

            cout << "Enter Product ID to update: ";
            cin >> pid;
            cout << "Enter new Product Name: ";
            cin.ignore();
            cin.getline(name, MaxNameLength);
            cout << "Enter new Price: ";
            cin >> price;

            if (updateMyProduct(products, productCount, pid, name, price)) {
                saveAounsProducts(products, productCount);
            }
        }
        else {
            cout << "Its an invalid option" << endl;
        }
    }
    else if (toupper(choice) == 'C') {
        cout << "Welcome Dear Customer!" << endl;
        displayingMyProducts(products, productCount);

        Product* shoppingList = new Product[MaxProducts];
        int itemCount = 0;

        char pid[10];
        int quantity;
        do {
            cout << "enter product ID: ";
            cin >> pid;
            if (strcmp(pid, "B") == 0) {
                break;
            }
            cout << "enter quantity: ";
            cin >> quantity;

            bool found = false;
            for (int i = 0; i < productCount; ++i) {
                if (strcmp(products[i].pid, pid) == 0) {
                    if (itemCount < MaxProducts) {
                        strcpy_s(shoppingList[itemCount].pid, products[i].pid);
                        strcpy_s(shoppingList[itemCount].name, products[i].name);
                        shoppingList[itemCount].price = products[i].price;
                        shoppingList[itemCount].quantity = quantity; 
                        itemCount++;
                        found = true;
                        break;
                    }
                    else {
                        cout << "cart is full" << endl;
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                cout << "Product not found, please enter a valid product ID" << endl;
            }
        } while (true);

        displayMyShoppingList(shoppingList, itemCount);
        saveBill(shoppingList, itemCount);

        delete[] shoppingList;
    }
    else {
        cout << "invalid choice" << endl;
    }

    delete[] products;

    return 0;
}
