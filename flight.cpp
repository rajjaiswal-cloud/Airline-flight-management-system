// ==========================================================
//   AIRLINE FLIGHT MANAGEMENT SYSTEM  (C++ OOP Mini Project)
//   Concepts: Encapsulation, Abstraction, Inheritance,
//   Polymorphism, Constructors, Destructor, Static members,
//   Friend function, Operator overloading, Function overloading
//   NOTE: While typing input, do not use spaces.
//         Use underscore instead, e.g. New_Delhi
// ==========================================================

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ----------------------------------------------------------
// ABSTRACT BASE CLASS: Person
// "= 0" makes displayInfo() a pure virtual function,
// so we cannot create an object of Person directly.
// ----------------------------------------------------------
class Person {
protected:                      // protected: child classes can use these
    string name;
    int age;

public:
    Person() {
        name = "";
        age = 0;
    }

    Person(string name, int age) {
        this->name = name;      // 'this' pointer
        this->age = age;
    }

    virtual ~Person() {}        // virtual destructor

    string getName() const { return name; }
    int getAge() const { return age; }

    virtual void displayInfo() const = 0;   // pure virtual function
};

// ----------------------------------------------------------
// DERIVED CLASS: Passenger (inherits Person)
// ----------------------------------------------------------
class Passenger : public Person {
private:
    string passport;
    string phone;

public:
    Passenger() : Person() {
        passport = "";
        phone = "";
    }

    Passenger(string name, int age, string passport, string phone)
        : Person(name, age) {
        this->passport = passport;
        this->phone = phone;
    }

    string getPassport() const { return passport; }

    // Overriding the pure virtual function
    void displayInfo() const {
        cout << "Passenger Name : " << name << endl;
        cout << "Age            : " << age << endl;
        cout << "Passport No    : " << passport << endl;
        cout << "Phone          : " << phone << endl;
    }
};

// ----------------------------------------------------------
// DERIVED CLASS: Staff (inherits Person)
// ----------------------------------------------------------
class Staff : public Person {
private:
    string role;
    int empId;

public:
    Staff() : Person() {
        role = "";
        empId = 0;
    }

    Staff(string name, int age, string role, int empId)
        : Person(name, age) {
        this->role = role;
        this->empId = empId;
    }

    void displayInfo() const {
        cout << "Staff Name : " << name << endl;
        cout << "Role       : " << role << endl;
        cout << "Employee ID: " << empId << endl;
    }
};

// ----------------------------------------------------------
// CLASS: Flight  (Encapsulation: data is private)
// Has a Staff object as pilot (Composition)
// ----------------------------------------------------------
class Flight {
private:
    int flightNo;
    string source;
    string destination;
    int totalSeats;
    int availableSeats;
    double baseFare;
    Staff pilot;                // object inside object (composition)

public:
    // Default constructor
    Flight() {
        flightNo = 0;
        source = "";
        destination = "";
        totalSeats = 0;
        availableSeats = 0;
        baseFare = 0;
    }

    // Parameterized constructor
    Flight(int no, string src, string dest, int seats, double fare, Staff p) {
        flightNo = no;
        source = src;
        destination = dest;
        totalSeats = seats;
        availableSeats = seats;
        baseFare = fare;
        pilot = p;
    }

    // Getters (const functions)
    int getFlightNo() const { return flightNo; }
    string getSource() const { return source; }
    string getDestination() const { return destination; }
    int getAvailableSeats() const { return availableSeats; }
    double getBaseFare() const { return baseFare; }

    // Reduce seats when a ticket is booked
    bool bookSeats(int n) {
        if (n > 0 && n <= availableSeats) {
            availableSeats = availableSeats - n;
            return true;
        }
        return false;
    }

    // Give seats back when a ticket is cancelled
    void cancelSeats(int n) {
        availableSeats = availableSeats + n;
    }

    // Operator overloading: two flights are same if flight numbers match
    bool operator==(const Flight &other) const {
        return flightNo == other.flightNo;
    }

    // One row of the flight table
    void display() const {
        cout << left << setw(10) << flightNo
             << setw(14) << source
             << setw(14) << destination
             << setw(8) << availableSeats
             << setw(10) << baseFare
             << pilot.getName() << endl;
    }

    // Friend function can access private members
    friend void showFlightSummary(Flight f);
};

// Friend function definition (not a member of Flight)
void showFlightSummary(Flight f) {
    cout << "\n--- Flight Summary ---" << endl;
    cout << "Flight No       : " << f.flightNo << endl;
    cout << "Route           : " << f.source << " -> " << f.destination << endl;
    cout << "Total Seats     : " << f.totalSeats << endl;
    cout << "Available Seats : " << f.availableSeats << endl;
    cout << "Base Fare       : Rs." << f.baseFare << endl;
    f.pilot.displayInfo();
}

// ----------------------------------------------------------
// ABSTRACT CLASS: Ticket
// calculateFare() is pure virtual, each ticket type
// gives its own version (Runtime Polymorphism)
// ----------------------------------------------------------
class Ticket {
protected:
    int ticketId;
    Passenger passenger;        // composition
    int flightNo;
    int seats;
    double baseFare;
    static int ticketCounter;   // static member shared by all tickets

public:
    Ticket(Passenger p, int fno, int s, double fare) {
        ticketCounter++;
        ticketId = ticketCounter;   // auto generated ID
        passenger = p;
        flightNo = fno;
        seats = s;
        baseFare = fare;
    }

    virtual ~Ticket() {}        // virtual destructor

    int getTicketId() const { return ticketId; }
    int getFlightNo() const { return flightNo; }
    int getSeats() const { return seats; }

    virtual double calculateFare() const = 0;    // pure virtual
    virtual string getClassName() const = 0;     // pure virtual

    // One row of the bookings table
    void display() const {
        cout << left << setw(10) << ticketId
             << setw(14) << passenger.getName()
             << setw(10) << flightNo
             << setw(12) << getClassName()
             << setw(8) << seats
             << calculateFare() << endl;
    }

    // Full ticket print
    void showTicket() const {
        cout << "\n====== TICKET ======" << endl;
        cout << "Ticket ID      : " << ticketId << endl;
        passenger.displayInfo();
        cout << "Flight No      : " << flightNo << endl;
        cout << "Class          : " << getClassName() << endl;
        cout << "Seats          : " << seats << endl;
        cout << "Total Fare     : Rs." << calculateFare() << endl;
        cout << "====================" << endl;
    }
};

// Initialise the static member outside the class
int Ticket::ticketCounter = 1000;

// ----------------------------------------------------------
// DERIVED CLASSES of Ticket
// ----------------------------------------------------------
class EconomyTicket : public Ticket {
public:
    EconomyTicket(Passenger p, int fno, int s, double fare)
        : Ticket(p, fno, s, fare) {}

    double calculateFare() const {
        return baseFare * seats;
    }

    string getClassName() const { return "Economy"; }
};

class BusinessTicket : public Ticket {
public:
    BusinessTicket(Passenger p, int fno, int s, double fare)
        : Ticket(p, fno, s, fare) {}

    double calculateFare() const {
        // double fare + lounge charge of 2000 per seat
        return (baseFare * 2 + 2000) * seats;
    }

    string getClassName() const { return "Business"; }
};

class FirstClassTicket : public Ticket {
public:
    FirstClassTicket(Passenger p, int fno, int s, double fare)
        : Ticket(p, fno, s, fare) {}

    double calculateFare() const {
        // triple fare + lounge and meal charge of 5000 per seat
        return (baseFare * 3 + 5000) * seats;
    }

    string getClassName() const { return "First"; }
};

// ----------------------------------------------------------
// MANAGER CLASS: FlightSystem
// Holds arrays of flights and ticket pointers
// ----------------------------------------------------------
class FlightSystem {
private:
    Flight flights[50];
    int flightCount;
    Ticket *tickets[100];       // base class pointers (polymorphism)
    int ticketCount;

    // Find the position of a flight in the array, -1 if not found
    int findFlightIndex(int no) {
        for (int i = 0; i < flightCount; i++) {
            if (flights[i].getFlightNo() == no) {
                return i;
            }
        }
        return -1;
    }

    // Print the heading of the flight table
    void printFlightHeader() {
        cout << left << setw(10) << "FlightNo"
             << setw(14) << "From"
             << setw(14) << "To"
             << setw(8) << "Seats"
             << setw(10) << "Fare"
             << "Pilot" << endl;
        cout << "-----------------------------------------------------------" << endl;
    }

    // Add some sample data at the start
    void loadSampleData() {
        flights[0] = Flight(101, "Ahmedabad", "Delhi", 60, 5000, Staff("Capt_Sharma", 45, "Pilot", 1));
        flights[1] = Flight(102, "Mumbai", "Bangalore", 50, 4500, Staff("Capt_Mehta", 42, "Pilot", 2));
        flights[2] = Flight(103, "Delhi", "Goa", 40, 6000, Staff("Capt_Singh", 48, "Pilot", 3));
        flights[3] = Flight(104, "Ahmedabad", "Mumbai", 30, 3500, Staff("Capt_Patel", 39, "Pilot", 4));
        flightCount = 4;
    }

public:
    // Constructor
    FlightSystem() {
        flightCount = 0;
        ticketCount = 0;
        loadSampleData();
    }

    // Destructor: free all tickets created with 'new'
    ~FlightSystem() {
        for (int i = 0; i < ticketCount; i++) {
            delete tickets[i];
        }
    }

    // 1. Add a flight
    void addFlight() {
        if (flightCount >= 50) {
            cout << "Flight list is full!" << endl;
            return;
        }

        int no, seats, pilotAge, pilotId;
        string src, dest, pilotName;
        double fare;

        cout << "Enter flight number : ";
        cin >> no;
        cout << "Enter source        : ";
        cin >> src;
        cout << "Enter destination   : ";
        cin >> dest;
        cout << "Enter total seats   : ";
        cin >> seats;
        cout << "Enter base fare     : ";
        cin >> fare;
        cout << "Enter pilot name    : ";
        cin >> pilotName;
        cout << "Enter pilot age     : ";
        cin >> pilotAge;
        cout << "Enter pilot ID      : ";
        cin >> pilotId;

        Staff pilot(pilotName, pilotAge, "Pilot", pilotId);
        Flight newFlight(no, src, dest, seats, fare, pilot);

        // Use the overloaded == operator to check duplicates
        for (int i = 0; i < flightCount; i++) {
            if (flights[i] == newFlight) {
                cout << "A flight with this number already exists!" << endl;
                return;
            }
        }

        flights[flightCount] = newFlight;
        flightCount++;
        cout << "Flight added successfully!" << endl;
        showFlightSummary(newFlight);   // friend function
    }

    // 2. View all flights
    void viewFlights() {
        if (flightCount == 0) {
            cout << "No flights available." << endl;
            return;
        }
        cout << "\n";
        printFlightHeader();
        for (int i = 0; i < flightCount; i++) {
            flights[i].display();
        }
    }

    // 3. Book a ticket
    void bookTicket() {
        int fno, age, classChoice, seats;
        string pname, passport, phone;

        cout << "Enter flight number : ";
        cin >> fno;

        int index = findFlightIndex(fno);
        if (index == -1) {
            cout << "Flight not found!" << endl;
            return;
        }
        if (ticketCount >= 100) {
            cout << "Ticket list is full!" << endl;
            return;
        }

        cout << "Seats available     : " << flights[index].getAvailableSeats() << endl;
        cout << "Enter passenger name: ";
        cin >> pname;
        cout << "Enter age           : ";
        cin >> age;
        cout << "Enter passport no   : ";
        cin >> passport;
        cout << "Enter phone         : ";
        cin >> phone;
        cout << "Number of seats     : ";
        cin >> seats;
        cout << "Choose class (1.Economy  2.Business  3.First): ";
        cin >> classChoice;

        // Reserve seats first
        if (!flights[index].bookSeats(seats)) {
            cout << "Not enough seats available (or invalid seat count)!" << endl;
            return;
        }

        Passenger p(pname, age, passport, phone);
        double fare = flights[index].getBaseFare();

        // Base class pointer holds a derived class object
        Ticket *t;
        if (classChoice == 1) {
            t = new EconomyTicket(p, fno, seats, fare);
        } else if (classChoice == 2) {
            t = new BusinessTicket(p, fno, seats, fare);
        } else if (classChoice == 3) {
            t = new FirstClassTicket(p, fno, seats, fare);
        } else {
            cout << "Invalid class choice!" << endl;
            flights[index].cancelSeats(seats);   // undo seat reservation
            return;
        }

        tickets[ticketCount] = t;
        ticketCount++;

        cout << "Ticket booked successfully!" << endl;
        t->showTicket();        // calls the correct calculateFare() automatically
    }

    // 4. Cancel a ticket
    void cancelTicket() {
        int id;
        cout << "Enter ticket ID to cancel: ";
        cin >> id;

        int index = -1;
        for (int i = 0; i < ticketCount; i++) {
            if (tickets[i]->getTicketId() == id) {
                index = i;
            }
        }

        if (index == -1) {
            cout << "Ticket not found!" << endl;
            return;
        }

        // Give the seats back to the flight
        int fi = findFlightIndex(tickets[index]->getFlightNo());
        if (fi != -1) {
            flights[fi].cancelSeats(tickets[index]->getSeats());
        }

        cout << "Ticket " << id << " cancelled. Refund amount: Rs."
             << tickets[index]->calculateFare() << endl;

        delete tickets[index];

        // Shift remaining tickets one place to the left
        for (int i = index; i < ticketCount - 1; i++) {
            tickets[i] = tickets[i + 1];
        }
        ticketCount--;
    }

    // 5a. Search flight by number  (function overloading)
    void searchFlight(int no) {
        int index = findFlightIndex(no);
        if (index == -1) {
            cout << "No flight found with number " << no << endl;
        } else {
            showFlightSummary(flights[index]);
        }
    }

    // 5b. Search flight by destination  (function overloading)
    void searchFlight(string dest) {
        bool found = false;
        for (int i = 0; i < flightCount; i++) {
            if (flights[i].getDestination() == dest) {
                if (!found) {
                    cout << "\n";
                    printFlightHeader();
                }
                flights[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No flights found for destination " << dest << endl;
        }
    }

    // 6. View all bookings
    void viewBookings() {
        if (ticketCount == 0) {
            cout << "No bookings yet." << endl;
            return;
        }
        cout << "\n";
        cout << left << setw(10) << "TicketID"
             << setw(14) << "Passenger"
             << setw(10) << "Flight"
             << setw(12) << "Class"
             << setw(8) << "Seats"
             << "Fare" << endl;
        cout << "---------------------------------------------------------" << endl;
        for (int i = 0; i < ticketCount; i++) {
            tickets[i]->display();      // polymorphic call
        }
    }
};

// ----------------------------------------------------------
// MAIN FUNCTION
// ----------------------------------------------------------
int main() {
    FlightSystem airline;
    int choice;

    do {
        cout << "\n===== AIRLINE MANAGEMENT SYSTEM =====" << endl;
        cout << "1. Add Flight" << endl;
        cout << "2. View Flights" << endl;
        cout << "3. Book Ticket" << endl;
        cout << "4. Cancel Ticket" << endl;
        cout << "5. Search Flight" << endl;
        cout << "6. View Bookings" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            airline.addFlight();
            break;
        case 2:
            airline.viewFlights();
            break;
        case 3:
            airline.bookTicket();
            break;
        case 4:
            airline.cancelTicket();
            break;
        case 5: {
            int type;
            cout << "Search by 1.Flight Number  2.Destination: ";
            cin >> type;
            if (type == 1) {
                int no;
                cout << "Enter flight number: ";
                cin >> no;
                airline.searchFlight(no);       // calls the int version
            } else if (type == 2) {
                string dest;
                cout << "Enter destination: ";
                cin >> dest;
                airline.searchFlight(dest);     // calls the string version
            } else {
                cout << "Invalid option!" << endl;
            }
            break;
        }
        case 6:
            airline.viewBookings();
            break;
        case 7:
            cout << "Thank you for using the system. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 7);

    return 0;
}