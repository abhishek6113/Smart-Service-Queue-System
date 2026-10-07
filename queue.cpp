#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;


// ==========================================
// CUSTOMER CLASS
// ==========================================

class Customer
{
public:

    int id;
    string name;
    string service;
    int priority;

    Customer(int i, string n, string s, int p)
    {
        id = i;
        name = n;
        service = s;
        priority = p;
    }
};


// ==========================================
// PRIORITY COMPARISON
// ==========================================

struct ComparePriority
{
    bool operator()(Customer a, Customer b)
    {
        return a.priority < b.priority;
    }
};


// ==========================================
// QUEUE SYSTEM CLASS
// ==========================================

class QueueSystem
{
private:

    priority_queue<
        Customer,
        vector<Customer>,
        ComparePriority
    > customers;

    vector<Customer> servedCustomers;

    int nextId;


public:

    QueueSystem()
    {
        nextId = 1;
    }


    // ======================================
    // ADD CUSTOMER
    // ======================================

    void addCustomer(
        string name,
        string service,
        int priority
    )
    {
        Customer c(
            nextId,
            name,
            service,
            priority
        );

        customers.push(c);

        cout << "\nCustomer added successfully!\n";

        cout << "Customer ID: "
             << nextId
             << endl;

        nextId++;
    }


    // ======================================
    // SERVE NEXT CUSTOMER
    // ======================================

    void serveCustomer()
    {
        if (customers.empty())
        {
            cout << "\nQueue is empty!\n";
            return;
        }


        Customer c = customers.top();

        cout << "\n====================================\n";
        cout << "          NOW SERVING\n";
        cout << "====================================\n";

        cout << "ID       : "
             << c.id
             << endl;

        cout << "Name     : "
             << c.name
             << endl;

        cout << "Service  : "
             << c.service
             << endl;

        cout << "Priority : "
             << getPriorityName(c.priority)
             << endl;


        customers.pop();

        servedCustomers.push_back(c);


        cout << "\nCustomer served successfully!\n";
    }


    // ======================================
    // DISPLAY CURRENT QUEUE
    // ======================================

    void displayQueue()
    {
        if (customers.empty())
        {
            cout << "\nQueue is empty!\n";
            return;
        }


        priority_queue<
            Customer,
            vector<Customer>,
            ComparePriority
        > temp = customers;


        cout << "\n====================================\n";
        cout << "          CURRENT QUEUE\n";
        cout << "====================================\n";


        while (!temp.empty())
        {
            Customer c = temp.top();


            cout << "\nID       : "
                 << c.id
                 << endl;

            cout << "Name     : "
                 << c.name
                 << endl;

            cout << "Service  : "
                 << c.service
                 << endl;

            cout << "Priority : "
                 << getPriorityName(c.priority)
                 << endl;


            temp.pop();
        }
    }


    // ======================================
    // SEARCH CUSTOMER
    // ======================================

    void searchCustomer()
    {
        if (customers.empty())
        {
            cout << "\nQueue is empty!\n";
            return;
        }


        string searchName;

        cin.ignore();

        cout << "\nEnter customer name to search: ";

        getline(cin, searchName);


        priority_queue<
            Customer,
            vector<Customer>,
            ComparePriority
        > temp = customers;


        bool found = false;


        while (!temp.empty())
        {
            Customer c = temp.top();


            if (c.name == searchName)
            {
                cout << "\nCustomer Found!\n";

                cout << "ID       : "
                     << c.id
                     << endl;

                cout << "Name     : "
                     << c.name
                     << endl;

                cout << "Service  : "
                     << c.service
                     << endl;

                cout << "Priority : "
                     << getPriorityName(c.priority)
                     << endl;

                found = true;

                break;
            }


            temp.pop();
        }


        if (!found)
        {
            cout << "\nCustomer not found!\n";
        }
    }


    // ======================================
    // SERVED CUSTOMER HISTORY
    // ======================================

    void displayServedCustomers()
    {
        if (servedCustomers.empty())
        {
            cout << "\nNo customer has been served yet.\n";
            return;
        }


        cout << "\n====================================\n";
        cout << "       SERVED CUSTOMER HISTORY\n";
        cout << "====================================\n";


        for (Customer c : servedCustomers)
        {
            cout << "\nID       : "
                 << c.id
                 << endl;

            cout << "Name     : "
                 << c.name
                 << endl;

            cout << "Service  : "
                 << c.service
                 << endl;

            cout << "Priority : "
                 << getPriorityName(c.priority)
                 << endl;

            cout << "Status   : Served\n";
        }
    }


    // ======================================
    // DISPLAY STATISTICS
    // ======================================

    void displayStatistics()
    {
        int waiting = customers.size();

        int served = servedCustomers.size();

        int total = waiting + served;


        cout << "\n====================================\n";
        cout << "         QUEUE STATISTICS\n";
        cout << "====================================\n";

        cout << "Total Customers   : "
             << total
             << endl;

        cout << "Waiting Customers : "
             << waiting
             << endl;

        cout << "Served Customers  : "
             << served
             << endl;
    }


    // ======================================
    // PRIORITY NAME
    // ======================================

    string getPriorityName(int priority)
    {
        if (priority == 3)
        {
            return "High";
        }

        else if (priority == 2)
        {
            return "Medium";
        }

        else
        {
            return "Low";
        }
    }
};


// ==========================================
// MAIN FUNCTION
// ==========================================

int main()
{
    QueueSystem system;

    int choice;


    while (true)
    {
        cout << "\n\n";
        cout << "====================================\n";
        cout << "    SMART SERVICE QUEUE SYSTEM\n";
        cout << "====================================\n";

        cout << "1. Add Customer\n";
        cout << "2. Serve Next Customer\n";
        cout << "3. Display Queue\n";
        cout << "4. Search Customer\n";
        cout << "5. Served Customer History\n";
        cout << "6. Queue Statistics\n";
        cout << "7. Exit\n";


        cout << "\nEnter your choice: ";

        cin >> choice;


        // ==================================
        // ADD CUSTOMER
        // ==================================

        if (choice == 1)
        {
            string name;
            string service;

            int priority;


            cin.ignore();


            cout << "Enter customer name: ";

            getline(cin, name);


            cout << "Enter service type: ";

            getline(cin, service);


            cout << "\nPriority:\n";

            cout << "3 = High\n";
            cout << "2 = Medium\n";
            cout << "1 = Low\n";


            cout << "Enter priority: ";

            cin >> priority;


            if (
                priority < 1 ||
                priority > 3
            )
            {
                cout << "\nInvalid priority!\n";

                continue;
            }


            system.addCustomer(
                name,
                service,
                priority
            );
        }


        // ==================================
        // SERVE CUSTOMER
        // ==================================

        else if (choice == 2)
        {
            system.serveCustomer();
        }


        // ==================================
        // DISPLAY QUEUE
        // ==================================

        else if (choice == 3)
        {
            system.displayQueue();
        }


        // ==================================
        // SEARCH CUSTOMER
        // ==================================

        else if (choice == 4)
        {
            system.searchCustomer();
        }


        // ==================================
        // SERVED HISTORY
        // ==================================

        else if (choice == 5)
        {
            system.displayServedCustomers();
        }


        // ==================================
        // STATISTICS
        // ==================================

        else if (choice == 6)
        {
            system.displayStatistics();
        }


        // ==================================
        // EXIT
        // ==================================

        else if (choice == 7)
        {
            cout << "\nProgram ended.\n";

            break;
        }


        else
        {
            cout << "\nInvalid choice!\n";
        }
    }


    return 0;
}