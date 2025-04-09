#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <memory>

using namespace std;

//Singleton Pattern through Session
class Session {
private:
    static Session* instance;
    string currentUser;
    Session() {}

public:
    static Session* getInstance() {
        if (!instance)
            instance = new Session();
        return instance;
    }

    void login(const string& user) { currentUser = user; }
    string getUser() const { return currentUser; }
};
Session* Session::instance = nullptr;

//Chain of Responsibility through Security Questions
class SecurityQuestionHandler {
    public:
        virtual ~SecurityQuestionHandler() {}
        virtual bool handleSecurityQuestion(const string& question, const string& answer) = 0;
    };
    
    class FirstSecurityQuestionHandler : public SecurityQuestionHandler {
    private:
        unique_ptr<SecurityQuestionHandler> nextHandler;
    
    public:
        FirstSecurityQuestionHandler(unique_ptr<SecurityQuestionHandler> next) : nextHandler(move(next)) {}
    
        bool handleSecurityQuestion(const string& question, const string& answer) override {
            if (question == "What is your mother's maiden name?") {
                string correctAnswer = "Smith";  // Sample answer
                if (answer == correctAnswer) {
                    cout << "Answer correct!\n";
                    return true;
                } else {
                    cout << "Answer incorrect. Proceeding to the next question...\n";
                    return nextHandler ? nextHandler->handleSecurityQuestion(question, answer) : false;
                }
            }
            return nextHandler ? nextHandler->handleSecurityQuestion(question, answer) : false;
        }
    };
    
    class SecondSecurityQuestionHandler : public SecurityQuestionHandler {
    private:
        unique_ptr<SecurityQuestionHandler> nextHandler;
    
    public:
        SecondSecurityQuestionHandler(unique_ptr<SecurityQuestionHandler> next) : nextHandler(move(next)) {}
    
        bool handleSecurityQuestion(const string& question, const string& answer) override {
            if (question == "What was the name of your first pet?") {
                string correctAnswer = "Buddy";  // Sample answer
                if (answer == correctAnswer) {
                    cout << "Answer correct!\n";
                    return true;
                } else {
                    cout << "Answer incorrect. Proceeding to the next question...\n";
                    return nextHandler ? nextHandler->handleSecurityQuestion(question, answer) : false;
                }
            }
            return nextHandler ? nextHandler->handleSecurityQuestion(question, answer) : false;
        }
    };
    
    class ThirdSecurityQuestionHandler : public SecurityQuestionHandler {
    private:
        unique_ptr<SecurityQuestionHandler> nextHandler;
    
    public:
        ThirdSecurityQuestionHandler(unique_ptr<SecurityQuestionHandler> next) : nextHandler(move(next)) {}
    
        bool handleSecurityQuestion(const string& question, const string& answer) override {
            if (question == "What is the name of your high school?") {
                string correctAnswer = "Saline";  // Sample answer
                if (answer == correctAnswer) {
                    cout << "Answer correct!\n";
                    return true;
                } else {
                    cout << "Answer incorrect.\n";
                    return false;
                }
            }
            return nextHandler ? nextHandler->handleSecurityQuestion(question, answer) : false;
        }
    };

//User Class
class User {
    public:
        string email;
        string password;
        vector<string> securityQuestions;
        vector<string> answers;
    
        string serialize() const {
            stringstream ss;
            ss << email << "," << password;
            for (const auto& answer : answers) {
                ss << "," << answer;
            }
            return ss.str() + "\n";
        }
    
        static User deserialize(const string& line) {
            User user;
            stringstream ss(line);
            string token;
            
            getline(ss, user.email, ',');
            getline(ss, user.password, ',');
            
            while (getline(ss, token, ',')) {
                user.answers.push_back(token);
            }
            
            return user;
        }
    };
    
    //User Authentification and Registeration
    class UserManager {
    private:
        vector<User> users;
    
        void loadUsers() {
            ifstream inFile("users.txt");
            string line;
            while (getline(inFile, line)) {
                users.push_back(User::deserialize(line));
            }
            inFile.close();
        }
    
        void saveUsers() {
            ofstream outFile("users.txt");
            for (const auto& user : users) {
                outFile << user.serialize();
            }
            outFile.close();
        }
    
    public:
        UserManager() { loadUsers(); }
    
        bool registerUser(const string& email, const string& password, const vector<string>& answers) {
            // Check if the email is already registered
            for (const auto& user : users) {
                if (user.email == email) {
                    cout << "This email is already registered.\n";
                    return false;
                }
            }
    
            User newUser = {email, password, answers};
            users.push_back(newUser);
            saveUsers();
            cout << "User registered successfully!\n";
            return true;
        }
    
        bool authenticateUser(const string& email, const string& password, const vector<string>& answers) {
            for (const auto& user : users) {
                if (user.email == email && user.password == password) {
                    // Verify answers to security questions
                    for (size_t i = 0; i < user.answers.size(); ++i) {
                        if (user.answers[i] != answers[i]) {
                            cout << "Security question answer incorrect.\n";
                            return false;
                        }
                    }
                    return true;  // Authentication successful
                }
            }
            cout << "Email or password incorrect.\n";
            return false;
        }
    };

//Car Management(Builder Pattern)
class Car {
public:
    string model;
    int year;
    int mileage;
    string location;
    float price;
    string owner;
    bool isAvailable;
    string availabilityStartDate;  // Added for availability management
    string availabilityEndDate;    // Added for availability management

    void display() const {
        cout << "Model: " << model << ", Year: " << year << ", Mileage: " << mileage
             << ", Location: " << location << ", Price: $" << price << ", Owner: " << owner
             << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    string serialize() const {
        return model + "," + to_string(year) + "," + to_string(mileage) + "," + location + ","
            + to_string(price) + "," + owner + "," + (isAvailable ? "1" : "0") + ","
            + availabilityStartDate + "," + availabilityEndDate + "\n";
    }

    static Car deserialize(const string& line) {
        Car car;
        stringstream ss(line);
        string token;
        
        getline(ss, car.model, ',');
        getline(ss, token, ','); car.year = stoi(token);
        getline(ss, token, ','); car.mileage = stoi(token);
        getline(ss, car.location, ',');
        getline(ss, token, ','); car.price = stof(token);
        getline(ss, car.owner, ',');
        getline(ss, token, ','); car.isAvailable = (token == "1");
        getline(ss, car.availabilityStartDate, ',');
        getline(ss, car.availabilityEndDate, ',');
        
        return car;
    }
};

class CarManager {
private:
    vector<Car> cars;
    
    void loadCars() {
        ifstream inFile("cars.txt");
        string line;
        while (getline(inFile, line)) {
            cars.push_back(Car::deserialize(line));
        }
        inFile.close();
    }
    
    void saveCars() {
        ofstream outFile("cars.txt");
        for (const auto& car : cars) {
            outFile << car.serialize();
        }
        outFile.close();
    }

public:
    CarManager() { loadCars(); }

    void addCar(const Car& car) {
        cars.push_back(car);
        saveCars();
    }

    bool updateCarAvailability(const string& model, const string& newStartDate, const string& newEndDate) {
        for (auto& car : cars) {
            if (car.model == model) {
                car.availabilityStartDate = newStartDate;
                car.availabilityEndDate = newEndDate;
                car.isAvailable = true;
                saveCars();
                return true;
            }
        }
        return false;
    }

    bool removeCar(const string& model) {
        auto it = remove_if(cars.begin(), cars.end(), [&model](const Car& car) { return car.model == model; });
        if (it != cars.end()) {
            cars.erase(it, cars.end()); // Erase the cars from the vector after removing
            saveCars();
            return true;
        }
        return false;
    }

    vector<Car> searchCars(const string& location, const string& startDate, const string& endDate) {
        vector<Car> results;
        for (const auto& car : cars) {
            if (car.location == location && car.isAvailable &&
                car.availabilityStartDate <= startDate && car.availabilityEndDate >= endDate) {
                results.push_back(car);
            }
        }
        return results;
    }
};

//Booking System(Updates assisted by Observer Pattern)
class Booking {
public:
    string renter;
    string carModel;
    string startDate;
    string endDate;
    bool isConfirmed;

    string serialize() const {
        return renter + "," + carModel + "," + startDate + "," + endDate + "," + (isConfirmed ? "1" : "0") + "\n";
    }

    static Booking deserialize(const string& line) {
        Booking booking;
        stringstream ss(line);
        string token;
        
        getline(ss, booking.renter, ',');
        getline(ss, booking.carModel, ',');
        getline(ss, booking.startDate, ',');
        getline(ss, booking.endDate, ',');
        getline(ss, token, ','); booking.isConfirmed = (token == "1");
        
        return booking;
    }
};

class BookingManager {
private:
    vector<Booking> bookings;

    void loadBookings() {
        ifstream inFile("bookings.txt");
        string line;
        while (getline(inFile, line)) {
            bookings.push_back(Booking::deserialize(line));
        }
        inFile.close();
    }

    void saveBookings() {
        ofstream outFile("bookings.txt");
        for (const auto& booking : bookings) {
            outFile << booking.serialize();
        }
        outFile.close();
    }

public:
    BookingManager() { loadBookings(); }

    bool createBooking(const string& renter, const string& carModel, const string& startDate, const string& endDate) {
        for (const auto& booking : bookings) {
            if (booking.carModel == carModel && booking.startDate <= endDate && booking.endDate >= startDate) {
                cout << "Car is not available for the selected dates!" << endl;
                return false;
            }
        }

        Booking newBooking = {renter, carModel, startDate, endDate, false};
        bookings.push_back(newBooking);
        saveBookings();
        return true;
    }

    void confirmBooking(const string& renter, const string& carModel) {
        for (auto& booking : bookings) {
            if (booking.renter == renter && booking.carModel == carModel) {
                booking.isConfirmed = true;
                saveBookings();
                cout << "Booking confirmed!" << endl;
                return;
            }
        }
        cout << "Booking not found!" << endl;
    }
};

//MessagingSystem(Meditator Pattern)
class MessagingSystem {
public:
    void sendNotification(const string& recipient, const string& message) {
        cout << "[Notification to " << recipient << "]: " << message << endl;
    }
};

//Payment Method(Proxy Design Pattern)
class Payment {
public:
    virtual void process(float amount) = 0;
};

class RealPayment : public Payment {
public:
    void process(float amount) override {
        cout << "Processing payment of $" << amount << endl;
    }
};

class PaymentProxy : public Payment {
private:
    RealPayment realPayment;

public:
    void process(float amount) override {
        cout << "[Proxy] Validating payment..." << endl;
        realPayment.process(amount);
    }
};

//Testing
int main() {
    UserManager userManager;
    CarManager carManager;
    BookingManager bookingManager;

    // User Registration
    cout << "User Registration\n";
    string email, password;
    vector<string> answers(3);

    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your password: ";
    getline(cin, password);
    cout << "Answer the following security questions:\n";
    cout << "What is your mother's maiden name? ";
    cin >> answers[0];
    cout << "What was the name of your first pet? ";
    cin >> answers[1];
    cout << "What is the name of your high school? ";
    cin >> answers[2];

    if (!userManager.registerUser(email, password, answers)) {
        cout << "Registration failed.\n";
        return 1;
    }

    // User Authentication
    cout << "\nUser Authentication\n";
    string authEmail, authPassword;
    vector<string> authAnswers(3);

    cout << "Enter your email: ";
    cin.ignore();
    getline(cin, authEmail);
    cout << "Enter your password: ";
    getline(cin, authPassword);
    cout << "Answer the following security questions:\n";
    cout << "What is your mother's maiden name? ";
    cin >> authAnswers[0];
    cout << "What was the name of your first pet? ";
    cin >> authAnswers[1];
    cout << "What is the name of your high school? ";
    cin >> authAnswers[2];

    if (userManager.authenticateUser(authEmail, authPassword, authAnswers)) {
        cout << "Authentication successful! Welcome, " << authEmail << ".\n";
    } else {
        cout << "Authentication failed.\n";
        return 1;
    }

    // Car Listing
    cout << "\nEnter Car Listing Details\n";
    string model, location, startDate, endDate;
    int year, mileage;
    float price;
    bool availability;

    cout << "Enter Car Model: ";
    cin >> model;
    cout << "Enter Year: ";
    cin >> year;
    cout << "Enter Mileage: ";
    cin >> mileage;
    cout << "Enter Location: ";
    cin >> location;
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Availability Start Date: ";
    cin >> startDate;
    cout << "Enter Availability End Date: ";
    cin >> endDate;
    cout << "Is Available (1 for Yes, 0 for No): ";
    cin >> availability;

    Car newCar = {model, year, mileage, location, price, authEmail, availability, startDate, endDate};
    carManager.addCar(newCar);

    // Car Search
    cout << "\nEnter Location and Dates to Search for Cars:\n";
    string searchLocation, searchStartDate, searchEndDate;
    cout << "Enter location to search: ";
    cin >> searchLocation;
    cout << "Enter start date (YYYY-MM-DD): ";
    cin >> searchStartDate;
    cout << "Enter end date (YYYY-MM-DD): ";
    cin >> searchEndDate;

    vector<Car> availableCars = carManager.searchCars(searchLocation, searchStartDate, searchEndDate);
    cout << "Available Cars: \n";
    for (const auto& car : availableCars) {
        car.display();
    }

    // Car Booking
    cout << "\nEnter Car Model to Book: ";
    cin >> model;
    cout << "Enter start date (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "Enter end date (YYYY-MM-DD): ";
    cin >> endDate;

    if (bookingManager.createBooking(authEmail, model, startDate, endDate)) {
        cout << "Booking successful. Awaiting confirmation...\n";
    }

    bookingManager.confirmBooking(authEmail, model);

    // Payment
    Payment* payment = new PaymentProxy();
    payment->process(price);
    delete payment;

    return 0;
}
