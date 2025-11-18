#include <bits/stdc++.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

const string COLOR_CYAN = "\033[38;5;39m";
const string COLOR_GREEN = "\033[38;5;46m";
const string COLOR_YELLOW = "\033[38;5;226m";
const string COLOR_RED = "\033[38;5;196m";
const string COLOR_RESET = "\033[0m";

void pause(int ms);
void enableVTMode();
int getConsoleWidth();
void printCentered(const string& s);
void animation(const string& s, const string& color, int delay, bool showBorder = true);

enum class RideStatus {
	PENDING,
	ONGOING,
	COMPLETED,
	CANCELLED
};

string rideStatusToString(RideStatus status) {
	switch (status) {
		case RideStatus::PENDING:
			return "Pending";
		case RideStatus::ONGOING:
			return "Ongoing";
		case RideStatus::COMPLETED:
			return "Completed";
		case RideStatus::CANCELLED:
			return "Cancelled";
		default:
			return "Unknown";
	}
}

class Review {
	private:
		string reviewer;
		string reviewee;
		int rating;
		string comment;
		string timestamp;

	public:
		Review(string reviewerUser, string revieweeUser, int rate, string commentText = "")
			: reviewer(reviewerUser), reviewee(revieweeUser), rating(rate), comment(commentText) {
			time_t now = time(0);
			timestamp = ctime(&now);
			timestamp = timestamp.substr(0, timestamp.length()-1);
		}

		string getReviewer() const {
			return reviewer;
		}
		string getReviewee() const {
			return reviewee;
		}
		int getRating() const {
			return rating;
		}
		string getComment() const {
			return comment;
		}
		string getTimestamp() const {
			return timestamp;
		}

		void displayReview() const {
			cout << "----------------------------------------\n";
			cout << "| From: " << setw(31) << left << reviewer << "|\n";
			cout << "| To: " << setw(33) << left << reviewee << "|\n";
			cout << "| Rating: " << setw(29) << left << string(rating, '*') << "|\n";
			cout << "| Comment: " << setw(28) << left << comment << "|\n";
			cout << "| Time: " << setw(31) << left << timestamp << "|\n";
			cout << "----------------------------------------\n";
		}
};

class User {
	private:
		string username;
		string password;
		string name;
		string phone;
		string email;
		double rating;
		int totalRides;
		vector<Review> reviews;

	public:
		User(string u, string p, string n, string ph, string e)
			: username(u), password(p), name(n), phone(ph), email(e), rating(0.0), totalRides(0) {}

		string getUsername() const {
			return username;
		}
		string getPassword() const {
			return password;
		}
		string getName() const {
			return name;
		}
		string getPhone() const {
			return phone;
		}
		string getEmail() const {
			return email;
		}
		double getRating() const {
			return rating;
		}
		int getTotalRides() const {
			return totalRides;
		}
		vector<Review> getReviews() const {
			return reviews;
		}

		void setRating(double r) {
			rating = r;
		}
		void incrementRides() {
			totalRides++;
		}

		void addReview(const Review& review) {
			reviews.push_back(review);
			updateRating();
		}

		void updateRating() {
			if (reviews.empty()) {
				rating = 0.0;
				return;
			}
			double total = 0.0;
			for (const auto& review : reviews) {
				total += review.getRating();
			}
			rating = total / reviews.size();
		}

		void display() const {
			cout << "Username: " << username << " | Name: " << name
			     << " | Rating: " << fixed << setprecision(1) << rating
			     << " | Rides: " << totalRides << endl;
		}

		void displayReviews() const {
			if (reviews.empty()) {
				cout << "No reviews available.\n";
				return;
			}
			cout << "\nReviews for " << username << ":\n";
			for (const auto& review : reviews) {
				review.displayReview();
			}
		}
};

class Graph {
	private:
		unordered_map<string, vector<pair<string, double>>> adjList;
    unordered_map<string, string> locationNames;

    void displaySouthKarachiMap() {
     system("cls");
    cout << "\n\n             SOUTH KARACHI MAP\n";
    cout << "          ========================\n";
    cout << "                   Arabian Sea         \n";
    cout << "                   |         |         \n";
    cout << "             Clifton Sea View(36)\n";
    cout << "                   |         |         \n";
    cout << "  Clifton(1)----Defence(9)---Boat Basin(37)\n";
    cout << "       |           |                   |     \n";
    cout << "Clifton Blk2(28)  Clifton Blk8(45)    Karachi Cantt(8)\n";
    cout << "       |           |                   |\n";
    cout << "    Saddar(4)-------- Saddar Market(35)\n";
    cout << "       |                     |     \n";
    cout << "  Frere Town(25)     Shahrah-e-Faisal(24)\n";
    cout << "========================================\n";
    cout << "Key Locations: 1,4,8,9,24,25,28,35,36,37,45\n";
}

void displayCentralKarachiMap() {
 system("cls");
    cout << "\n\n             CENTRAL KARACHI MAP\n";
    cout << "          ========================\n";
    cout << "   Gulshan-e-Iqbal(2)--Bahadurabad(10)\n";
    cout << "       |                 |           \n";
    cout << "   Tariq Road(38)    PECHS(13)--PECHS Block6(34)\n";
    cout << "       |                 |           \n";
    cout << "  Federal B Area(14)--Gulberg(15)--Jamshed Quarters(16)\n";
    cout << "       |                 |           \n";
    cout << "  Nazimabad(11)--North Nazimabad(6)--Liaquatabad(12)\n";
    cout << "       |                 |           \n";
    cout << "  Buffer Zone(27)   Nazimabad No.4(39)\n";
    cout << "       |                           \n";
    cout << "Soldier Bazaar(33)  PIB Colony(44)\n";
    cout << "========================================\n";
    cout << "Key Locations: 2,6,10,11,12,13,14,15,16,27,33,34,38,39,44\n";
}

void displayEastKarachiMap() {
 system("cls");
    cout << "\n\n             EAST KARACHI MAP\n";
    cout << "          ====================\n";
    cout << "  Korangi(3)--Korangi Ind.Area(42)--Korangi No.5(47)\n";
    cout << "    |                 |                 |    \n";
    cout << "  Korangi Creek(30)  Landhi(19)     Shah Faisal(20)\n";
    cout << "    |                 |                 |    \n";
    cout << "  Malir(7)------Malir Cantonment(31)  Shah Latif(43)\n";
    cout << "    |                                   \n";
    cout << "  Malir River Bank(48)                     \n";
    cout << "    |                                   \n";
    cout << "   Karsaz(22)--Karachi University(23)            \n";
    cout << "========================================\n";
    cout << "Key Locations: 3,7,19,20,22,23,30,31,42,43,47,48\n";
}

void displayWestKarachiMap() {
 system("cls");
    cout << "\n\n              WEST KARACHI MAP\n";
    cout << "          ====================\n";
    cout << "  SITE Town(17)--Orangi Town(18)--North Karachi(41)\n";
    cout << "    |                 |                 |    \n";
    cout << "  Gulshan-e-Maymar(40) Buffer Zone(27)  Surjani\n";
    cout << "    |                                   \n";
    cout << "  North Nazimabad(6)--Nazimabad(11)     \n";
    cout << "    |                 |                   \n";
    cout << "  Federal B Area(14)  Liaquatabad(12)        \n";
    cout << "    |                                   \n";
    cout << "  Gulberg(15)------Gulshan-e-Iqbal(2)     \n";
    cout << "========================================\n";
    cout << "Key Locations: 2,6,11,12,14,15,17,18,27,40,41\n";
}

void displayCoastalMap() {
 system("cls");
    cout << "\n\n          COASTAL AREAS MAP\n";
    cout << "          =====================\n";
    cout << "              Arabian Sea\n";
    cout << "               |         |         |         \n";
    cout << "        Hawksbay(50) Sea View(36) Karachi Port(49)\n";
    cout << "               |         |         |         \n";
    cout << "        Gizri Creek(46) Boat Basin(37) Karachi Cantt(8)\n";
    cout << "               |         |         |         \n";
    cout << "        Gizri(21)    Clifton(1)    Defence(9)\n";
    cout << "               |         |         |         \n";
    cout << "        Karsaz(22)  Shahrah-e-Faisal(24)      \n";
    cout << "               |                           \n";
    cout << "        Karachi University(23)              \n";
    cout << "========================================\n";
    cout << "Key Locations: 1,8,9,21,22,23,24,36,37,46,49,50\n";
}

public:
    void displayRegionalMap() {
        int regionChoice;
        
        do {
            cout << "\nKARACHI REGIONAL MAPS\n";
            cout << "========================================\n";
            cout << "1. South Karachi (Clifton, Defence, Saddar)\n";
            cout << "2. Central Karachi (Gulshan, PECHS, Nazimabad)\n";
            cout << "3. East Karachi (Korangi, Landhi, Malir)\n";
            cout << "4. West Karachi (SITE, Orangi, North Karachi)\n";
            cout << "5. Coastal Areas (Sea View, Hawksbay, Port)\n";
            cout << "6. Return to Main Menu\n";
            cout << "Choose region to view: ";
            cin >> regionChoice;
            
            switch(regionChoice) {
                case 1:
                    displaySouthKarachiMap();
                    break;
                case 2:
                    displayCentralKarachiMap();
                    break;
                case 3:
                    displayEastKarachiMap();
                    break;
                case 4:
                    displayWestKarachiMap();
                    break;
                case 5:
                    displayCoastalMap();
                    break;
                case 6:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
            
            if (regionChoice >= 1 && regionChoice <= 5) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } while (regionChoice != 6);
    }
void displayVisualMap() {
    cout << "\n🗺  KARACHI CITY MAP - KEY LOCATIONS\n";
    cout << "==================================================\n\n";
    
    vector<pair<string, vector<string>>> areas = {
        {"SOUTH KARACHI", {"Clifton", "Clifton Block 2", "Clifton Sea View", "Clifton Block 8", 
                          "Boat Basin", "Defence", "Karachi Cantt", "Saddar", "Saddar Market"}},
        
        {"CENTRAL KARACHI", {"Gulshan-e-Iqbal", "Bahadurabad", "PECHS", "PECHS Block 6", 
                           "Tariq Road", "Federal B Area", "Gulberg", "Nazimabad", 
                           "North Nazimabad", "Liaquatabad", "Liaquatabad No.10"}},
        
        {"WEST KARACHI", {"SITE Town", "Orangi Town", "North Karachi", "Gulshan-e-Maymar", 
                         "Buffer Zone", "Nazimabad No.4"}},
        
        {"EAST KARACHI", {"Korangi", "Korangi Creek", "Korangi Industrial Area", "Korangi No.5", 
                         "Landhi", "Shah Faisal Town", "Shah Latif Town", "Malir", 
                         "Malir Cantonment"}},
        
        {"OTHER AREAS", {"Gizri", "Karsaz", "Karachi University", "Shahrah-e-Faisal", 
                       "Frere Town", "Tipu Sultan Road", "Jamshed Quarters", 
                       "Soldier Bazaar", "PIB Colony", "Malir River Bank", 
                       "Karachi Port", "Hawksbay", "Gizri Creek", "DHA Phase 5"}}
    };
    
    for (const auto& area : areas) {
        cout << "📍 " << area.first << ":\n";
        cout << "--------------------------------------------------\n";
        
        const vector<string>& locations = area.second;
        int locationsPerLine = 3;
        
        for (size_t i = 0; i < locations.size(); i += locationsPerLine) {
            for (int j = 0; j < locationsPerLine && (i + j) < locations.size(); j++) {
                cout << "• " << setw(25) << left << locations[i + j];
            }
            cout << endl;
        }
        cout << endl;
    }
    
    cout << "==================================================\n";
    cout << "📍 Total Locations: 50 | Use numbers 1-50 for booking\n";
}
		
		void addEdge(string u, string v, double weight) {
			adjList[u].push_back({v, weight});
			adjList[v].push_back({u, weight});
		}

		void setLocationName(string node, string name) {
			locationNames[node] = name;
		}

		string getLocationName(string node) const {
			if (locationNames.find(node) != locationNames.end()) {
				return locationNames.at(node);
			}
			return "Unknown Location";
		}

		pair<double, vector<string>> shortestPath(string start, string end) {
			if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
				return {-1, {}};
			}

			priority_queue<pair<double, string>,
			               vector<pair<double, string>>,
			               greater<pair<double, string>>> pq;

			unordered_map<string, double> dist;
			unordered_map<string, string> prev;
			unordered_set<string> visited;

			for (const auto& node : adjList) {
				dist[node.first] = numeric_limits<double>::max();
			}

			dist[start] = 0;
			pq.push({0, start});

			while (!pq.empty()) {
				string current = pq.top().second;
				pq.pop();

				if (current == end) break;
				if (visited.count(current)) continue;
				visited.insert(current);

				for (const auto& neighbor : adjList[current]) {
					string nextNode = neighbor.first;
					double weight = neighbor.second;

					if (!visited.count(nextNode)) {
						double newDist = dist[current] + weight;
						if (newDist < dist[nextNode]) {
							dist[nextNode] = newDist;
							prev[nextNode] = current;
							pq.push({newDist, nextNode});
						}
					}
				}
			}

			if (dist[end] == numeric_limits<double>::max()) {
				return {-1, {}};
			}

			vector<string> path;
			string current = end;
			while (current != start) {
				path.push_back(current);
				current = prev[current];
			}
			path.push_back(start);
			reverse(path.begin(), path.end());

			return {dist[end], path};
		}

		void displayLocations() {
			cout << "\nAvailable Karachi Locations (1-50):\n";
			cout << "========================================\n";

			for (int i = 1; i <= 50; i += 2) {
				string node1 = to_string(i);
				string node2 = to_string(i + 1);

				cout << setw(2) << node1 << ": " << setw(25) << left << getLocationName(node1);
				if (i + 1 <= 50) {
					cout << " | " << setw(2) << node2 << ": " << setw(25) << left << getLocationName(node2);
				}
				cout << endl;
			}
			cout << "========================================\n";
		}

		bool locationExists(const string& location) {
			return adjList.find(location) != adjList.end();
		}
};

class Ride {
	private:
		static int nextRideID;

		int rideID;
		string userUsername;
		string driverUsername;
		string source;
		string destination;
		RideStatus status;
		double fare;
		double distance;
		vector<string> route;
		double proposedFare;
		bool fareNegotiated;
		string vehicleType;
		bool reviewed;
		string currentLocation;
		

	public:
		Ride(string user, string src, string dest, string vehicle = "Car")
			: rideID(nextRideID++), userUsername(user), source(src),
			  destination(dest), status(RideStatus::PENDING), fare(0.0),
			  distance(0.0), proposedFare(0.0), fareNegotiated(false), vehicleType(vehicle), reviewed(false), currentLocation(src) {
			driverUsername = "";
		}

		int getRideID() const {
			return rideID;
		}
		string getUserUsername() const {
			return userUsername;
		}
		string getDriverUsername() const {
			return driverUsername;
		}
		string getSource() const {
			return source;
		}
		string getDestination() const {
			return destination;
		}
		RideStatus getStatus() const {
			return status;
		}
		double getFare() const {
			return fare;
		}
		double getDistance() const {
			return distance;
		}
		vector<string> getRoute() const {
			return route;
		}
		double getProposedFare() const {
			return proposedFare;
		}
		bool isFareNegotiated() const {
			return fareNegotiated;
		}
		string getVehicleType() const {
			return vehicleType;
		}
		bool isReviewed() const {
			return reviewed;
		}
		string getCurrentLocation() const {
			return currentLocation;
		}

		void assignDriver(string driver) {
			driverUsername = driver;
			status = RideStatus::ONGOING;
		}

		void completeRide(double rideFare) {
			status = RideStatus::COMPLETED;
			fare = rideFare;
			currentLocation = destination;
		}

		void setFare(double f) {
			fare = f;
		}
		void setDistance(double dist) {
			distance = dist;
		}
		void setRoute(const vector<string>& path) {
			route = path;
		}
		void setProposedFare(double pf) {
			proposedFare = pf;
		}
		void setFareNegotiated(bool negotiated) {
			fareNegotiated = negotiated;
		}
		void setVehicleType(string vehicle) {
			vehicleType = vehicle;
		}
		void setReviewed(bool isReviewed) {
			reviewed = isReviewed;
		}
		void setCurrentLocation(string location) {
			currentLocation = location;
		}
		void cancelRide() {
			status = RideStatus::CANCELLED;
		}

		void displayRide() const {
			cout << "\n=== RIDE DETAILS ===" << endl;
			cout << "Ride ID: " << rideID << endl;
			cout << "User: " << userUsername << endl;
			cout << "Driver: " << (driverUsername.empty() ? "Not Assigned" : driverUsername) << endl;
			cout << "Route: " << source << " → " << destination << endl;
			cout << "Current Location: " << currentLocation << endl;
			cout << "Vehicle Type: " << vehicleType << endl;
			cout << "Status: " << rideStatusToString(status) << endl;
			cout << "Distance: " << fixed << setprecision(2) << distance << " km" << endl;

			if (status == RideStatus::COMPLETED || status == RideStatus::ONGOING) {
				cout << "Fare: $" << fixed << setprecision(2) << fare << endl;
			}

			if (proposedFare > 0) {
				cout << "Proposed Fare: $" << fixed << setprecision(2) << proposedFare << endl;
			}

			if (status == RideStatus::COMPLETED) {
				cout << "Reviewed: " << (reviewed ? "Yes" : "No") << endl;
			}

			if (!route.empty()) {
				cout << "Optimal Route: ";
				for (size_t i = 0; i < route.size(); ++i) {
					cout << route[i];
					if (i < route.size() - 1) cout << " → ";
				}
				cout << endl;
			}
			cout << "===================" << endl;
		}
};

int Ride::nextRideID = 1;

class RouteXSystem {
	private:
		Graph cityMap;
		vector<User> users;
		vector<Ride> rides;
		vector<Review> reviews;
		vector<string> availableDrivers;
		unordered_map<string, string> driverLocations;
		unordered_map<string, string> driverVehicles;
		void simulateRideProgress(Ride& ride);
		void animateCarMovement(const string& fromLocation, const string& toLocation);
	    void animateCarMovementClean(const string& fromLocation, const string& toLocation);
	    void drawCar(int position);
	    
void repeatPreviousRide(User* user) {
    displayHeader("REPEAT PREVIOUS RIDE");
    
    vector<Ride> userCompletedRides;
    for (const auto& ride : rides) {
        if (ride.getUserUsername() == user->getUsername() && 
            ride.getStatus() == RideStatus::COMPLETED) {
            userCompletedRides.push_back(ride);
        }
    }
    
    if (userCompletedRides.empty()) {
        cout << "❌ No previous rides found to repeat.\n";
        cout << "💡 Complete at least one ride to use this feature.\n";
        waitForEnter();
        return;
    }
    
    cout << "🔄 Select a previous ride to repeat:\n";
    cout << "========================================\n";
    for (size_t i = 0; i < userCompletedRides.size(); i++) {
        if (i >= 10) break;
        cout << i+1 << ". " << userCompletedRides[i].getSource() << " → " 
             << userCompletedRides[i].getDestination() << endl;
        cout << "   Vehicle: " << userCompletedRides[i].getVehicleType();
        cout << " | Fare: $" << fixed << setprecision(2) << userCompletedRides[i].getFare();
        cout << " | Date: Ride ID " << userCompletedRides[i].getRideID() << endl;
        cout << "----------------------------------------\n";
    }
    
    if (userCompletedRides.size() > 10) {
        cout << "... and " << (userCompletedRides.size() - 10) << " more rides\n";
    }
    
    cout << "0. Cancel\n";
    cout << "Choose ride to repeat (1-" << min((size_t)10, userCompletedRides.size()) << "): ";
    
    int choice;
    cin >> choice;
    
    if (choice == 0 || choice < 1 || choice > min((size_t)10, userCompletedRides.size())) {
        cout << "Operation cancelled.\n";
        waitForEnter();
        return;
    }
    
    Ride selectedRide = userCompletedRides[choice - 1];
    
    cout << "\n✅ Selected: " << cityMap.getLocationName(selectedRide.getSource()) 
         << " → " << cityMap.getLocationName(selectedRide.getDestination()) << endl;
    cout << "🚗 Vehicle: " << selectedRide.getVehicleType() << endl;
    cout << "\nDo you want to book this ride again? (y/n): ";
    
    char confirm;
    cin >> confirm;
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Ride repetition cancelled.\n";
        waitForEnter();
        return;
    }
    
    bookRepeatedRide(user, selectedRide);
}

		const string USERS_FILE = "users.txt";
		const string RIDES_FILE = "rides.txt";
		const string REVIEWS_FILE = "reviews.txt";
		const string DRIVERS_FILE = "drivers.txt";

		void initializeCityMap() {
			unordered_map<string, string> karachiLocations = {
				{"1", "Clifton"}, {"2", "Gulshan-e-Iqbal"}, {"3", "Korangi"}, {"4", "Saddar"}, {"5", "Lyari"},
				{"6", "North Nazimabad"}, {"7", "Malir"}, {"8", "Karachi Cantt"}, {"9", "Defence"}, {"10", "Bahadurabad"},
				{"11", "Nazimabad"}, {"12", "Liaquatabad"}, {"13", "PECHS"}, {"14", "Federal B Area"}, {"15", "Gulberg"},
				{"16", "Jamshed Quarters"}, {"17", "SITE Town"}, {"18", "Orangi Town"}, {"19", "Landhi"}, {"20", "Shah Faisal Town"},
				{"21", "Gizri"}, {"22", "Karsaz"}, {"23", "Karachi University"}, {"24", "Shahrah-e-Faisal"}, {"25", "Frere Town"},
				{"26", "Tipu Sultan Road"}, {"27", "Buffer Zone"}, {"28", "Clifton Block 2"}, {"29", "DHA Phase 5"}, {"30", "Korangi Creek"},
				{"31", "Malir Cantonment"}, {"32", "Liaquatabad No.10"}, {"33", "Soldier Bazaar"}, {"34", "PECHS Block 6"}, {"35", "Saddar Market"},
				{"36", "Clifton Sea View"}, {"37", "Boat Basin"}, {"38", "Tariq Road"}, {"39", "Nazimabad No.4"}, {"40", "Gulshan-e-Maymar"},
				{"41", "North Karachi"}, {"42", "Korangi Industrial Area"}, {"43", "Shah Latif Town"}, {"44", "PIB Colony"}, {"45", "Clifton Block 8"},
				{"46", "Gizri Creek"}, {"47", "Korangi No.5"}, {"48", "Malir River Bank"}, {"49", "Karachi Port"}, {"50", "Hawksbay"}
			};

			for (int i = 1; i <= 50; i++) {
				string node = to_string(i);
				cityMap.setLocationName(node, karachiLocations[node]);
			}

			for (int i = 1; i < 50; i++) {
				string u = to_string(i);
				string v = to_string(i + 1);
				double weight = 1.0 + (rand() % 10);
				cityMap.addEdge(u, v, weight);
			}

			srand(1);

			for (int u = 1; u <= 50; u++) {
				int edgeCount = 1 + rand() % 5;

				for (int i = 0; i < edgeCount; i++) {
					int v = 1 + rand() % 50;
					if (v == u) continue;

					string nodeU = to_string(u);
					string nodeV = to_string(v);
					double weight = 1.0 + (rand() % 15);

					cityMap.addEdge(nodeU, nodeV, weight);
				}
			}

			cityMap.addEdge("1", "28", 2.5);
			cityMap.addEdge("1", "36", 3.0);
			cityMap.addEdge("1", "9", 5.0);
			cityMap.addEdge("2", "10", 2.0);
			cityMap.addEdge("2", "13", 3.0);
			cityMap.addEdge("4", "35", 1.0);
			cityMap.addEdge("6", "11", 2.0);
			cityMap.addEdge("6", "14", 3.0);
			cityMap.addEdge("17", "18", 5.0);
			cityMap.addEdge("25", "35", 1.5);
		}
		void bookRepeatedRide(User* user, const Ride& originalRide) {
    
	displayHeader("BOOKING REPEATED RIDE");
    
    cout << "🔄 Repeating your previous ride...\n";
    cout << "📍 From: " << cityMap.getLocationName(originalRide.getSource()) << endl;
    cout << "  To: " << cityMap.getLocationName(originalRide.getDestination()) << endl;
    cout << "🚗 Vehicle: " << originalRide.getVehicleType() << endl;
    
    auto [distance, route] = cityMap.shortestPath(originalRide.getSource(), originalRide.getDestination());
    
    if (distance < 0) {
        cout << "❌ No route found between locations!\n";
        waitForEnter();
        return;
    }
    
    double baseFare = calculateFare(distance, originalRide.getVehicleType());
    
    cout << "\n=== ROUTE FOUND ===\n";
    cout << "Vehicle Type: " << originalRide.getVehicleType() << endl;
    cout << "Optimal Route: ";
    for (size_t i = 0; i < route.size(); ++i) {
        cout << route[i];
        if (i < route.size() - 1) cout << " → ";
    }
    cout << "\nTotal Distance: " << fixed << setprecision(2) << distance << " km";
    cout << "\nEstimated Fare: $" << fixed << setprecision(2) << baseFare;
    
    displayNearestDrivers(originalRide.getSource(), originalRide.getVehicleType());
    
    Ride newRide(user->getUsername(), originalRide.getSource(), originalRide.getDestination(), originalRide.getVehicleType());
    newRide.setRoute(route);
    newRide.setDistance(distance);
    newRide.setProposedFare(baseFare);
    newRide.setFareNegotiated(false);
    
    rides.push_back(newRide);
    saveRides();
    
    cout << "\n✅ Ride booked successfully! Ride ID: " << newRide.getRideID() << endl;
    cout << "⏳ Waiting for driver assignment...\n";
    
    string assignedDriver = assignDriverToRide(newRide.getRideID(), originalRide.getVehicleType());
    
    if (!assignedDriver.empty()) {
        cout << "\n🚗 Driver " << assignedDriver << " is on the way to pick you up!\n";
        cout << "📍 Pickup Location: " << cityMap.getLocationName(originalRide.getSource()) << "\n";
        waitForEnter();
        
        displayHeader("DRIVER REACHED!");
        cout << "🎯 Driver " << assignedDriver << " has reached your location!\n";
        cout << "✅ Ride is starting now...\n";
        waitForEnter();
        
        simulateRideProgress(newRide);
        
        completeRideWithReview(newRide.getRideID());
    } else {
        cout << "❌ No drivers available at the moment. Please try again later.\n";
        waitForEnter();
    }
}
	
		void initializeDrivers() {
			loadDriversFromFile();

			if (availableDrivers.empty()) {
				createDefaultDrivers();
			}
		}

		void loadDriversFromFile() {
			ifstream file(DRIVERS_FILE);
			if (!file) {
				cout << "❌ Could not load drivers file. Creating default drivers.\n";
				return;
			}

			string driverName, vehicleType, location;
			while (getline(file, driverName)) {
				if (getline(file, vehicleType) && getline(file, location)) {
					availableDrivers.push_back(driverName);
					driverVehicles[driverName] = vehicleType;
					driverLocations[driverName] = location;
				}
			}
			file.close();
		}

		void createDefaultDrivers() {
			vector<string> pakistaniNames = {
				"Faraz_Khan", "Muhammad_Ali", "Bilal_Shah", "Usman_Raza", "Hassan_Akhtar",
				"Farhan_Malik", "Zubair_Ahmed", "Kamran_Hussain", "Tariq_Mehmood", "Javed_Iqbal",
				"Nasir_Chaudhry", "Salman_Khan", "Imran_Yousuf", "Asif_Zaman", "Rashid_Mahmood",
				"Waqar_Abbas", "Faisal_Siddiqui", "Shahid_Pervez", "Arif_Khalid", "Noman_Hayat"
			};

			vector<string> vehicles = {"Bike", "Car", "Rickshaw"};

			for (int i = 0; i < 20; i++) {
				string driverName = "driver_" + pakistaniNames[i];
				string vehicleType = vehicles[rand() % vehicles.size()];
				string location = to_string(1 + rand() % 50);

				availableDrivers.push_back(driverName);
				driverVehicles[driverName] = vehicleType;
				driverLocations[driverName] = location;
			}

			saveDriversToFile();
		}

		void saveDriversToFile() {
			ofstream file(DRIVERS_FILE);
			if (!file) {
				cout << "❌ Could not save drivers to file.\n";
				return;
			}

			for (const auto& driver : availableDrivers) {
				file << driver << "\n"
				     << driverVehicles[driver] << "\n"
				     << driverLocations[driver] << "\n";
			}
			file.close();
		}

		bool validateUsername(const string& username) {
			if (username.length() < 8) {
				cout << "❌ Username must be at least 8 characters long.\n";
				return false;
			}

			if (username.length() > 20) {
				cout << "❌ Username must be at most 20 characters long.\n";
				return false;
			}

			bool hasNumber = false;
			bool hasUnderscore = false;

			for (char c : username) {
				if (isdigit(c)) hasNumber = true;
				if (c == '_') hasUnderscore = true;
			}

			if (!hasNumber) {
				cout << "❌ Username must contain at least one number (0-9).\n";
				return false;
			}

			if (!hasUnderscore) {
				cout << "❌ Username must contain at least one underscore (_).\n";
				return false;
			}

			return true;
		}

		bool validatePassword(const string& password) {
			if (password.length() < 8) {
				cout << "❌ Password must be at least 8 characters long.\n";
				return false;
			}

			if (password.length() > 16) {
				cout << "❌ Password must be at most 16 characters long.\n";
				return false;
			}

			int digitCount = 0;
			bool hasUpper = false;
			bool hasSpecial = false;

			for (char c : password) {
				if (isdigit(c)) digitCount++;
				if (isupper(c)) hasUpper = true;
				if (!isalnum(c)) hasSpecial = true;
			}

			if (digitCount < 3) {
				cout << "❌ Password must contain at least 3 digits (0-9).\n";
				return false;
			}
			if (!hasUpper) {
				cout << "❌ Password must contain at least one uppercase letter (A-Z).\n";
				return false;
			}
			if (!hasSpecial) {
				cout << "❌ Password must contain at least one special character (!@#$%^&* etc.).\n";
				return false;
			}

			return true;
		}

		bool validateName(const string& name) {
			if (name.length() < 2) {
				cout << "❌ Name must be at least 2 characters long.\n";
				return false;
			}

			if (name.length() > 30) {
				cout << "❌ Name must be at most 30 characters long.\n";
				return false;
			}

			for (char c : name) {
				if (!isalpha(c) && c != ' ') {
					cout << "❌ Name can only contain letters and spaces.\n";
					return false;
				}
			}

			return true;
		}

		bool validatePhone(const string& phone) {
			if (phone.length() != 12) {
				cout << "❌ Phone must be 12 characters in format 03XX-XXXXXXX.\n";
				return false;
			}
			if (phone[4] != '-') {
				cout << "❌ Phone format must be 03XX-XXXXXXX.\n";
				return false;
			}

			for (int i = 0; i < 12; i++) {
				if (i == 4) continue;
				if (!isdigit(phone[i])) {
					cout << "❌ Phone must contain only digits and one hyphen.\n";
					return false;
				}
			}

			if (phone[0] != '0' || phone[1] != '3') {
				cout << "❌ Phone must start with 03.\n";
				return false;
			}

			return true;
		}

		bool validateEmail(const string& email) {
			string suffix = "@gmail.com";
			if (email.length() <= suffix.length()) {
				cout << "❌ Invalid email length.\n";
				return false;
			}

			if (email.substr(email.length() - suffix.length()) != suffix) {
				cout << "❌ Email must end with @gmail.com\n";
				return false;
			}

			return true;
		}

		bool validateLocation(const string& location) {
			try {
				int locNum = stoi(location);
				if (locNum < 1 || locNum > 50) {
					cout << "❌ Location must be a number between 1 and 50.\n";
					return false;
				}
			} catch (const exception& e) {
				cout << "❌ Location must be a number between 1 and 50.\n";
				return false;
			}

			return cityMap.locationExists(location);
		}

		void clearScreen() {
			system("cls");
		}

		void waitForEnter() {
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
		}

		void displayHeader(const string& title) {
			clearScreen();
			animation(title, COLOR_GREEN, 40);
		}

		
	

		double negotiateFare(double baseFare) {
			double proposedFare;
			int attemptCount = 0;
			const double MIN_ALLOWED = baseFare * 0.95;

			while (true) {
				cout << "Enter your proposed fare: $";
				if (!(cin >> proposedFare)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "❌ Invalid input! Please enter a numeric value.\n";
					continue;
				}

				if (proposedFare >= MIN_ALLOWED) {
					cout << "✅ Fare accepted!\n";
					return proposedFare;
				} else {
					attemptCount++;
					cout << "❌ Your proposed fare is too low (less than 95% of system fare).\n";
					cout << "💡 System fare: $" << fixed << setprecision(2) << baseFare << endl;
					cout << "💡 Minimum allowed: $" << fixed << setprecision(2) << MIN_ALLOWED << endl;

					int choice;
					while (true) {
						cout << "\nOptions:\n";
						cout << "1. Propose a new fare\n";
						cout << "2. Cancel ride\n";
						cout << "Choose option (1-2): ";

						if (cin >> choice) {
							if (choice == 1 || choice == 2) {
								break;
							}
						}

						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "❌ Invalid choice! Please enter 1 or 2.\n";
					}

					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					if (choice == 2) {
						return -1;
					}
				}
			}
		}

	public:
		RouteXSystem() {
			srand(1);
			initializeCityMap();
			initializeDrivers();
			loadUsers();
			loadRides();
			loadReviews();
		}

		bool registerUser() {
			displayHeader("USER REGISTRATION");

			string username, password, name, phone, email;

			cout << "📝 Registration Requirements:\n";
			cout << "• Username: 8-20 chars, must include number and underscore\n";
			cout << "• Password: 8-16 chars, 3 digits, 1 uppercase, 1 special char\n";
			cout << "• Name: 2-30 chars, letters and spaces only\n";
			cout << "• Phone: 03XX-XXXXXXX format\n";
			cout << "• Email: must end with @gmail.com\n\n";

			while (true) {
				cout << "Enter Username: ";
				cin >> username;

				if (!validateUsername(username)) {
					continue;
				}

				bool exists = false;
				for (const auto& user : users) {
					if (user.getUsername() == username) {
						cout << "❌ Username already exists! Please choose another.\n";
						exists = true;
						break;
					}
				}
				if (!exists) break;
			}

			while (true) {
				cout << "Enter Password: ";
				cin >> password;
				if (validatePassword(password)) break;
			}

			while (true) {
				cout << "Enter Name: ";
				cin.ignore();
				getline(cin, name);
				if (validateName(name)) break;
			}

			while (true) {
				cout << "Enter Phone (03XX-XXXXXXX): ";
				cin >> phone;
				if (validatePhone(phone)) break;
			}

			while (true) {
				cout << "Enter Email: ";
				cin >> email;
				if (validateEmail(email)) break;
			}

			User newUser(username, password, name, phone, email);
			users.push_back(newUser);
			saveUsers();

			cout << "\n✅ Registration successful! Welcome to RouteX, " << name << "!\n";
			waitForEnter();
			return true;
		}

		User* loginUser() {
			displayHeader("USER LOGIN");

			string username, password;

			cout << "Username: ";
			cin >> username;
			cout << "Password: ";
			cin >> password;

			for (auto& user : users) {
				if (user.getUsername() == username && user.getPassword() == password) {
					cout << "\n✅ Login successful! Welcome back, " << user.getName() << "!\n";
					waitForEnter();
					return &user;
				}
			}

			cout << "\n❌ Invalid username or password!\n";
			waitForEnter();
			return nullptr;
		}

		User* findUserByUsername(const string& username) {
			for (auto& user : users) {
				if (user.getUsername() == username) {
					return &user;
				}
			}
			return nullptr;
		}

		string selectVehicleType() {
			int choice;
			cout << "\nSelect Vehicle Type:\n";
			cout << "1. Bike (Base fare: $2.00 + $0.80/km)\n";
			cout << "2. Car (Base fare: $3.00 + $1.20/km)\n";
			cout << "3. Rickshaw (Base fare: $2.50 + $1.00/km)\n";
			cout << "Enter your choice (1-3): ";
			cin >> choice;

			switch(choice) {
				case 1:
					return "Bike";
				case 2:
					return "Car";
				case 3:
					return "Rickshaw";
				default:
					cout << "❌ Invalid choice, defaulting to Car.\n";
					return "Car";
			}
		}

		double calculateFare(double distance, const string& vehicleType) {
			double baseFare, perKmRate;

			if (vehicleType == "Bike") {
				baseFare = 2.00;
				perKmRate = 0.80;
			} else if (vehicleType == "Car") {
				baseFare = 3.00;
				perKmRate = 1.20;
			} else if (vehicleType == "Rickshaw") {
				baseFare = 2.50;
				perKmRate = 1.00;
			} else {
				baseFare = 3.00;
				perKmRate = 1.20;
			}

			return baseFare + (distance * perKmRate);
		}

		void displayNearestDrivers(const string& source, const string& vehicleType) {
			cout << "\n🔍 Finding nearest " << vehicleType << " drivers to your location...\n";

			vector<pair<string, double>> driverDistances;

			for (const auto& driver : availableDrivers) {
				if (driverVehicles[driver] == vehicleType) {
					string driverLocation = driverLocations[driver];
					auto [distance, route] = cityMap.shortestPath(driverLocation, source);

					if (distance >= 0) {
						driverDistances.push_back({driver, distance});
					}
				}
			}

			sort(driverDistances.begin(), driverDistances.end(),
			[](const auto& a, const auto& b) {
				return a.second < b.second;
			});

			if (driverDistances.empty()) {
				cout << "❌ No " << vehicleType << " drivers available near your location.\n";
			} else {
				cout << "\n🏍  Available Drivers (Nearest to Farthest):\n";
				cout << "----------------------------------------\n";
				for (size_t i = 0; i < driverDistances.size(); i++) {
					cout << i+1 << ". " << driverDistances[i].first
					     << " (" << vehicleType << ") - "
					     << fixed << setprecision(1) << driverDistances[i].second
					     << " km away\n";
				}
				cout << "----------------------------------------\n";
			}
		}

		void bookRide(User* user) {
			displayHeader("BOOK A RIDE");
			animation("Available Karachi Locations", COLOR_CYAN, 40, false);
			cityMap.displayLocations();

			string source, destination;

			while (true) {
				cout << "\nEnter Source Location (1-50): ";
				cin >> source;
				if (validateLocation(source)) break;
			}

			while (true) {
				cout << "Enter Destination Location (1-50): ";
				cin >> destination;
				if (validateLocation(destination)) break;
			}

			if (source == destination) {
				cout << "❌ Source and destination cannot be the same!\n";
				waitForEnter();
				return;
			}

			string vehicleType = selectVehicleType();

			displayNearestDrivers(source, vehicleType);

			auto [distance, route] = cityMap.shortestPath(source, destination);

			if (distance < 0) {
				cout << "❌ No route found between " << source << " and " << destination << "!\n";
				waitForEnter();
				return;
			}

			double baseFare = calculateFare(distance, vehicleType);

			cout << "\n=== ROUTE FOUND ===\n";
			cout << "Vehicle Type: " << vehicleType << endl;
			cout << "Optimal Route: ";
			for (size_t i = 0; i < route.size(); ++i) {
				cout << route[i];
				if (i < route.size() - 1) cout << " → ";
			}
			cout << "\nTotal Distance: " << fixed << setprecision(2) << distance << " km";
			cout << "\nBase Fare: $" << fixed << setprecision(2) << baseFare;

			double proposedFare = baseFare;
			cout << "\n\nDo you want to propose a different fare? (y/n): ";
			char choice;
			cin >> choice;

			if (choice == 'y' || choice == 'Y') {
				proposedFare = negotiateFare(baseFare);
				if (proposedFare < 0) {
					cout << "❌ Ride cancelled by user.\n";
					waitForEnter();
					return;
				}
			}

			Ride newRide(user->getUsername(), source, destination, vehicleType);
			newRide.setRoute(route);
			newRide.setDistance(distance);
			newRide.setProposedFare(proposedFare);
			newRide.setFareNegotiated(choice == 'y' || choice == 'Y');

			rides.push_back(newRide);
			saveRides();

			cout << "\n✅ Ride booked successfully! Ride ID: " << newRide.getRideID() << endl;
			cout << "⏳ Waiting for driver assignment...\n";

			string assignedDriver = assignDriverToRide(newRide.getRideID(), vehicleType);

			if (!assignedDriver.empty()) {
				cout << "\n🚗 Driver " << assignedDriver << " is on the way to pick you up!\n";
				cout << "📍 Pickup Location: " << cityMap.getLocationName(source) << "\n";
				waitForEnter();

				displayHeader("DRIVER REACHED!");
				cout << " Driver " << assignedDriver << " has reached your location!\n";
				cout << "✅ Ride is starting now...\n";
				waitForEnter();

				simulateRideProgress(newRide);

				completeRideWithReview(newRide.getRideID());
			} else {
				cout << "❌ No drivers available at the moment. Please try again later.\n";
				waitForEnter();
			}
		}

		string assignDriverToRide(int rideID, const string& vehicleType) {
			for (auto& ride : rides) {
				if (ride.getRideID() == rideID && ride.getStatus() == RideStatus::PENDING) {
					string nearestDriver = findNearestDriver(ride.getSource(), vehicleType);
					if (!nearestDriver.empty()) {
						ride.assignDriver(nearestDriver);

						driverLocations[nearestDriver] = ride.getSource();

						saveRides();
						return nearestDriver;
					}
				}
			}
			return "";
		}

		string findNearestDriver(const string& source, const string& vehicleType) {
			string nearestDriver = "";
			double minDistance = numeric_limits<double>::max();

			for (const auto& driver : availableDrivers) {
				if (driverVehicles[driver] != vehicleType) {
					continue;
				}

				string driverLocation = driverLocations[driver];
				auto [distance, route] = cityMap.shortestPath(driverLocation, source);

				if (distance >= 0 && distance < minDistance) {
					minDistance = distance;
					nearestDriver = driver;
				}
			}

			return nearestDriver;
		}

		void completeRideWithReview(int rideID) {
			for (auto& ride : rides) {
				if (ride.getRideID() == rideID && ride.getStatus() == RideStatus::ONGOING) {
					double finalFare = ride.getProposedFare() > 0 ? ride.getProposedFare() : calculateFare(ride.getDistance(), ride.getVehicleType());
					ride.completeRide(finalFare);

					for (auto& user : users) {
						if (user.getUsername() == ride.getUserUsername() ||
						        user.getUsername() == ride.getDriverUsername()) {
							user.incrementRides();
						}
					}

					saveRides();
					saveUsers();

					displayHeader("RIDE COMPLETED");
					animation("Ride Completed Successfully!", COLOR_GREEN, 40, false);
					cout << "🎉 Ride Completed Successfully!\n";
					cout << "💰 Final Fare: $" << fixed << setprecision(2) << finalFare << endl;
					cout << "📍 Destination: " << cityMap.getLocationName(ride.getDestination()) << endl;
					cout << "\nWe value your feedback! Please rate your experience.\n";

					giveReviewForRide(ride);
					return;
				}
			}
		}

		void giveReviewForRide(Ride& ride) {
			string driverUsername = ride.getDriverUsername();
			if (driverUsername.empty() || ride.isReviewed()) {
				return;
			}

			int rating;
			string comment;

			cout << "\n";
			animation("Rate Your Ride Experience", COLOR_YELLOW, 40, false);
			cout << "⭐ RATE YOUR RIDE EXPERIENCE ⭐\n";
			cout << "Driver: " << driverUsername << "\n";
			cout << "Vehicle: " << ride.getVehicleType() << "\n";
			cout << "Route: " << ride.getSource() << " → " << ride.getDestination() << "\n\n";

			cout << "Enter rating (1-5 stars): ";
			cin >> rating;
			cin.ignore();

			if (rating < 1 || rating > 5) {
				cout << "❌ Invalid rating. Using default rating of 5.\n";
				rating = 5;
			}

			cout << "Enter comment (optional): ";
			getline(cin, comment);

			Review review(ride.getUserUsername(), driverUsername, rating, comment);
			reviews.push_back(review);

			User* driver = findUserByUsername(driverUsername);
			if (driver) {
				driver->addReview(review);
			}

			ride.setReviewed(true);

			cout << "\n✅ Thank you for your feedback!\n";
			cout << "⭐ Rating submitted: " << string(rating, '*') << "\n";

			saveReviews();
			saveRides();
			saveUsers();
			waitForEnter();
		}

		void viewRides(User* user) {
			displayHeader("MY RIDES");

			vector<Ride> userRides;
			for (const auto& ride : rides) {
				if (ride.getUserUsername() == user->getUsername()) {
					userRides.push_back(ride);
				}
			}

			if (userRides.empty()) {
				cout << "No rides found.\n";
			} else {
				for (const auto& ride : userRides) {
					ride.displayRide();
				}
			}
			waitForEnter();
		}

		void viewMyReviews(User* user) {
			displayHeader("MY REVIEWS");

			if (user->getReviews().empty()) {
				cout << "No reviews received yet.\n";
			} else {
				cout << "Reviews received by " << user->getName() << ":\n";
				user->displayReviews();
			}
			waitForEnter();
		}

		void loadUsers() {
			ifstream file(USERS_FILE);
			if (!file) return;

			string username, password, name, phone, email;
			double rating;
			int totalRides;

			while (file >> username >> password >> name >> phone >> email >> rating >> totalRides) {
				replace(name.begin(), name.end(), '_', ' ');
				User user(username, password, name, phone, email);
				user.setRating(rating);
				for (int i = 0; i < totalRides; i++) user.incrementRides();
				users.push_back(user);
			}
			file.close();
		}

		void saveUsers() {
			ofstream file(USERS_FILE);
			for (const auto& user : users) {
				string name = user.getName();
				replace(name.begin(), name.end(), ' ', '_');
				file << user.getUsername() << " " << user.getPassword() << " "
				     << name << " " << user.getPhone() << " " << user.getEmail() << " "
				     << user.getRating() << " " << user.getTotalRides() << endl;
			}
			file.close();
		}

		void loadRides() {
			ifstream file(RIDES_FILE);
			if (!file) return;

			int rideID, statusInt;
			string user, driver, source, dest, vehicleType;
			double fare, distance, proposedFare;
			bool negotiated, reviewed;

			while (file >> rideID >> user >> driver >> source >> dest >> statusInt >> fare >> distance >> proposedFare >> negotiated >> vehicleType >> reviewed) {
				Ride ride(user, source, dest, vehicleType);
				if (!driver.empty() && driver != "null") {
					ride.assignDriver(driver);
				}
				ride.setFare(fare);
				ride.setDistance(distance);
				ride.setProposedFare(proposedFare);
				ride.setFareNegotiated(negotiated);
				ride.setReviewed(reviewed);

				if (statusInt == 2) {
					ride.completeRide(fare);
				} else if (statusInt == 3) {
					ride.cancelRide();
				}

				rides.push_back(ride);
			}
			file.close();
		}

		void saveRides() {
			ofstream file(RIDES_FILE);
			for (const auto& ride : rides) {
				file << ride.getRideID() << " " << ride.getUserUsername() << " "
				     << (ride.getDriverUsername().empty() ? "null" : ride.getDriverUsername()) << " "
				     << ride.getSource() << " " << ride.getDestination() << " "
				     << static_cast<int>(ride.getStatus()) << " "
				     << ride.getFare() << " " << ride.getDistance() << " "
				     << ride.getProposedFare() << " " << ride.isFareNegotiated() << " "
				     << ride.getVehicleType() << " " << ride.isReviewed() << endl;
			}
			file.close();
		}

		void loadReviews() {
			ifstream file(REVIEWS_FILE);
			if (!file) return;

			string reviewer, reviewee, comment, timestamp;
			int rating;

			while (getline(file, reviewer) && getline(file, reviewee) &&
			        file >> rating && file.ignore() && getline(file, comment) && getline(file, timestamp)) {
				Review review(reviewer, reviewee, rating, comment);
				reviews.push_back(review);

				User* user = findUserByUsername(reviewee);
				if (user) {
					user->addReview(review);
				}
			}
			file.close();
		}

		void saveReviews() {
			ofstream file(REVIEWS_FILE);
			for (const auto& review : reviews) {
				file << review.getReviewer() << "\n"
				     << review.getReviewee() << "\n"
				     << review.getRating() << "\n"
				     << review.getComment() << "\n"
				     << review.getTimestamp() << "\n";
			}
			file.close();
		}

		void run() {
			User* currentUser = nullptr;

			while (true) {
				displayHeader("RouteX - Intelligent Route and Ride Management System");

				if (!currentUser) {
					cout << "1. Register\n";
					cout << "2. Login\n";
					cout << "3. View Locations\n";
					cout << "4. Exit\n";
					cout << "Choose option: ";

					int choice;
					cin >> choice;

					switch (choice) {
						case 1:
							registerUser();
							break;
						case 2:
							currentUser = loginUser();
							break;
						case 3:
							displayHeader("KARACHI LOCATIONS");
							cityMap.displayLocations();
							waitForEnter();
							break;
						case 4:
							cout << "Thank you for using RouteX!\n";
							return;
						default:
							cout << "❌ Invalid choice!\n";
							waitForEnter();
					}
								}
								 else {
					cout << "Welcome, " << currentUser->getName() << "!\n\n";
				    
				    cout << "1. Book a Ride\n";
				    cout << "2. View My Rides\n";
				    cout << "3. Repeat Previous Ride \n";
				    cout << "4. View Locations (List)\n";
				    cout << "5. View Regional Maps\n";
				    cout << "6. Logout\n";
				
				    cout << "Choose option: ";
				    int choice;
				    cin >> choice;
				
				    switch (choice) {
				        case 1:
				            bookRide(currentUser);
				            break;
				        case 2:
				            viewRides(currentUser);
				            break;
				        case 3:
				            repeatPreviousRide(currentUser);
				            break;
				        case 4:
				            displayHeader("KARACHI LOCATIONS");
				            cityMap.displayLocations();
				            waitForEnter();
				            break;
				        case 5:
				            displayHeader("KARACHI REGIONAL MAPS");
				            cityMap.displayRegionalMap();
				            break;
				        case 6:
				            currentUser = nullptr;
				            cout << "Logged out successfully!\n";
				            waitForEnter();
				            break;
				        default:
				            cout << "Invalid choice!\n";
				            waitForEnter();
				    }
				}
			}
		}
};
void RouteXSystem::simulateRideProgress(Ride& ride) {
    displayHeader("RIDE IN PROGRESS");

    cout << "🚗 Ride Started! Tracking your journey...\n\n";

    vector<string> route = ride.getRoute();
    if (route.size() < 2) return;

    for (size_t i = 1; i < route.size(); i++) {
        ride.setCurrentLocation(route[i]);

        clearScreen();
        displayHeader("RIDE IN PROGRESS");
        
        cout << "📍 From: " << cityMap.getLocationName(route[i-1]) << " ➔ ";
        cout << "To: " << cityMap.getLocationName(route[i]) << "\n";
        cout << " Progress: " << i << "/" << route.size()-1 << " locations passed\n";

        if (i < route.size() - 1) {
            cout << "⏭  Next: " << cityMap.getLocationName(route[i+1]) << "\n";
        } else {
            cout << " Destination approaching...\n";
        }

        cout << "\n";

        animateCarMovementClean(cityMap.getLocationName(route[i-1]), cityMap.getLocationName(route[i]));

        if (i < route.size() - 1) {
            cout << "\n🚗 Continuing to next location...\n";
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
    }

    cout << "\n✅ Destination reached! Ride completed successfully!\n";
    waitForEnter();
}

void RouteXSystem::animateCarMovement(const string& fromLocation, const string& toLocation) {
    int screenWidth = 60;
    int carWidth = 20;
    int totalDistance = screenWidth - carWidth;
    int segmentDistance = totalDistance / 5;
    
    cout << " Moving: " << fromLocation << " → " << toLocation << "\n\n";
    
    int currentPosition = 0;
    
    for (int segment = 1; segment <= 5; segment++) {
        int targetPosition = segment * segmentDistance;
        
        while (currentPosition < targetPosition) {
            cout << "\r";
            for (int i = 0; i < currentPosition; i++) cout << " ";
            cout << "      ______";
            for (int i = currentPosition + 12; i < screenWidth; i++) cout << " ";
            
            cout << "\r";
            for (int i = 0; i < currentPosition; i++) cout << " ";
            cout << "  __/      \\_";
            for (int i = currentPosition + 18; i < screenWidth; i++) cout << " ";
            
            cout << "\r";
            for (int i = 0; i < currentPosition; i++) cout << " ";
            cout << " /_  _________  _\\";
            for (int i = currentPosition + 19; i < screenWidth; i++) cout << " ";
            
            cout << "\r";
            for (int i = 0; i < currentPosition; i++) cout << " ";
            cout << "   (O)       (O)";
            for (int i = currentPosition + 17; i < screenWidth; i++) cout << " ";
            
            cout << "\r";
            cout << "[";
            int progress = (currentPosition * 100) / totalDistance;
            int bars = (progress * 20) / 100;
            for (int j = 0; j < 20; j++) {
                if (j < bars) cout << "=";
                else cout << " ";
            }
            cout << "] " << progress << "%";
            
            cout << "\r";
            cout << fromLocation;
            int fromLocLength = fromLocation.length();
            int toLocLength = toLocation.length();
            int pathSpace = screenWidth - fromLocLength - toLocLength - 4;
            
            for (int j = 0; j < pathSpace; j++) {
                if (j == currentPosition - fromLocLength) {
                    cout << "🚗";
                } else if (j < currentPosition - fromLocLength) {
                    cout << "─";
                } else {
                    cout << " ";
                }
            }
            cout << "🚩 " << toLocation;
            
            currentPosition++;
            this_thread::sleep_for(chrono::milliseconds(80));
            
            if (currentPosition < targetPosition) {
                cout << "\033[6A";
            }
        }
        
        cout << "\n\n Ride " << segment << "/5 Completed! ";
        
        if (segment < 5) {
            cout << "Continuing...\n";
            this_thread::sleep_for(chrono::milliseconds(2000));
            cout << "\033[2A";
        } else {
            cout << "Reached: " << toLocation << " ✅\n";
        }
    }
    cout << "\n";
}

void RouteXSystem::animateCarMovementClean(const string& fromLocation, const string& toLocation) {
    int totalSegments = 5;
    int animationWidth = 50;
    
    cout << " Journey: " << fromLocation << " → " << toLocation << "\n";
    
    for (int segment = 1; segment <= totalSegments; segment++) {
        int percentage = (segment * 100) / totalSegments;
        
        cout << " Progress: " << percentage << "% Complete\n";
        cout << "Route: " << fromLocation;
        
        int segmentLength = animationWidth / totalSegments;
        int currentPos = segment * segmentLength;
        
        for (int i = 0; i < animationWidth; i++) {
            if (i < currentPos) {
                cout << "─";
            } else {
                cout << " ";
            }
        }
        
        cout << "🚩 " << toLocation;
        cout << " [" << segment << "/5]";
        
        if (segment < totalSegments) {
            cout << " Moving...";
            this_thread::sleep_for(chrono::milliseconds(2800));
            cout << "\n\n";
            
            cout << "\n";
            drawCar(currentPos);
            cout << "\n";
        } else {
            cout << " ✅ Arrived!";
            cout << "\n\n";
            
            cout << "\n";
            drawCar(currentPos);
            cout << "\n";
        }
    }
}

void RouteXSystem::drawCar(int position) {
    for (int i = 0; i < position; i++) cout << " ";
    cout << "      ______\n";
    
    for (int i = 0; i < position; i++) cout << " ";
    cout << "  __/      \\_\n";
    
    for (int i = 0; i < position; i++) cout << " ";
    cout << " /_  _________  _\\ \n";
    
    for (int i = 0; i < position; i++) cout << " ";
    cout << "   (O)       (O) \n";
}
int main() {
	SetConsoleOutputCP(65001);

	RouteXSystem system;
	system.run();
	return 0;
}

void pause(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void enableVTMode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

int getConsoleWidth() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(hOut, &csbi))
        return 80;

    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void printCentered(const string& s) {
    int width = getConsoleWidth();
    int leftPadding = max(0, (width - (int)s.length()) / 2);

    for (int i = 0; i < leftPadding; i++)
        cout << " ";

    cout << s << endl;
}

void animation(const string& s, const string& color, int delay, bool showBorder) {
    enableVTMode();

    if (showBorder) {
        string border(s.length() + 12, '=');
        printCentered(border);
        cout << "\n";
    }

    int leftPadding = max(0, (getConsoleWidth() - (int)s.length()) / 2);
    for (int i = 0; i < leftPadding; i++)
        cout << " ";

    for (char c : s) {
        cout << color << c << COLOR_RESET << flush;
        pause(delay);
    }

    if (showBorder) {
        cout << "\n\n";
        string border(s.length() + 12, '=');
        printCentered(border);
    }
    cout << endl;
}