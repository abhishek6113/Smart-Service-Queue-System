// ==========================================
// SMART SERVICE QUEUE OPTIMIZATION SYSTEM
// ==========================================


// Customer data
let customers = [];

let servedCustomers = [];

let customerId = 1;


// ==========================================
// GET HTML ELEMENTS
// ==========================================

const form = document.querySelector("form");

const nameInput = document.querySelector(
    'input[placeholder="Enter customer name"]'
);

const serviceSelect = document.querySelectorAll("select")[0];

const prioritySelect = document.querySelectorAll("select")[1];

const tableBody = document.querySelector("tbody");

const servedTableBody = document.getElementById(
    "servedTableBody"
);

const totalCustomers = document.getElementById(
    "totalCustomers"
);

const waitingCustomers = document.getElementById(
    "waitingCustomers"
);

const servedCustomersCount = document.getElementById(
    "servedCustomers"
);

const serveButton = document.querySelector(
    ".serve-button"
);

const servingBox = document.querySelector(
    ".customer"
);

const searchInput = document.getElementById(
    "searchInput"
);

const searchButton = document.getElementById(
    "searchButton"
);

const searchResult = document.getElementById(
    "searchResult"
);


// ==========================================
// ADD CUSTOMER
// ==========================================

form.addEventListener("submit", function(event)
{
    event.preventDefault();


    const name = nameInput.value.trim();

    const service = serviceSelect.value;

    const priority = parseInt(
        prioritySelect.value
    );


    // Validation

    if (
        name === "" ||
        service === "" ||
        isNaN(priority)
    )
    {
        alert("Please enter all details.");

        return;
    }


    // Create customer

    const customer =
    {
        id: customerId,

        name: name,

        service: service,

        priority: priority
    };


    // Add customer

    customers.push(customer);

    customerId++;


    // Sort by priority

    customers.sort(function(a, b)
    {
        return b.priority - a.priority;
    });


    // Update screen

    updateQueue();

    updateDashboard();


    // Clear form

    nameInput.value = "";

    serviceSelect.value = "";

    prioritySelect.value = "";


    alert(
        "Customer added successfully!"
    );
});


// ==========================================
// GET PRIORITY NAME
// ==========================================

function getPriorityName(priority)
{
    if (priority === 3)
    {
        return "High";
    }

    else if (priority === 2)
    {
        return "Medium";
    }

    else
    {
        return "Low";
    }
}


// ==========================================
// GET PRIORITY CLASS
// ==========================================

function getPriorityClass(priority)
{
    if (priority === 3)
    {
        return "high";
    }

    else if (priority === 2)
    {
        return "medium";
    }

    else
    {
        return "low";
    }
}


// ==========================================
// UPDATE QUEUE TABLE
// ==========================================

function updateQueue()
{
    tableBody.innerHTML = "";


    if (customers.length === 0)
    {
        tableBody.innerHTML = `
            <tr>
                <td colspan="6">
                    No waiting customers
                </td>
            </tr>
        `;

        return;
    }


    customers.forEach(function(customer, index)
    {
        const priorityText =
            getPriorityName(customer.priority);


        const priorityClass =
            getPriorityClass(customer.priority);


        const row =
            document.createElement("tr");


        row.innerHTML = `
            <td>${index + 1}</td>

            <td>${customer.id}</td>

            <td>${customer.name}</td>

            <td>${customer.service}</td>

            <td>
                <span class="priority ${priorityClass}">
                    ${priorityText}
                </span>
            </td>

            <td>
                <span class="waiting">
                    Waiting
                </span>
            </td>
        `;


        tableBody.appendChild(row);
    });
}


// ==========================================
// UPDATE DASHBOARD
// ==========================================

function updateDashboard()
{
    const total =
        customers.length +
        servedCustomers.length;


    totalCustomers.textContent =
        total;


    waitingCustomers.textContent =
        customers.length;


    servedCustomersCount.textContent =
        servedCustomers.length;
}


// ==========================================
// SERVE NEXT CUSTOMER
// ==========================================

serveButton.addEventListener(
    "click",
    function()
    {
        if (customers.length === 0)
        {
            alert("Queue is empty!");


            servingBox.innerHTML = `
                <p class="customer-label">
                    Current Customer
                </p>

                <h3>
                    No Customer Being Served
                </h3>

                <p>
                    There are no waiting customers.
                </p>
            `;

            return;
        }


        // Highest priority customer

        const customer =
            customers.shift();


        const priorityText =
            getPriorityName(
                customer.priority
            );


        // Add to served history

        servedCustomers.push(
            customer
        );


        // Show current customer

        servingBox.innerHTML = `
            <p class="customer-label">
                Current Customer
            </p>

            <h3>
                ${customer.name}
            </h3>

            <p>
                Customer ID:
                ${customer.id}
            </p>

            <p>
                Service:
                ${customer.service}
            </p>

            <p>
                Priority:
                ${priorityText}
            </p>

            <p>
                Status:
                <strong>Served</strong>
            </p>
        `;


        // Update everything

        updateQueue();

        updateDashboard();

        updateServedHistory();
    }
);


// ==========================================
// SERVED CUSTOMER HISTORY
// ==========================================

function updateServedHistory()
{
    servedTableBody.innerHTML = "";


    if (servedCustomers.length === 0)
    {
        servedTableBody.innerHTML = `
            <tr>
                <td colspan="5">
                    No served customers yet
                </td>
            </tr>
        `;

        return;
    }


    servedCustomers.forEach(
        function(customer)
        {
            const priorityText =
                getPriorityName(
                    customer.priority
                );


            const priorityClass =
                getPriorityClass(
                    customer.priority
                );


            const row =
                document.createElement("tr");


            row.innerHTML = `
                <td>${customer.id}</td>

                <td>${customer.name}</td>

                <td>${customer.service}</td>

                <td>
                    <span class="priority ${priorityClass}">
                        ${priorityText}
                    </span>
                </td>

                <td>
                    <span class="served">
                        Served
                    </span>
                </td>
            `;


            servedTableBody.appendChild(row);
        }
    );
}


// ==========================================
// SEARCH CUSTOMER
// ==========================================

searchButton.addEventListener(
    "click",
    function()
    {
        const searchName =
            searchInput.value.trim();


        if (searchName === "")
        {
            searchResult.textContent =
                "Please enter customer name.";

            return;
        }


        // Search waiting customers

        const waitingCustomer =
            customers.find(
                function(customer)
                {
                    return customer.name.toLowerCase()
                        === searchName.toLowerCase();
                }
            );


        // Search served customers

        const servedCustomer =
            servedCustomers.find(
                function(customer)
                {
                    return customer.name.toLowerCase()
                        === searchName.toLowerCase();
                }
            );


        if (waitingCustomer)
        {
            searchResult.innerHTML = `
                Customer Found!<br>
                ID: ${waitingCustomer.id}<br>
                Name: ${waitingCustomer.name}<br>
                Service: ${waitingCustomer.service}<br>
                Priority:
                ${getPriorityName(
                    waitingCustomer.priority
                )}<br>
                Status: Waiting
            `;
        }

        else if (servedCustomer)
        {
            searchResult.innerHTML = `
                Customer Found!<br>
                ID: ${servedCustomer.id}<br>
                Name: ${servedCustomer.name}<br>
                Service: ${servedCustomer.service}<br>
                Priority:
                ${getPriorityName(
                    servedCustomer.priority
                )}<br>
                Status: Served
            `;
        }

        else
        {
            searchResult.textContent =
                "Customer not found.";
        }
    }
);


// ==========================================
// ENTER KEY FOR SEARCH
// ==========================================

searchInput.addEventListener(
    "keypress",
    function(event)
    {
        if (event.key === "Enter")
        {
            searchButton.click();
        }
    }
);


// ==========================================
// INITIAL DISPLAY
// ==========================================

updateQueue();

updateServedHistory();

updateDashboard();