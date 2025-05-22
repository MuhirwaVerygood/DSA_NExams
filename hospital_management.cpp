#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// Struct to represent a hospital (node in the graph)
struct Hospital {
    string id; // Unique identifier (e.g., H001)
    string name;
    string location;
    int beds;
    string specialties; // Comma-separated list of specialties
};

// Class to manage the hospital graph and CSV operations
class HospitalGraph {
private:
    // Adjacency list: maps hospital ID to list of (neighbor ID, distance)
    map<string, vector<pair<string, double > > > adjList;
    // Maps hospital ID to Hospital details
    map<string, Hospital> hospitals;
    const string csvFile = "hospitals.csv";

    // Helper function to split a string by delimiter
    vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        string token;
        stringstream ss(str);
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Load hospitals from CSV file
    void loadFromCSV() {
        ifstream file(csvFile);
        if (!file.is_open()) {
            cout << "No existing CSV file found. Starting fresh.\n";
            return;
        }

        string line;
        // Skip header if exists
        getline(file, line);
        while (getline(file, line)) {
            vector<string> tokens = split(line, ',');
            if (tokens.size() >= 5) {
                Hospital h;
                h.id = tokens[0];
                h.name = tokens[1];
                h.location = tokens[2];
                try {
                    h.beds = stoi(tokens[3]);
                } catch (...) {
                    h.beds = 0;
                }
                h.specialties = tokens[4];
                hospitals[h.id] = h;
            }
        }
        file.close();
        cout << "Loaded " << hospitals.size() << " hospitals from CSV.\n";
    }

    // Save hospitals to CSV file
    void saveToCSV() {
        ofstream file(csvFile);
        if (!file.is_open()) {
            cout << "Error: Unable to open CSV file for writing.\n";
            return;
        }

        // Write header
        file << "ID,Name,Location,Beds,Specialties\n";
        for (const auto& pair : hospitals) {
            const Hospital& h = pair.second;
            file << h.id << "," << h.name << "," << h.location << ","
                 << h.beds << "," << h.specialties << "\n";
        }
        file.close();
        cout << "Saved " << hospitals.size() << " hospitals to CSV.\n";
    }

public:
    HospitalGraph() {
        loadFromCSV();
    }

    // Add a new hospital
    void addHospital(const string& id, const string& name, const string& location, int beds, const string& specialties) {
        if (hospitals.find(id) != hospitals.end()) {
            cout << "Error: Hospital ID " << id << " already exists.\n";
            return;
        }
        Hospital h = {id, name, location, beds, specialties};
        hospitals[id] = h;
        adjList[id] = vector<pair<string, double>>(); // Initialize adjacency list for new hospital
        saveToCSV();
        cout << "Hospital " << name << " added successfully.\n";
    }

    // Update an existing hospital
    void updateHospital(const string& id, const string& name, const string& location, int beds, const string& specialties) {
        if (hospitals.find(id) == hospitals.end()) {
            cout << "Error: Hospital ID " << id << " not found.\n";
            return;
        }
        Hospital& h = hospitals[id];
        h.name = name;
        h.location = location;
        h.beds = beds;
        h.specialties = specialties;
        saveToCSV();
        cout << "Hospital ID " << id << " updated successfully.\n";
    }

    // Delete a hospital
    void deleteHospital(const string& id) {
        if (hospitals.find(id) == hospitals.end()) {
            cout << "Error: Hospital ID " << id << " not found.\n";
            return;
        }
        // Remove hospital from hospitals map
        hospitals.erase(id);
        // Remove hospital from adjacency list and its edges
        adjList.erase(id);
        for (auto& pair : adjList) {
            auto& neighbors = pair.second;
            neighbors.erase(
                remove_if(neighbors.begin(), neighbors.end(),
                    [&id](const std::pair<std::string, double>& edge) { return edge.first == id; }),
                neighbors.end()
            );
        }
        saveToCSV();
        cout << "Hospital ID " << id << " deleted successfully.\n";
    }

    // Read/display a hospital's details
    void displayHospital(const string& id) {
        if (hospitals.find(id) == hospitals.end()) {
            cout << "Error: Hospital ID " << id << " not found.\n";
            return;
        }
        const Hospital& h = hospitals[id];
        cout << "\nHospital Details:\n";
        cout << "ID: " << h.id << "\n";
        cout << "Name: " << h.name << "\n";
        cout << "Location: " << h.location << "\n";
        cout << "Beds: " << h.beds << "\n";
        cout << "Specialties: " << h.specialties << "\n";
        cout << "Connected Hospitals (ID, Distance):\n";
        for (const auto& edge : adjList[id]) {
            cout << "  -> " << edge.first << " (" << edge.second << " km)\n";
        }
    }

    // Add an edge (distance) between two hospitals
    void addEdge(const string& id1, const string& id2, double distance) {
        if (hospitals.find(id1) == hospitals.end() || hospitals.find(id2) == hospitals.end()) {
            cout << "Error: One or both hospital IDs not found.\n";
            return;
        }
        if (id1 == id2) {
            cout << "Error: Cannot add edge to the same hospital.\n";
            return;
        }
        // Add bidirectional edge
        adjList[id1].push_back({id2, distance});
        adjList[id2].push_back({id1, distance});
        cout << "Edge added between " << id1 << " and " << id2 << " with distance " << distance << " km.\n";
    }

    // Display all hospitals
    void displayAllHospitals() {
        if (hospitals.empty()) {
            cout << "No hospitals in the system.\n";
            return;
        }
        cout << "\nAll Hospitals:\n";
        for (const auto& pair : hospitals) {
            displayHospital(pair.first);
        }
    }
};

// Main function with interactive menu
int main() {
    HospitalGraph graph;
    int choice;
    string id, name, location, specialties, id2;
    int beds;
    double distance;

    while (true) {
        cout << "\nHospital Management System\n";
        cout << "1. Add Hospital\n";
        cout << "2. Update Hospital\n";
        cout << "3. Delete Hospital\n";
        cout << "4. Display Hospital\n";
        cout << "5. Display All Hospitals\n";
        cout << "6. Add Connection Between Hospitals\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from input buffer

        switch (choice) {
            case 1: // Add Hospital
                cout << "Enter Hospital ID: ";
                getline(cin, id);
                cout << "Enter Hospital Name: ";
                getline(cin, name);
                cout << "Enter Location: ";
                getline(cin, location);
                cout << "Enter Number of Beds: ";
                cin >> beds;
                cin.ignore();
                cout << "Enter Specialties (comma-separated): ";
                getline(cin, specialties);
                graph.addHospital(id, name, location, beds, specialties);
                break;

            case 2: // Update Hospital
                cout << "Enter Hospital ID to update: ";
                getline(cin, id);
                cout << "Enter New Hospital Name: ";
                getline(cin, name);
                cout << "Enter New Location: ";
                getline(cin, location);
                cout << "Enter New Number of Beds: ";
                cin >> beds;
                cin.ignore();
                cout << "Enter New Specialties (comma-separated): ";
                getline(cin, specialties);
                graph.updateHospital(id, name, location, beds, specialties);
                break;

            case 3: // Delete Hospital
                cout << "Enter Hospital ID to delete: ";
                getline(cin, id);
                graph.deleteHospital(id);
                break;

            case 4: // Display Hospital
                cout << "Enter Hospital ID to display: ";
                getline(cin, id);
                graph.displayHospital(id);
                break;

            case 5: // Display All Hospitals
                graph.displayAllHospitals();
                break;

            case 6: // Add Connection
                cout << "Enter First Hospital ID: ";
                getline(cin, id);
                cout << "Enter Second Hospital ID: ";
                getline(cin, id2);
                cout << "Enter Distance (km): ";
                cin >> distance;
                cin.ignore();
                graph.addEdge(id, id2, distance);
                break;

            case 7: // Exit
                cout << "Exiting program.\n";
                return 0;

            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}
