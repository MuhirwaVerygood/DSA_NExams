#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// Patient structure
struct Patient {
    int patient_id;
    string name;
    string dob;
    string gender;
    Patient* next;
};

// Doctor structure
struct Doctor {
    int doctor_id;
    string name;
    string specialization;
    Doctor* next;
};

// Appointment structure
struct Appointment {
    int appointment_id;
    int patient_id;
    int doctor_id;
    string appointment_date;
    Appointment* next;
};

// Hospital structure
struct Hospital {
    string hospital_id;
    string name;
    Patient* patients;
    Doctor* doctors;
    Appointment* appointments;
};

// Graph edge structure
struct Edge {
    string to_hospital;
    float distance;
};

// Linked list management class for each hospital
class HospitalLists {
private:
    Patient* patients_head;
    Doctor* doctors_head;
    Appointment* appointments_head;
    string hospital_id;

public:
    HospitalLists(string id) : patients_head(nullptr), doctors_head(nullptr), appointments_head(nullptr), hospital_id(id) {}

    // Add patient
    bool addPatient(int id, string name, string dob, string gender) {
        if (findPatient(id)) {
            cout << "Error: Patient ID " << id << " already exists.\n";
            return false;
        }
        Patient* newPatient = new Patient{id, name, dob, gender, patients_head};
        patients_head = newPatient;
        savePatients();
        return true;
    }

    // Add doctor
    bool addDoctor(int id, string name, string specialization) {
        if (findDoctor(id)) {
            cout << "Error: Doctor ID " << id << " already exists.\n";
            return false;
        }
        Doctor* newDoctor = new Doctor{id, name, specialization, doctors_head};
        doctors_head = newDoctor;
        saveDoctors();
        return true;
    }

    // Add appointment
    bool addAppointment(int app_id, int patient_id, int doctor_id, string date) {
        if (findAppointment(app_id)) {
            cout << "Error: Appointment ID " << app_id << " already exists.\n";
            return false;
        }
        if (!findPatient(patient_id)) {
            cout << "Error: Patient ID " << patient_id << " does not exist.\n";
            return false;
        }
        if (!findDoctor(doctor_id)) {
            cout << "Error: Doctor ID " << doctor_id << " does not exist.\n";
            return false;
        }
        Appointment* newApp = new Appointment{app_id, patient_id, doctor_id, date, appointments_head};
        appointments_head = newApp;
        saveAppointments();
        return true;
    }

    // Display patients
    void displayPatients() {
        if (!patients_head) {
            cout << "No patients registered.\n";
            return;
        }
        cout << "\n" << left
             << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(15) << "DOB"
             << setw(10) << "Gender" << "\n";
        cout << string(55, '-') << "\n";
        Patient* current = patients_head;
        while (current) {
            cout << left
                 << setw(10) << current->patient_id
                 << setw(20) << current->name
                 << setw(15) << current->dob
                 << setw(10) << current->gender << "\n";
            current = current->next;
        }
    }

    // Display doctors
    void displayDoctors() {
        if (!doctors_head) {
            cout << "No doctors registered.\n";
            return;
        }
        cout << "\n" << left
             << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(20) << "Specialization" << "\n";
        cout << string(50, '-') << "\n";
        Doctor* current = doctors_head;
        while (current) {
            cout << left
                 << setw(10) << current->doctor_id
                 << setw(20) << current->name
                 << setw(20) << current->specialization << "\n";
            current = current->next;
        }
    }

    // Display appointments
    void displayAppointments() {
        if (!appointments_head) {
            cout << "No appointments registered.\n";
            return;
        }
        cout << "\n" << left
             << setw(15) << "Appointment ID"
             << setw(15) << "Patient ID"
             << setw(15) << "Doctor ID"
             << setw(15) << "Date" << "\n";
        cout << string(60, '-') << "\n";
        Appointment* current = appointments_head;
        while (current) {
            cout << left
                 << setw(15) << current->appointment_id
                 << setw(15) << current->patient_id
                 << setw(15) << current->doctor_id
                 << setw(15) << current->appointment_date << "\n";
            current = current->next;
        }
    }

    // Find patient by ID
    bool findPatient(int id) {
        Patient* current = patients_head;
        while (current) {
            if (current->patient_id == id) return true;
            current = current->next;
        }
        return false;
    }

    // Find doctor by ID
    bool findDoctor(int id) {
        Doctor* current = doctors_head;
        while (current) {
            if (current->doctor_id == id) return true;
            current = current->next;
        }
        return false;
    }

    // Find appointment by ID
    bool findAppointment(int id) {
        Appointment* current = appointments_head;
        while (current) {
            if (current->appointment_id == id) return true;
            current = current->next;
        }
        return false;
    }

    // Save patients to CSV
    void savePatients() {
        ofstream file(hospital_id + "_patients.csv");
        file << "PatientID,Name,DOB,Gender\n";
        Patient* current = patients_head;
        while (current) {
            file << current->patient_id << "," << current->name << "," << current->dob << "," << current->gender << "\n";
            current = current->next;
        }
        file.close();
    }

    // Save doctors to CSV
    void saveDoctors() {
        ofstream file(hospital_id + "_doctors.csv");
        file << "DoctorID,Name,Specialization\n";
        Doctor* current = doctors_head;
        while (current) {
            file << current->doctor_id << "," << current->name << "," << current->specialization << "\n";
            current = current->next;
        }
        file.close();
    }

    // Save appointments to CSV
    void saveAppointments() {
        ofstream file(hospital_id + "_appointments.csv");
        file << "AppointmentID,PatientID,DoctorID,Date\n";
        Appointment* current = appointments_head;
        while (current) {
            file << current->appointment_id << "," << current->patient_id << "," << current->doctor_id << "," << current->appointment_date << "\n";
            current = current->next;
        }
        file.close();
    }

    // Load patients from CSV
    void loadPatients() {
        ifstream file(hospital_id + "_patients.csv");
        if (!file.is_open()) return;
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            Patient* p = new Patient;
            string id;
            getline(ss, id, ',');
            p->patient_id = stoi(id);
            getline(ss, p->name, ',');
            getline(ss, p->dob, ',');
            getline(ss, p->gender);
            p->next = patients_head;
            patients_head = p;
        }
        file.close();
    }

    // Load doctors from CSV
    void loadDoctors() {
        ifstream file(hospital_id + "_doctors.csv");
        if (!file.is_open()) return;
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            Doctor* d = new Doctor;
            string id;
            getline(ss, id, ',');
            d->doctor_id = stoi(id);
            getline(ss, d->name, ',');
            getline(ss, d->specialization);
            d->next = doctors_head;
            doctors_head = d;
        }
        file.close();
    }

    // Load appointments from CSV
    void loadAppointments() {
        ifstream file(hospital_id + "_appointments.csv");
        if (!file.is_open()) return;
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            Appointment* a = new Appointment;
            string id, pid, did;
            getline(ss, id, ',');
            a->appointment_id = stoi(id);
            getline(ss, pid, ',');
            a->patient_id = stoi(pid);
            getline(ss, did, ',');
            a->doctor_id = stoi(did);
            getline(ss, a->appointment_date);
            a->next = appointments_head;
            appointments_head = a;
        }
        file.close();
    }

    // Destructor to free memory
    ~HospitalLists() {
        while (patients_head) {
            Patient* temp = patients_head;
            patients_head = patients_head->next;
            delete temp;
        }
        while (doctors_head) {
            Doctor* temp = doctors_head;
            doctors_head = doctors_head->next;
            delete temp;
        }
        while (appointments_head) {
            Appointment* temp = appointments_head;
            appointments_head = appointments_head->next;
            delete temp;
        }
    }
};

// Graph class for hospital network
class HospitalGraph {
private:
    map<string, Hospital> hospitals;
    map<string, vector<Edge>> adjList;

public:
    HospitalGraph() {
        loadHospitals();
        loadConnections();
    }

    // Add hospital
    bool addHospital(string id, string name) {
        if (hospitals.find(id) != hospitals.end()) {
            cout << "Error: Hospital ID " << id << " already exists.\n";
            return false;
        }
        Hospital h = {id, name, nullptr, nullptr, nullptr};
        hospitals[id] = h;
        saveHospitals();
        return true;
    }

    // Update hospital
    bool updateHospital(string id, string newName) {
        if (hospitals.find(id) == hospitals.end()) {
            cout << "Error: Hospital ID " << id << " not found.\n";
            return false;
        }
        hospitals[id].name = newName;
        saveHospitals();
        return true;
    }

    // Delete hospital
    bool deleteHospital(string id) {
        if (hospitals.find(id) == hospitals.end()) {
            cout << "Error: Hospital ID " << id << " not found.\n";
            return false;
        }
        hospitals.erase(id);
        adjList.erase(id);
        for (auto& pair : adjList) {
            pair.second.erase(
                remove_if(pair.second.begin(), pair.second.end(),
                    [id](const Edge& e) { return e.to_hospital == id; }),
                pair.second.end()
            );
        }
        remove((id + "_patients.csv").c_str());
        remove((id + "_doctors.csv").c_str());
        remove((id + "_appointments.csv").c_str());
        saveHospitals();
        saveConnections();
        return true;
    }

    // Add connection
    bool addConnection(string from_id, string to_id, float distance) {
        if (hospitals.find(from_id) == hospitals.end() || hospitals.find(to_id) == hospitals.end()) {
            cout << "Error: One or both Hospital IDs not found.\n";
            return false;
        }
        if (from_id == to_id) {
            cout << "Error: Cannot connect a hospital to itself.\n";
            return false;
        }
        for (const auto& edge : adjList[from_id]) {
            if (edge.to_hospital == to_id) {
                cout << "Error: Connection already exists.\n";
                return false;
            }
        }
        adjList[from_id].push_back({to_id, distance});
        adjList[to_id].push_back({from_id, distance});
        saveConnections();
        return true;
    }

    // Display hospitals and connections
    void displayNetwork() {
        if (hospitals.empty()) {
            cout << "No hospitals registered.\n";
            return;
        }
        cout << "\n" << left
             << setw(15) << "Hospital ID"
             << setw(25) << "Name"
             << setw(30) << "Connections (ID:Distance)" << "\n";
        cout << string(70, '-') << "\n";
        for (const auto& pair : hospitals) {
            string connections;
            for (const auto& edge : adjList[pair.first]) {
                connections += edge.to_hospital + ":" + to_string(edge.distance) + "; ";
            }
            if (connections.empty()) connections = "None";
            else connections.pop_back(); // Remove trailing semicolon
            cout << left
                 << setw(15) << pair.first
                 << setw(25) << pair.second.name
                 << setw(30) << connections << "\n";
        }
    }

    // Get hospital lists
    HospitalLists* getHospitalLists(string id) {
        if (hospitals.find(id) == hospitals.end()) return nullptr;
        HospitalLists* lists = new HospitalLists(id);
        lists->loadPatients();
        lists->loadDoctors();
        lists->loadAppointments();
        return lists;
    }

    // Save hospitals to CSV
    void saveHospitals() {
        ofstream file("hospitals.csv");
        file << "HospitalID,Name\n";
        for (const auto& pair : hospitals) {
            file << pair.first << "," << pair.second.name << "\n";
        }
        file.close();
    }

    // Load hospitals from CSV
    void loadHospitals() {
        ifstream file("hospitals.csv");
        if (!file.is_open()) return;
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name;
            getline(ss, id, ',');
            getline(ss, name);
            Hospital h = {id, name, nullptr, nullptr, nullptr};
            hospitals[id] = h;
        }
        file.close();
    }

    // Save connections to CSV
    void saveConnections() {
        ofstream file("hospital_connections.csv");
        file << "FromID,ToID,DistanceKM\n";
        set<pair<string, string>> added;
        for (const auto& pair : adjList) {
            for (const auto& edge : pair.second) {
                string edge_key = min(pair.first, edge.to_hospital) + "," + max(pair.first, edge.to_hospital);
                if (added.find({min(pair.first, edge.to_hospital), max(pair.first, edge.to_hospital)}) == added.end()) {
                    file << pair.first << "," << edge.to_hospital << "," << fixed << setprecision(2) << edge.distance << "\n";
                    added.insert({min(pair.first, edge.to_hospital), max(pair.first, edge.to_hospital)});
                }
            }
        }
        file.close();
    }

    // Load connections from CSV
    void loadConnections() {
        ifstream file("hospital_connections.csv");
        if (!file.is_open()) return;
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string from_id, to_id, distance;
            getline(ss, from_id, ',');
            getline(ss, to_id, ',');
            getline(ss, distance);
            try {
                float dist = stof(distance);
                adjList[from_id].push_back({to_id, dist});
                adjList[to_id].push_back({from_id, dist});
            } catch (...) {
                cout << "Error parsing connection: " << line << "\n";
            }
        }
        file.close();
    }

    // List hospital IDs
    vector<string> getHospitalIDs() {
        vector<string> ids;
        for (const auto& pair : hospitals) {
            ids.push_back(pair.first);
        }
        return ids;
    }
};

// Main program
int main() {
    HospitalGraph graph;
    string current_hospital;
    HospitalLists* current_lists = nullptr;

    while (true) {
        cout << "\n==== Multi-Hospital Management System ====\n";
        if (current_hospital.empty()) {
            cout << "1. Add Hospital\n";
            cout << "2. Update Hospital\n";
            cout << "3. Delete Hospital\n";
            cout << "4. Add Hospital Connection\n";
            cout << "5. View Hospital Network\n";
            cout << "6. Select Hospital\n";
            cout << "7. Exit\n";
            cout << "Enter choice: ";
        } else {
            cout << "Current Hospital: " << current_hospital << "\n";
            cout << "1. Register Patient\n";
            cout << "2. Register Doctor\n";
            cout << "3. Register Appointment\n";
            cout << "4. Display Patients\n";
            cout << "5. Display Doctors\n";
            cout << "6. Display Appointments\n";
            cout << "7. Go to Another Hospital\n";
            cout << "8. Exit\n";
            cout << "Enter choice: ";
        }

        string input;
        getline(cin, input);
        int choice;
        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (current_hospital.empty()) {
            switch (choice) {
                case 1: {
                    string id, name;
                    cout << "Enter Hospital ID: ";
                    getline(cin, id);
                    cout << "Enter Hospital Name: ";
                    getline(cin, name);
                    if (graph.addHospital(id, name)) {
                        cout << "Hospital added successfully.\n";
                    }
                    break;
                }
                case 2: {
                    string id, name;
                    cout << "Enter Hospital ID to update: ";
                    getline(cin, id);
                    cout << "Enter new Hospital Name: ";
                    getline(cin, name);
                    if (graph.updateHospital(id, name)) {
                        cout << "Hospital updated successfully.\n";
                    }
                    break;
                }
                case 3: {
                    string id;
                    cout << "Enter Hospital ID to delete: ";
                    getline(cin, id);
                    if (graph.deleteHospital(id)) {
                        cout << "Hospital deleted successfully.\n";
                    }
                    break;
                }
                case 4: {
                    string from_id, to_id;
                    float distance;
                    cout << "Enter From Hospital ID: ";
                    getline(cin, from_id);
                    cout << "Enter To Hospital ID: ";
                    getline(cin, to_id);
                    cout << "Enter Distance (km): ";
                    cin >> distance;
                    cin.ignore();
                    if (graph.addConnection(from_id, to_id, distance)) {
                        cout << "Connection added successfully.\n";
                    }
                    break;
                }
                case 5:
                    graph.displayNetwork();
                    break;
                case 6: {
                    vector<string> ids = graph.getHospitalIDs();
                    if (ids.empty()) {
                        cout << "No hospitals available.\n";
                        break;
                    }
                    cout << "Available Hospitals:\n";
                    for (const auto& id : ids) {
                        cout << id << "\n";
                    }
                    cout << "Enter Hospital ID to select: ";
                    getline(cin, current_hospital);
                    current_lists = graph.getHospitalLists(current_hospital);
                    if (!current_lists) {
                        cout << "Error: Hospital ID not found.\n";
                        current_hospital.clear();
                    }
                    break;
                }
                case 7:
                    delete current_lists;
                    return 0;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } else {
            switch (choice) {
                case 1: {
                    int id;
                    string name, dob, gender;
                    cout << "Enter Patient ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Name: ";
                    getline(cin, name);
                    cout << "Enter DOB (YYYY-MM-DD): ";
                    getline(cin, dob);
                    cout << "Enter Gender: ";
                    getline(cin, gender);
                    if (current_lists->addPatient(id, name, dob, gender)) {
                        cout << "Patient registered successfully.\n";
                    }
                    break;
                }
                case 2: {
                    int id;
                    string name, spec;
                    cout << "Enter Doctor ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Name: ";
                    getline(cin, name);
                    cout << "Enter Specialization: ";
                    getline(cin, spec);
                    if (current_lists->addDoctor(id, name, spec)) {
                        cout << "Doctor registered successfully.\n";
                    }
                    break;
                }
                case 3: {
                    int app_id, patient_id, doctor_id;
                    string date;
                    cout << "Enter Appointment ID: ";
                    cin >> app_id;
                    cout << "Enter Patient ID: ";
                    cin >> patient_id;
                    cout << "Enter Doctor ID: ";
                    cin >> doctor_id;
                    cin.ignore();
                    cout << "Enter Appointment Date (YYYY-MM-DD): ";
                    getline(cin, date);
                    if (current_lists->addAppointment(app_id, patient_id, doctor_id, date)) {
                        cout << "Appointment registered successfully.\n";
                    }
                    break;
                }
                case 4:
                    current_lists->displayPatients();
                    break;
                case 5:
                    current_lists->displayDoctors();
                    break;
                case 6:
                    current_lists->displayAppointments();
                    break;
                case 7:
                    delete current_lists;
                    current_hospital.clear();
                    current_lists = nullptr;
                    break;
                case 8:
                    delete current_lists;
                    return 0;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        }
    }
}