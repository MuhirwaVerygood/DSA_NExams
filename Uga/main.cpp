#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <set>
#include <algorithm>
#include <cctype>
#include <climits>

using namespace std;

const int MAX = 1000; // Maximum number of health centers
const float INF = numeric_limits<float>::infinity();

// HealthCenter structure
struct HealthCenter {
    int id;
    string name;
    string district;
    float lat;
    float lon;
    int capacity;
};

// Connection structure
struct Connection {
    int to;
    float distance;
    int time;
    string description;
};

// Global variables
vector<HealthCenter> centers;
vector<Connection> adjList[MAX];

// Function prototypes
void readHealthCenters();
void saveHealthCenters();
void readConnections();
void saveConnections();
void addHealthCenter();
void editHealthCenter();
void viewHealthCenters();
void removeHealthCenter();
void addConnection();
void editConnection();
void viewConnections();
void removeConnection();
void viewRelationships();
void dijkstra(int start, int end);
void bfs(int start);
bool detectCycle();
void floydWarshall();
void primMST(int start);
void emergencyRouting(int minCapacity);
bool centerExists(int id);
bool isValidNumber(const string& str);
bool isValidFloat(const string& str);
void trim(string& str);
bool detectCycleDFS(int v, vector<bool>& visited, vector<bool>& recStack, int parent);

// Main menu
int main() {
    readHealthCenters();
    readConnections();
    
    int choice;
    do {
        cout << "\n==== Health Center Network System ====\n";
        cout << "1. Add Health Center\n";
        cout << "2. Edit Health Center\n";
        cout << "3. View Health Centers\n";
        cout << "4. Remove Health Center\n";
        cout << "5. Add Connection\n";
        cout << "6. Edit Connection\n";
        cout << "7. View Connections\n";
        cout << "8. Remove Connection\n";
        cout << "9. View Relationships\n";
        cout << "10. Dijkstra's Shortest Path\n";
        cout << "11. BFS Traversal\n";
        cout << "12. Detect Cycle\n";
        cout << "13. Floyd-Warshall All-Pairs\n";
        cout << "14. Prim's MST\n";
        cout << "15. Emergency Routing\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        
        string input;
        getline(cin, input);
        if (!isValidNumber(input)) {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        choice = stoi(input);
        
        switch (choice) {
            case 1: addHealthCenter(); break;
            case 2: editHealthCenter(); break;
            case 3: viewHealthCenters(); break;
            case 4: removeHealthCenter(); break;
            case 5: addConnection(); break;
            case 6: editConnection(); break;
            case 7: viewConnections(); break;
            case 8: removeConnection(); break;
            case 9: viewRelationships(); break;
            case 10: {
                int start, end;
                cout << "Enter start Health Center ID: ";
                getline(cin, input);
                if (!isValidNumber(input)) { cout << "Invalid ID.\n"; break; }
                start = stoi(input);
                cout << "Enter end Health Center ID: ";
                getline(cin, input);
                if (!isValidNumber(input)) { cout << "Invalid ID.\n"; break; }
                end = stoi(input);
                dijkstra(start, end);
                break;
            }
            case 11: {
                cout << "Enter start Health Center ID: ";
                getline(cin, input);
                if (!isValidNumber(input)) { cout << "Invalid ID.\n"; break; }
                bfs(stoi(input));
                break;
            }
            case 12: detectCycle(); break;
            case 13: floydWarshall(); break;
            case 14: {
                cout << "Enter start Health Center ID: ";
                getline(cin, input);
                if (!isValidNumber(input)) { cout << "Invalid ID.\n"; break; }
                primMST(stoi(input));
                break;
            }
            case 15: {
                cout << "Enter minimum capacity: ";
                getline(cin, input);
                if (!isValidNumber(input)) { cout << "Invalid capacity.\n"; break; }
                emergencyRouting(stoi(input));
                break;
            }
            case 0: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
    
    return 0;
}

// Read health centers from CSV
void readHealthCenters() {
    ifstream file("health_centers.csv");
    if (!file.is_open()) {
        ofstream createFile("health_centers.csv");
        createFile << "ID,Name,District,Latitude,Longitude,Capacity\n";
        createFile.close();
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        try {
            stringstream ss(line);
            HealthCenter hc;
            string id, lat, lon, cap;
            getline(ss, id, ',');
            if (!isValidNumber(id)) continue;
            hc.id = stoi(id);
            getline(ss, hc.name, ',');
            getline(ss, hc.district, ',');
            getline(ss, lat, ',');
            if (!isValidFloat(lat)) continue;
            hc.lat = stof(lat);
            getline(ss, lon, ',');
            if (!isValidFloat(lon)) continue;
            hc.lon = stof(lon);
            getline(ss, cap);
            if (!isValidNumber(cap)) continue;
            hc.capacity = stoi(cap);
            centers.push_back(hc);
        } catch (...) {
            cout << "Error parsing line: " << line << "\n";
        }
    }
    file.close();
}

// Save health centers to CSV
void saveHealthCenters() {
    ofstream file("health_centers.csv");
    file << "ID,Name,District,Latitude,Longitude,Capacity\n";
    for (const auto& hc : centers) {
        file << hc.id << "," << hc.name << "," << hc.district << ","
             << fixed << setprecision(4) << hc.lat << "," << hc.lon << "," << hc.capacity << "\n";
    }
    file.close();
}

// Read connections from CSV
void readConnections() {
    ifstream file("connections.csv");
    if (!file.is_open()) {
        ofstream createFile("connections.csv");
        createFile << "FromID,ToID,DistanceKM,TimeMinutes,Description\n";
        createFile.close();
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        try {
            stringstream ss(line);
            Connection c;
            string from, to, dist, time;
            getline(ss, from, ',');
            if (!isValidNumber(from)) continue;
            int fromID = stoi(from);
            getline(ss, to, ',');
            if (!isValidNumber(to)) continue;
            c.to = stoi(to);
            getline(ss, dist, ',');
            if (!isValidFloat(dist)) continue;
            c.distance = stof(dist);
            getline(ss, time, ',');
            if (!isValidNumber(time)) continue;
            c.time = stoi(time);
            getline(ss, c.description);
            trim(c.description);
            adjList[fromID].push_back(c);
            // Add reverse connection for undirected graph
            Connection reverse = {fromID, c.distance, c.time, c.description};
            adjList[c.to].push_back(reverse);
        } catch (...) {
            cout << "Error parsing line: " << line << "\n";
        }
    }
    file.close();
}

// Save connections to CSV
void saveConnections() {
    ofstream file("connections.csv");
    file << "FromID,ToID,DistanceKM,TimeMinutes,Description\n";
    set<pair<int, int>> added; // To avoid duplicate edges
    for (int i = 0; i < MAX; ++i) {
        for (const auto& c : adjList[i]) {
            pair<int, int> edge = {min(i, c.to), max(i, c.to)};
            if (added.find(edge) == added.end()) {
                file << i << "," << c.to << "," << fixed << setprecision(2) << c.distance
                     << "," << c.time << "," << c.description << "\n";
                added.insert(edge);
            }
        }
    }
    file.close();
}

// Add a new health center
void addHealthCenter() {
    HealthCenter hc;
    string input;
    
    cout << "Enter Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    hc.id = stoi(input);
    
    if (centerExists(hc.id)) {
        cout << "Error: Health Center ID already exists.\n";
        return;
    }
    
    cout << "Enter Name: ";
    getline(cin, hc.name);
    cout << "Enter District: ";
    getline(cin, hc.district);
    cout << "Enter Latitude: ";
    getline(cin, input);
    if (!isValidFloat(input)) {
        cout << "Error: Latitude must be a valid number.\n";
        return;
    }
    hc.lat = stof(input);
    cout << "Enter Longitude: ";
    getline(cin, input);
    if (!isValidFloat(input)) {
        cout << "Error: Longitude must be a valid number.\n";
        return;
    }
    hc.lon = stof(input);
    cout << "Enter Capacity: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: Capacity must be a valid number.\n";
        return;
    }
    hc.capacity = stoi(input);
    
    centers.push_back(hc);
    saveHealthCenters();
    cout << "Health Center added successfully.\n";
}

// Edit an existing health center
void editHealthCenter() {
    string input;
    cout << "Enter Health Center ID to edit: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int id = stoi(input);
    
    auto it = find_if(centers.begin(), centers.end(), [id](const HealthCenter& hc) { return hc.id == id; });
    if (it == centers.end()) {
        cout << "Error: Health Center ID not found.\n";
        return;
    }
    
    cout << "Enter new Name (current: " << it->name << "): ";
    getline(cin, it->name);
    cout << "Enter new District (current: " << it->district << "): ";
    getline(cin, it->district);
    cout << "Enter new Latitude (current: " << it->lat << "): ";
    getline(cin, input);
    if (!isValidFloat(input)) {
        cout << "Error: Latitude must be a valid number.\n";
        return;
    }
    it->lat = stof(input);
    cout << "Enter new Longitude (current: " << it->lon << "): ";
    getline(cin, input);
    if (!isValidFloat(input)) {
        cout << "Error: Longitude must be a valid number.\n";
        return;
    }
    it->lon = stof(input);
    cout << "Enter new Capacity (current: " << it->capacity << "): ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: Capacity must be a valid number.\n";
        return;
    }
    it->capacity = stoi(input);
    
    saveHealthCenters();
    cout << "Health Center updated successfully.\n";
}

// Display all health centers
void viewHealthCenters() {
    if (centers.empty()) {
        cout << "No health centers found.\n";
        return;
    }
    
    cout << "\n" << left
         << setw(10) << "ID"
         << setw(25) << "Name"
         << setw(20) << "District"
         << setw(12) << "Latitude"
         << setw(12) << "Longitude"
         << setw(10) << "Capacity" << "\n";
    cout << string(89, '-') << "\n";
    
    for (const auto& hc : centers) {
        cout << left
             << setw(10) << hc.id
             << setw(25) << hc.name
             << setw(20) << hc.district
             << setw(12) << fixed << setprecision(4) << hc.lat
             << setw(12) << hc.lon
             << setw(10) << hc.capacity << "\n";
    }
}

// Remove a health center
void removeHealthCenter() {
    string input;
    cout << "Enter Health Center ID to remove: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int id = stoi(input);
    
    auto it = find_if(centers.begin(), centers.end(), [id](const HealthCenter& hc) { return hc.id == id; });
    if (it == centers.end()) {
        cout << "Error: Health Center ID not found.\n";
        return;
    }
    
    centers.erase(it);
    adjList[id].clear();
    for (int i = 0; i < MAX; ++i) {
        adjList[i].erase(
            remove_if(adjList[i].begin(), adjList[i].end(), [id](const Connection& c) { return c.to == id; }),
            adjList[i].end()
        );
    }
    
    saveHealthCenters();
    saveConnections();
    cout << "Health Center and related connections removed successfully.\n";
}

// Add a new connection
void addConnection() {
    Connection c;
    string input;
    
    cout << "Enter From Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int fromID = stoi(input);
    
    cout << "Enter To Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    c.to = stoi(input);
    
    if (!centerExists(fromID) || !centerExists(c.to)) {
        cout << "Error: One or both Health Center IDs not found.\n";
        return;
    }
    
    if (fromID == c.to) {
        cout << "Error: Cannot connect a health center to itself.\n";
        return;
    }
    
    // Check if connection exists
    for (const auto& conn : adjList[fromID]) {
        if (conn.to == c.to) {
            cout << "Error: Connection already exists.\n";
            return;
        }
    }
    
    cout << "Enter Distance (km): ";
    getline(cin, input);
    if (!isValidFloat(input)) {
        cout << "Error: Distance must be a valid number.\n";
        return;
    }
    c.distance = stof(input);
    
    cout << "Enter Time (minutes): ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: Time must be a valid number.\n";
        return;
    }
    c.time = stoi(input);
    
    cout << "Enter Description: ";
    getline(cin, c.description);
    trim(c.description);
    
    adjList[fromID].push_back(c);
    Connection reverse = {fromID, c.distance, c.time, c.description};
    adjList[c.to].push_back(reverse);
    
    saveConnections();
    cout << "Connection added successfully.\n";
}

// Edit an existing connection
void editConnection() {
    string input;
    cout << "Enter From Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int fromID = stoi(input);
    
    cout << "Enter To Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int toID = stoi(input);
    
    if (!centerExists(fromID) || !centerExists(toID)) {
        cout << "Error: One or both Health Center IDs not found.\n";
        return;
    }
    
    auto it = find_if(adjList[fromID].begin(), adjList[fromID].end(),
        [toID](const Connection& c) { return c.to == toID; });
    if (it == adjList[fromID].end()) {
        cout << "Error: Connection not found.\n";
        return;
    }
    
    cout << "Enter new Distance (km, current: " << it->distance << "): ";
    getline(cin, input);
    if (!isValidFloat(input)) {
        cout << "Error: Distance must be a valid number.\n";
        return;
    }
    it->distance = stof(input);
    
    cout << "Enter new Time (minutes, current: " << it->time << "): ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: Time must be a valid number.\n";
        return;
    }
    it->time = stoi(input);
    
    cout << "Enter new Description (current: " << it->description << "): ";
    getline(cin, it->description);
    trim(it->description);
    
    // Update reverse connection
    auto reverseIt = find_if(adjList[toID].begin(), adjList[toID].end(),
        [fromID](const Connection& c) { return c.to == fromID; });
    if (reverseIt != adjList[toID].end()) {
        reverseIt->distance = it->distance;
        reverseIt->time = it->time;
        reverseIt->description = it->description;
    }
    
    saveConnections();
    cout << "Connection updated successfully.\n";
}

// Display all connections
void viewConnections() {
    bool hasConnections = false;
    cout << "\n" << left
         << setw(10) << "FromID"
         << setw(10) << "ToID"
         << setw(15) << "Distance(km)"
         << setw(15) << "Time(min)"
         << setw(20) << "Description" << "\n";
    cout << string(70, '-') << "\n";
    
    set<pair<int, int>> added;
    for (int i = 0; i < MAX; ++i) {
        for (const auto& c : adjList[i]) {
            pair<int, int> edge = {min(i, c.to), max(i, c.to)};
            if (added.find(edge) == added.end()) {
                cout << left
                     << setw(10) << i
                     << setw(10) << c.to
                     << setw(15) << fixed << setprecision(2) << c.distance
                     << setw(15) << c.time
                     << setw(20) << c.description << "\n";
                added.insert(edge);
                hasConnections = true;
            }
        }
    }
    
    if (!hasConnections) {
        cout << "No connections found.\n";
    }
}

// Remove a connection
void removeConnection() {
    string input;
    cout << "Enter From Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int fromID = stoi(input);
    
    cout << "Enter To Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    int toID = stoi(input);
    
    if (!centerExists(fromID) || !centerExists(toID)) {
        cout << "Error: One or both Health Center IDs not found.\n";
        return;
    }
    
    auto it = find_if(adjList[fromID].begin(), adjList[fromID].end(),
        [toID](const Connection& c) { return c.to == toID; });
    if (it == adjList[fromID].end()) {
        cout << "Error: Connection not found.\n";
        return;
    }
    
    adjList[fromID].erase(it);
    
    auto reverseIt = find_if(adjList[toID].begin(), adjList[toID].end(),
        [fromID](const Connection& c) { return c.to == fromID; });
    if (reverseIt != adjList[toID].end()) {
        adjList[toID].erase(reverseIt);
    }
    
    saveConnections();
    cout << "Connection removed successfully.\n";
}

// Display relationships table and save to CSV
void viewRelationships() {
    ofstream file("relationship_table.csv");
    file << "Health Center ID,Name,Connected Centers,Descriptions\n";
    
    cout << "\n" << left
         << setw(15) << "ID"
         << setw(25) << "Name"
         << setw(25) << "Connected Centers"
         << setw(30) << "Descriptions" << "\n";
    cout << string(95, '-') << "\n";
    
    for (const auto& hc : centers) {
        string connected, descriptions;
        for (const auto& c : adjList[hc.id]) {
            auto it = find_if(centers.begin(), centers.end(),
                [c](const HealthCenter& center) { return center.id == c.to; });
            if (it != centers.end()) {
                connected += to_string(c.to) + ";";
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
        
        cout << left
             << setw(15) << hc.id
             << setw(25) << hc.name
             << setw(25) << connected
             << setw(30) << descriptions << "\n";
        
        file << hc.id << "," << hc.name << "," << connected << "," << descriptions << "\n";
    }
    
    file.close();
    cout << "Relationships exported to relationship_table.csv successfully.\n";
}

// Dijkstra's algorithm for shortest path
void dijkstra(int start, int end) {
    if (!centerExists(start) || !centerExists(end)) {
        cout << "Error: One or both Health Center IDs not found.\n";
        return;
    }
    
    vector<float> dist(MAX, INF);
    vector<int> prev(MAX, -1);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        float d = pq.top().first;
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (const auto& c : adjList[u]) {
            if (dist[u] + c.distance < dist[c.to]) {
                dist[c.to] = dist[u] + c.distance;
                prev[c.to] = u;
                pq.push({dist[c.to], c.to});
            }
        }
    }
    
    if (dist[end] == INF) {
        cout << "No path exists between " << start << " and " << end << ".\n";
        return;
    }
    
    cout << "Shortest distance from " << start << " to " << end << ": " << fixed << setprecision(2) << dist[end] << " km\n";
    cout << "Path: ";
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\n";
}

// BFS traversal
void bfs(int start) {
    if (!centerExists(start)) {
        cout << "Error: Health Center ID not found.\n";
        return;
    }
    
    vector<bool> visited(MAX, false);
    queue<int> q;
    
    visited[start] = true;
    q.push(start);
    
    cout << "BFS Traversal starting from " << start << ": ";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        cout << u << " ";
        
        for (const auto& c : adjList[u]) {
            if (!visited[c.to]) {
                visited[c.to] = true;
                q.push(c.to);
            }
        }
    }
    cout << "\n";
}

// Cycle detection using DFS
bool detectCycle() {
    vector<bool> visited(MAX, false);
    vector<bool> recStack(MAX, false);
    
    for (const auto& hc : centers) {
        if (!visited[hc.id]) {
            if (detectCycleDFS(hc.id, visited, recStack, -1)) {
                return true;
            }
        }
    }
    return false;
}

bool detectCycleDFS(int v, vector<bool>& visited, vector<bool>& recStack, int parent) {
    visited[v] = true;
    recStack[v] = true;
    
    for (const auto& c : adjList[v]) {
        if (!visited[c.to]) {
            if (detectCycleDFS(c.to, visited, recStack, v)) {
                return true;
            }
        } else if (recStack[c.to] && c.to != parent) {
            return true;
        }
    }
    
    recStack[v] = false;
    return false;
}

// Floyd-Warshall algorithm
void floydWarshall() {
    int maxID = 0;
    for (const auto& hc : centers) {
        maxID = max(maxID, hc.id);
    }
    
    vector<vector<float>> dist(maxID + 1, vector<float>(maxID + 1, INF));
    vector<vector<int>> next(maxID + 1, vector<int>(maxID + 1, -1));
    
    for (int i = 0; i <= maxID; ++i) {
        dist[i][i] = 0;
    }
    
    for (int u = 0; u <= maxID; ++u) {
        for (const auto& c : adjList[u]) {
            dist[u][c.to] = c.distance;
            next[u][c.to] = c.to;
        }
    }
    
    for (int k = 0; k <= maxID; ++k) {
        for (int i = 0; i <= maxID; ++i) {
            for (int j = 0; j <= maxID; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    cout << "\nAll-Pairs Shortest Paths (distances in km):\n";
    cout << left << setw(10) << "From\\To";
    for (const auto& hc : centers) {
        cout << setw(10) << hc.id;
    }
    cout << "\n" << string(10 + centers.size() * 10, '-') << "\n";
    
    for (const auto& hc1 : centers) {
        cout << left << setw(10) << hc1.id;
        for (const auto& hc2 : centers) {
            if (dist[hc1.id][hc2.id] == INF) {
                cout << setw(10) << "INF";
            } else {
                cout << setw(10) << fixed << setprecision(2) << dist[hc1.id][hc2.id];
            }
        }
        cout << "\n";
    }
}

// Prim's algorithm for MST
void primMST(int start) {
    if (!centerExists(start)) {
        cout << "Error: Health Center ID not found.\n";
        return;
    }
    
    vector<float> key(MAX, INF);
    vector<int> parent(MAX, -1);
    vector<bool> inMST(MAX, false);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> pq;
    
    key[start] = 0;
    pq.push({0, start});
    
    float totalWeight = 0;
    vector<pair<int, int>> edges;
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (inMST[u]) continue;
        inMST[u] = true;
        
        if (parent[u] != -1) {
            totalWeight += key[u];
            edges.emplace_back(parent[u], u);
        }
        
        for (const auto& c : adjList[u]) {
            if (!inMST[c.to] && c.distance < key[c.to]) {
                key[c.to] = c.distance;
                parent[c.to] = u;
                pq.push({key[c.to], c.to});
            }
        }
    }
    
    cout << "Minimum Spanning Tree Edges:\n";
    for (const auto& edge : edges) {
        cout << edge.first << " - " << edge.second << "\n";
    }
    cout << "Total MST weight: " << fixed << setprecision(2) << totalWeight << " km\n";
}

// Emergency routing
void emergencyRouting(int minCapacity) {
    if (centers.empty()) {
        cout << "No health centers available.\n";
        return;
    }
    
    int start;
    string input;
    cout << "Enter starting Health Center ID: ";
    getline(cin, input);
    if (!isValidNumber(input)) {
        cout << "Error: ID must be a number.\n";
        return;
    }
    start = stoi(input);
    
    if (!centerExists(start)) {
        cout << "Error: Starting Health Center ID not found.\n";
        return;
    }
    
    vector<float> dist(MAX, INF);
    vector<int> prev(MAX, -1);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    int nearestID = -1;
    float minDist = INF;
    
    while (!pq.empty()) {
        int u = pq.top().second;
        float d = pq.top().first;
        pq.pop();
        
        if (d > dist[u]) continue;
        
        auto it = find_if(centers.begin(), centers.end(), [u](const HealthCenter& hc) { return hc.id == u; });
        if (it != centers.end() && it->capacity >= minCapacity && u != start && d < minDist) {
            nearestID = u;
            minDist = d;
        }
        
        for (const auto& c : adjList[u]) {
            if (dist[u] + c.distance < dist[c.to]) {
                dist[c.to] = dist[u] + c.distance;
                prev[c.to] = u;
                pq.push({dist[c.to], c.to});
            }
        }
    }
    
    if (nearestID == -1) {
        cout << "No health center found with capacity >= " << minCapacity << ".\n";
        return;
    }
    
    cout << "Nearest Health Center with capacity >= " << minCapacity << ": ID " << nearestID
         << ", Distance: " << fixed << setprecision(2) << minDist << " km\n";
    cout << "Path: ";
    vector<int> path;
    for (int at = nearestID; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\n";
}

// Utility functions
bool centerExists(int id) {
    return find_if(centers.begin(), centers.end(), [id](const HealthCenter& hc) { return hc.id == id; }) != centers.end();
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

bool isValidFloat(const string& str) {
    try {
        size_t pos;
        stof(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

void trim(string& str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char c) { return !isspace(c); }));
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char c) { return !isspace(c); }).base(), str.end());
}

