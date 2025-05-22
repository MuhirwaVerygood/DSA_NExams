#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

// Hospital structure to store hospital details
struct Hospital {
    string id;
    string name;
    string location;
    int numPatients;
};

// Structure to store connection details
struct Connection {
    string hospital1;
    string hospital2;
    string description;
};

// Function prototypes
void loadHospitals(vector<Hospital>& hospitals);
void saveHospitals(const vector<Hospital>& hospitals);
void loadConnections(vector<Connection>& connections);
void saveConnections(const vector<Connection>& connections);
void addHospital(vector<Hospital>& hospitals);
void displayHospitals(const vector<Hospital>& hospitals);
void updateHospital(vector<Hospital>& hospitals);
void deleteHospital(vector<Hospital>& hospitals, vector<Connection>& connections);
void addConnection(vector<Hospital>& hospitals, vector<Connection>& connections);
void displayConnections(const vector<Hospital>& hospitals, const vector<Connection>& connections);
void exportRelationships(const vector<Hospital>& hospitals, const vector<Connection>& connections);
bool isValidNumber(const string& str);
bool hospitalExists(const vector<Hospital>& hospitals, const string& id);
void trim(string& str);

int main() {
    vector<Hospital> hospitals;
    vector<Connection> connections;
    
    // Load initial data
    loadHospitals(hospitals);
    loadConnections(connections);
    
    int choice;
    do {
        cout << "\n=== Hospital Network Management System ===\n";
        cout << "1. Add Hospital\n";
        cout << "2. Display All Hospitals\n";
        cout << "3. Update Hospital\n";
        cout << "4. Delete Hospital\n";
        cout << "5. Add Connection\n";
        cout << "6. Display Relationships\n";
        cout << "7. Export Relationships to CSV\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addHospital(hospitals);
                break;
            case 2:
                displayHospitals(hospitals);
                break;
            case 3:
                updateHospital(hospitals);
                break;
            case 4:
                deleteHospital(hospitals, connections);
                break;
            case 5:
                addConnection(hospitals, connections);
                break;
            case 6:
                displayConnections(hospitals, connections);
                break;
            case 7:
                exportRelationships(hospitals, connections);
                break;
            case 8:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);
    
    return 0;
}

void loadHospitals(vector<Hospital>& hospitals) {
    ifstream file("hospitals_castella.csv");
    if (!file.is_open()) return;
    
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        Hospital h;
        string numPatients;
        getline(ss, h.id, ',');
        getline(ss, h.name, ',');
        getline(ss, h.location, ',');
        getline(ss, numPatients);
        if (isValidNumber(numPatients)) {
            h.numPatients = stoi(numPatients);
            hospitals.push_back(h);
        }
    }
    file.close();
}

void saveHospitals(const vector<Hospital>& hospitals) {
    ofstream file("hospitals_castella.csv");
    file << "ID,Name,Location,Number of Patients\n";
    for (const auto& h : hospitals) {
        file << h.id << "," << h.name << "," << h.location << "," << h.numPatients << "\n";
    }
    file.close();
}

void loadConnections(vector<Connection>& connections) {
    ifstream file("graph.txt");
    if (!file.is_open()) return;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string hospital1, rest;
        getline(ss, hospital1, ',');
        while (getline(ss, rest)) {
            size_t colonPos = rest.find(':');
            if (colonPos != string::npos) {
                Connection c;
                c.hospital1 = hospital1;
                c.hospital2 = rest.substr(0, colonPos);
                c.description = rest.substr(colonPos + 1);
                trim(c.hospital1);
                trim(c.hospital2);
                trim(c.description);
                connections.push_back(c);
            }
        }
    }
    file.close();
}

void saveConnections(const vector<Connection>& connections) {
    map<string, vector<pair<string, string>>> grouped;
    for (const auto& c : connections) {
        grouped[c.hospital1].emplace_back(c.hospital2, c.description);
        grouped[c.hospital2].emplace_back(c.hospital1, c.description); // Bidirectional
    }
    
    ofstream file("graph.txt");
    for (const auto& pair : grouped) {
        file << pair.first;
        for (const auto& conn : pair.second) {
            file << "," << conn.first << ":" << conn.second;
        }
        file << "\n";
    }
    file.close();
}

void addHospital(vector<Hospital>& hospitals) {
    Hospital h;
    cout << "Enter Hospital ID (e.g., H1): ";
    getline(cin, h.id);
    trim(h.id);
    
    if (hospitalExists(hospitals, h.id)) {
        cout << "Error: Hospital ID already exists.\n";
        return;
    }
    
    cout << "Enter Hospital Name: ";
    getline(cin, h.name);
    cout << "Enter Location: ";
    getline(cin, h.location);
    string numPatients;
    cout << "Enter Number of Patients: ";
    getline(cin, numPatients);
    
    if (!isValidNumber(numPatients)) {
        cout << "Error: Number of patients must be a valid number.\n";
        return;
    }
    
    h.numPatients = stoi(numPatients);
    hospitals.push_back(h);
    saveHospitals(hospitals);
    cout << "Hospital added successfully.\n";
}

void displayHospitals(const vector<Hospital>& hospitals) {
    if (hospitals.empty()) {
        cout << "No hospitals found.\n";
        return;
    }
    
    cout << "\n" << left
         << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(20) << "Location"
         << setw(15) << "Patients" << "\n";
    cout << string(65, '-') << "\n";
    
    for (const auto& h : hospitals) {
        cout << left
             << setw(10) << h.id
             << setw(20) << h.name
             << setw(20) << h.location
             << setw(15) << h.numPatients << "\n";
    }
}

void updateHospital(vector<Hospital>& hospitals) {
    string id;
    cout << "Enter Hospital ID to update: ";
    getline(cin, id);
    trim(id);
    
    auto it = find_if(hospitals.begin(), hospitals.end(),
        [&id](const Hospital& h) { return h.id == id; });
    
    if (it == hospitals.end()) {
        cout << "Error: Hospital ID not found.\n";
        return;
    }
    
    cout << "Enter new Hospital Name (current: " << it->name << "): ";
    getline(cin, it->name);
    cout << "Enter new Location (current: " << it->location << "): ";
    getline(cin, it->location);
    string numPatients;
    cout << "Enter new Number of Patients (current: " << it->numPatients << "): ";
    getline(cin, numPatients);
    
    if (!isValidNumber(numPatients)) {
        cout << "Error: Number of patients must be a valid number.\n";
        return;
    }
    
    it->numPatients = stoi(numPatients);
    saveHospitals(hospitals);
    cout << "Hospital updated successfully.\n";
}

void deleteHospital(vector<Hospital>& hospitals, vector<Connection>& connections) {
    string id;
    cout << "Enter Hospital ID to delete: ";
    getline(cin, id);
    trim(id);
    
    auto it = find_if(hospitals.begin(), hospitals.end(),
        [&id](const Hospital& h) { return h.id == id; });
    
    if (it == hospitals.end()) {
        cout << "Error: Hospital ID not found.\n";
        return;
    }
    
    hospitals.erase(it);
    
    // Remove related connections
    connections.erase(
        remove_if(connections.begin(), connections.end(),
            [&id](const Connection& c) { return c.hospital1 == id || c.hospital2 == id; }),
        connections.end()
    );
    
    saveHospitals(hospitals);
    saveConnections(connections);
    cout << "Hospital and related connections deleted successfully.\n";
}

void addConnection(vector<Hospital>& hospitals, vector<Connection>& connections) {
    Connection c;
    cout << "Enter first Hospital ID: ";
    getline(cin, c.hospital1);
    trim(c.hospital1);
    cout << "Enter second Hospital ID: ";
    getline(cin, c.hospital2);
    trim(c.hospital2);
    
    if (!hospitalExists(hospitals, c.hospital1) || !hospitalExists(hospitals, c.hospital2)) {
        cout << "Error: One or both Hospital IDs not found.\n";
        return;
    }
    
    if (c.hospital1 == c.hospital2) {
        cout << "Error: Cannot connect a hospital to itself.\n";
        return;
    }
    
    // Check if connection already exists
    for (const auto& conn : connections) {
        if ((conn.hospital1 == c.hospital1 && conn.hospital2 == c.hospital2) ||
            (conn.hospital1 == c.hospital2 && conn.hospital2 == c.hospital1)) {
            cout << "Error: Connection already exists.\n";
            return;
        }
    }
    
    cout << "Enter connection description: ";
    getline(cin, c.description);
    trim(c.description);
    
    connections.push_back(c);
    saveConnections(connections);
    cout << "Connection added successfully.\n";
}

void displayConnections(const vector<Hospital>& hospitals, const vector<Connection>& connections) {
    cout << "\n" << left
         << setw(20) << "Hospital Center"
         << setw(25) << "Connected Hospitals"
         << setw(30) << "Description" << "\n";
    cout << string(75, '-') << "\n";
    
    for (const auto& h : hospitals) {
        string connected, descriptions;
        for (const auto& c : connections) {
            if (c.hospital1 == h.id) {
                connected += c.hospital2 + ", ";
                descriptions += c.description + ", ";
            } else if (c.hospital2 == h.id) {
                connected += c.hospital1 + ", ";
                descriptions += c.description + ", ";
            }
        }
        
        if (!connected.empty()) {
            connected = connected.substr(0, connected.length() - 2);
            descriptions = descriptions.substr(0, descriptions.length() - 2);
        } else {
            connected = "None";
            descriptions = "None";
        }
        
        cout << left
             << setw(20) << h.id
             << setw(25) << connected
             << setw(30) << descriptions << "\n";
    }
}

void exportRelationships(const vector<Hospital>& hospitals, const vector<Connection>& connections) {
    ofstream file("relationships.csv");
    file << "Hospital Center,Connected Hospitals,Description\n";
    
    for (const auto& h : hospitals) {
        string connected, descriptions;
        for (const auto& c : connections) {
            if (c.hospital1 == h.id) {
                connected += c.hospital2 + ";";
                descriptions += c.description + ";";
            } else if (c.hospital2 == h.id) {
                connected += c.hospital1 + ";";
                descriptions += c.description + ";";
            }
        }
        
        if (!connected.empty()) {
            connected = connected.substr(0, connected.length() - 1);
            descriptions = descriptions.substr(0, descriptions.length() - 1);
        } else {
            connected = "None";
            descriptions = "None";
        }
        
        file << h.id << "," << connected << "," << descriptions << "\n";
    }
    
    file.close();
    cout << "Relationships exported to relationships.csv successfully.\n";
}

bool isValidNumber(const string& str) {
    try {
        size_t pos;
        stoi(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

bool hospitalExists(const vector<Hospital>& hospitals, const string& id) {
    return find_if(hospitals.begin(), hospitals.end(),
        [&id](const Hospital& h) { return h.id == id; }) != hospitals.end();
}

void trim(string& str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char c) { return !isspace(c); }));
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char c) { return !isspace(c); }).base(), str.end());
}