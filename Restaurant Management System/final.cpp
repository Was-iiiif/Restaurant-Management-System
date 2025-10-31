#include <iostream>
#include <string>
using namespace std;

// Global variables for accounts
float totalRevenue = 0.0;
float totalExpenses = 1500.0; // Fixed daily expenses
int totalOrders = 0;

// Item sales tracking
string itemNames[16] = {
    "French Fries", "Chicken Wings", "Garlic Bread", "Spring Rolls",
    "Burger", "Pizza", "Pasta", "Grilled Chicken",
    "Coca Cola", "Fresh Juice", "Coffee", "Milkshake",
    "Ice Cream", "Chocolate Cake", "Brownie", "Pudding"};

float itemPrices[16] = {
    80.0, 120.0, 90.0, 100.0,
    150.0, 200.0, 180.0, 220.0,
    50.0, 70.0, 60.0, 90.0,
    80.0, 120.0, 100.0, 90.0};

int itemsSold[16] = {0};
float itemRevenue[16] = {0.0};

// Rating tracking
float itemRatings[16] = {0.0};
int itemRatingCount[16] = {0};

// Forward declarations for friend operators
class MenuItem;
class Customer;

ostream &operator<<(ostream &os, const MenuItem &m);
ostream &operator<<(ostream &os, const Customer &c);
istream &operator>>(istream &is, Customer &c);

// Class 1: MenuItem
class MenuItem
{
private:
    string name;
    float price;
    int itemID;

public:
    MenuItem(string n = "", float p = 0.0, int id = 0) : name(n), price(p), itemID(id) {}

    // * operator multiplies price by quantity
    MenuItem operator*(int qty)
    {
        MenuItem temp = *this;
        temp.price = this->price * qty;
        return temp;
    }

    float getPrice() const { return price; }
    string getName() const { return name; }
    int getID() const { return itemID; }

    friend ostream &operator<<(ostream &os, const MenuItem &m);
};

// << operator for MenuItem
ostream &operator<<(ostream &os, const MenuItem &m)
{
    os << m.itemID << ". " << m.name << " - BDT " << m.price;
    return os;
}

// Class 2: Customer
class Customer
{
private:
    string name;
    string phone;
    int customerID;

public:
    Customer(string n = "", string ph = "", int id = 0) : name(n), phone(ph), customerID(id) {}

    friend ostream &operator<<(ostream &os, const Customer &c);
    friend istream &operator>>(istream &is, Customer &c);
};

// >> operator for Customer
istream &operator>>(istream &is, Customer &c)
{
    string custName;
    string custPhone;

    getline(is, custName);

    bool validPhone = false;
    while (!validPhone)
    {
        cout << "Please enter your phone number (11 digits): ";
        getline(is, custPhone);

        bool allDigits = true;
        for (size_t i = 0; i < custPhone.length(); i++)
        {
            if (custPhone[i] < '0' || custPhone[i] > '9')
            {
                allDigits = false;
                break;
            }
        }

        if (!allDigits)
        {
            cout << "Error: Phone number must contain only digits (0-9)!\n";
            continue;
        }

        if (custPhone.length() != 11)
        {
            cout << "Error: Phone number must be exactly 11 digits!\n";
            continue;
        }

        validPhone = true;
    }

    c.name = custName;
    c.phone = custPhone;
    c.customerID = 1001;
    return is;
}

// << operator for Customer
ostream &operator<<(ostream &os, const Customer &c)
{
    os << "Customer: " << c.name << " | Phone: " << c.phone;
    return os;
}

// Class 3: Order
class Order
{
private:
    string items[20];
    float prices[20];
    int quantities[20];
    int quantity;
    float totalAmount;

public:
    Order() : quantity(0), totalAmount(0.0)
    {
        for (int i = 0; i < 20; i++)
        {
            quantities[i] = 0;
            prices[i] = 0.0f;
            items[i] = "";
        }
    }

    bool operator>=(float threshold) const
    {
        return totalAmount >= threshold;
    }

    void addItem(string itemName, float itemPrice, int qty)
    {
        MenuItem tempItem(itemName, itemPrice, 0);
        MenuItem lineItem = tempItem * qty;

        items[quantity] = itemName;
        prices[quantity] = lineItem.getPrice();
        quantities[quantity] = qty;
        quantity++;
        totalAmount += lineItem.getPrice();

        for (int i = 0; i < 16; i++)
        {
            if (itemNames[i] == itemName)
            {
                itemsSold[i] += qty;
                itemRevenue[i] += lineItem.getPrice();
                break;
            }
        }
    }

    void display()
    {
        cout << "\n========== YOUR ORDER ==========\n";
        for (int i = 0; i < quantity; i++)
        {
            cout << (i + 1) << ". " << items[i] << " x " << quantities[i] << " - BDT " << prices[i] << endl;
        }
        cout << "================================\n";
        cout << "Total Items in Order: " << quantity << endl;
        cout << "Total Amount: BDT " << totalAmount << endl;
        cout << "================================\n";
    }

    float getTotal() { return totalAmount; }
    int getQuantity() { return quantity; }

    string getItemAt(int index)
    {
        if (index >= 0 && index < quantity)
            return items[index];
        return "";
    }

    friend class Bill;
};

// Class 4: Discount
class Discount
{
private:
    float discountPercent;
    float discountAmount;
    string discountReason;

public:
    Discount(float percent = 0.0, string reason = "") : discountPercent(percent), discountReason(reason), discountAmount(0.0) {}

    float calculateDiscount(float totalAmount)
    {
        discountAmount = totalAmount * (discountPercent / 100.0);
        return discountAmount;
    }

    void display()
    {
        cout << "Discount: " << discountPercent << "% (" << discountReason << ") - BDT " << discountAmount << " off" << endl;
    }

    float getDiscountAmount() { return discountAmount; }
};

// Class 5: Bill
class Bill
{
private:
    float subtotal;
    float tax;
    float grandTotal;

public:
    Bill(float sub = 0.0) : subtotal(sub)
    {
        tax = subtotal * 0.10;
        grandTotal = subtotal + tax;
    }

    Bill operator-(float discount)
    {
        Bill temp = *this;
        temp.grandTotal -= discount;
        return temp;
    }

    void display()
    {
        cout << "\n========== BILL ==========\n";
        cout << "Subtotal:    BDT " << subtotal << endl;
        cout << "VAT (10%):   BDT " << tax << endl;
        cout << "-------------------------\n";
        cout << "Grand Total: BDT " << grandTotal << endl;
        cout << "==========================\n";
    }

    float getGrandTotal() { return grandTotal; }
    friend void takeOrder();
};

// Class 6: Rating
class Rating
{
private:
    string itemName;
    float averageRating;
    int totalRatings;

public:
    Rating(string name = "", float avg = 0.0, int count = 0)
        : itemName(name), averageRating(avg), totalRatings(count) {}

    Rating operator+(float newRating)
    {
        Rating temp = *this;
        temp.totalRatings++;
        temp.averageRating = ((averageRating * totalRatings) + newRating) / temp.totalRatings;
        return temp;
    }

    float getAverage() const { return averageRating; }
    int getCount() const { return totalRatings; }
};

// =================== showAccounts() ===================
void showAccounts()
{
    cout << "\n********************************************\n";
    cout << "       DAILY SALES REPORT\n";
    cout << "********************************************\n\n";

    cout << "========== ITEMS SOLD TODAY ==========\n\n";

    auto printSection = [&](string title, int start, int end)
    {
        cout << "--- " << title << " ---\n";
        float sectionRevenue = 0.0;
        for (int i = start; i < end; i++)
        {
            if (itemsSold[i] > 0)
            {
                cout << itemNames[i] << ": " << itemsSold[i]
                     << " sold | Revenue: BDT " << itemRevenue[i] << endl;
                sectionRevenue += itemRevenue[i];
            }
        }
        if (sectionRevenue == 0)
            cout << "No items sold\n";
        cout << "Total " << title << " Revenue: BDT " << sectionRevenue << "\n\n";
        return sectionRevenue;
    };

    float startersProfit = printSection("STARTERS", 0, 4);
    float mainCourseProfit = printSection("MAIN COURSE", 4, 8);
    float drinksProfit = printSection("DRINKS", 8, 12);
    float dessertsProfit = printSection("DESSERTS", 12, 16);

    cout << "========== FINANCIAL SUMMARY ==========\n";
    cout << "Total Orders Completed: " << totalOrders << endl;
    cout << "Total Revenue: BDT " << totalRevenue << endl;
    cout << "Target Revenue: BDT " << totalExpenses << endl;
    cout << "---------------------------------------\n";

    float netProfit = totalRevenue - totalExpenses;
    cout << "Net Profit: BDT " << netProfit << endl;

    if (netProfit > 0)
        cout << "\nStatus: PROFIT \n";
    else if (netProfit < 0)
        cout << "\nStatus: LOSS \n";
    else
        cout << "\nStatus: BREAK-EVEN\n";

    cout << "\n========== CUSTOMER RATINGS ==========\n\n";
    bool hasRatings = false;
    for (int i = 0; i < 16; i++)
        if (itemRatingCount[i] > 0)
            hasRatings = true;

    if (!hasRatings)
    {
        cout << "No ratings received yet.\n";
    }
    else
    {
        for (int i = 0; i < 16; i++)
        {
            if (itemRatingCount[i] > 0)
            {
                float avg = itemRatings[i] / itemRatingCount[i];
                cout << itemNames[i] << " - Avg: " << avg << "/5.0 (" << itemRatingCount[i] << " reviews ) \n";
            }
        }
    }

    cout << "\n********************************************\n";
}

// =================== takeOrder() ===================
void takeOrder()
{
    cout << "\n********************************************\n";
    cout << "         ORDER SECTION\n";
    cout << "********************************************\n\n";

    cout << "Welcome to KRUSTY KRAB!\n\n";

    cout << "Please enter your name: ";
    cin.ignore();
    Customer customer;
    cin >> customer;

    cout << "\nThank you, " << customer << endl;

    MenuItem starters[4] = {
        MenuItem("French Fries", 80.0, 1),
        MenuItem("Chicken Wings", 120.0, 2),
        MenuItem("Garlic Bread", 90.0, 3),
        MenuItem("Spring Rolls", 100.0, 4)};

    MenuItem mainCourse[4] = {
        MenuItem("Burger", 150.0, 5),
        MenuItem("Pizza", 200.0, 6),
        MenuItem("Pasta", 180.0, 7),
        MenuItem("Grilled Chicken", 220.0, 8)};

    MenuItem drinks[4] = {
        MenuItem("Coca Cola", 50.0, 9),
        MenuItem("Fresh Juice", 70.0, 10),
        MenuItem("Coffee", 60.0, 11),
        MenuItem("Milkshake", 90.0, 12)};

    MenuItem desserts[4] = {
        MenuItem("Ice Cream", 80.0, 13),
        MenuItem("Chocolate Cake", 120.0, 14),
        MenuItem("Brownie", 100.0, 15),
        MenuItem("Pudding", 90.0, 16)};

    Order myOrder;
    bool ordering = true;

    while (ordering)
    {
        cout << "\n========== MENU ==========\n";
        cout << "1. Starters\n2. Main Course\n3. Drinks\n4. Desserts\n5. View Order\n0. Checkout\n";
        cout << "==========================\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        auto handleCategory = [&](MenuItem menu[], int startID)
        {
            bool inCategory = true;
            while (inCategory)
            {
                cout << "\n--- SELECT ITEM ---\n";
                for (int i = 0; i < 4; i++)
                    cout << menu[i] << endl;
                cout << "0. Go Back\nEnter item number: ";
                int itemChoice;
                cin >> itemChoice;
                if (itemChoice == 0)
                    inCategory = false;
                else if (itemChoice >= startID && itemChoice < startID + 4)
                {
                    cout << "Enter quantity: ";
                    int qty;
                    cin >> qty;
                    if (qty > 0)
                    {
                        myOrder.addItem(menu[itemChoice - startID].getName(),
                                        menu[itemChoice - startID].getPrice(), qty);
                        cout  << qty << "x "
                             << menu[itemChoice - startID].getName()
                             << " added!\n";
                    }
                }
                else
                    cout << "Invalid item number!\n";
            }
        };

        switch (choice)
        {
        case 0:
            if (myOrder.getQuantity() > 0)
                ordering = false;
            else
                cout << "Add something first!\n";
            break;
        case 1:
            handleCategory(starters, 1);
            break;
        case 2:
            handleCategory(mainCourse, 5);
            break;
        case 3:
            handleCategory(drinks, 9);
            break;
        case 4:
            handleCategory(desserts, 13);
            break;
        case 5:
            myOrder.display();
            break;
        default:
            cout << "Invalid choice!\n";
        }
    }

    cout << "\n\nFinalizing your order...\n";
    cout << customer << endl;
    myOrder.display();

    Bill finalBill(myOrder.getTotal());
    finalBill.display();

    float finalAmount = finalBill.getGrandTotal();
    float totalDiscountAmount = 0.0;

    cout << "\n--- Discount Section ---\n";

    if (myOrder >= 1000.0)
    {
        Discount highValueDiscount(5.0, "High Value Order Discount");
        float discountAmt = highValueDiscount.calculateDiscount(finalBill.getGrandTotal());
        highValueDiscount.display();
        finalBill = finalBill - discountAmt;
        totalDiscountAmount += discountAmt;
        finalAmount = finalBill.getGrandTotal();
    }

    cout << "Are you a KUETian? (1 = Yes, 0 = No): ";
    int isKUETian;
    cin >> isKUETian;

    if (isKUETian == 1)
    {
        Discount kuetDiscount(10.0, "KUET Student Discount");
        float discountAmt = kuetDiscount.calculateDiscount(finalBill.getGrandTotal());
        kuetDiscount.display();
        finalBill = finalBill - discountAmt;
        totalDiscountAmount += discountAmt;
        finalAmount = finalBill.getGrandTotal();
    }

    if (totalDiscountAmount > 0)
        cout << "\nTotal Discount: BDT " << totalDiscountAmount
             << "\nFinal Amount: BDT " << finalAmount << endl;
    else
        cout << "\nNo discount applied.\n";

    totalRevenue += finalAmount;
    totalOrders++;

    // ✅ Rating Section (fixed)
    cout << "\n\n========== RATE YOUR EXPERIENCE ==========\n";
    cout << "Please rate the items you ordered (0-5)\n\n";

    for (int i = 0; i < myOrder.getQuantity(); i++)
    {
        string item = myOrder.getItemAt(i);
        cout << "Rate " << item << " (0-5): ";
        float rating;
        cin >> rating;
        while (rating < 0 || rating > 5)
        {
            cout << "Invalid rating! Enter 0-5: ";
            cin >> rating;
        }

        for (int j = 0; j < 16; j++)
        {
            if (itemNames[j] == item)
            {
                float currentAvg = (itemRatingCount[j] > 0)
                                       ? (itemRatings[j] / itemRatingCount[j])
                                       : 0.0;
                Rating r(item, currentAvg, itemRatingCount[j]);
                r = r + rating;

                itemRatingCount[j] = r.getCount();
                itemRatings[j] = r.getAverage() * itemRatingCount[j];
                break;
            }
        }
        cout << "Thank you for rating " << item << "!\n";
    }

    cout << "\nOrder #" << totalOrders << " completed successfully!\n";
}

// =================== main() ===================
int main()
{
    cout << "\n**********************************************\n";
    cout << "          WELCOME TO KRUSTY KRAB!\n";
    cout << "**********************************************\n\n";

    bool running = true;
    while (running)
    {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Order Now!\n2. Admin Section\n3. Exit\n";
        cout << "===============================\n";
        cout << "Enter your choice: ";
        int mainChoice;
        cin >> mainChoice;

        switch (mainChoice)
        {
        case 1:
            takeOrder();
            break;

        case 2:
            showAccounts();
            break;

        case 3:
            cout << "\nThank you for visiting KRUSTY KRAB!\n";
            cout << "Have a great day!\n";
            running = false;
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
