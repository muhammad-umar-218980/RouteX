# RouteX – Ride Allocation System

RouteX is an intelligent routing and ride-allocation system designed as a simulation of modern ride-sharing applications. The city is represented as a weighted graph with 50 key locations in Karachi. The system integrates route optimization, ride management, user and driver handling, fare calculation, reviews, and persistent data storage.

---

## 📌 Project Description

RouteX models a real-world ride-hailing platform using Data Structures & Algorithms concepts. It uses a graph-based structure to represent the city, Dijkstra’s algorithm for route optimization, vectors and hash maps for data management, and an ASCII-based interface to visualize the city and active rides.

---

## 🚀 Core Components

### 1. Graph-Based City Mapping
- **Purpose:** Model Karachi as a connected graph of 50 locations.  
- **Data Structures Used:**
  - `unordered_map<string, vector<pair<string, double>>> adjList` – adjacency list
  - `unordered_map<string, string> locationNames` – location ID to readable name mapping
- **Justification:**  
  Hash maps provide fast O(1) average access for efficient route calculations and location lookups.

---

### 2. Ride Management System
- **Purpose:** Manage the full lifecycle of rides — booking to completion.  
- **Data Structures:**
  - `vector<Ride> rides`
  - `enum class RideStatus { PENDING, ONGOING, COMPLETED, CANCELLED }`
  - `vector<string> route`
  - `static int nextRideID` for unique ride IDs
- **Justification:**  
  Vectors offer efficient dynamic storage; enums ensure strict ride-state integrity.

---

### 3. User Management & Social Features
- **Purpose:** Handle user accounts, ratings, and reviews.  
- **Data Structures:**
  - `vector<User> users`
  - `vector<Review> reviews`
  - `double rating` for average rating tracking
- **Justification:**  
  OOP keeps user data modular, secure, and logically structured.

---

### 4. Intelligent Driver Allocation
- **Purpose:** Automatically allocate the nearest appropriate driver.  
- **Data Structures:**
  - `vector<string> availableDrivers`
  - `unordered_map<string, string> driverVehicles`
  - `unordered_map<string, string> driverLocations`
- **Justification:**  
  Hash maps ensure efficient spatial lookup for nearest-driver matching.

---

### 5. Route Optimization Engine
- **Purpose:** Calculate shortest paths using Dijkstra’s Algorithm.  
- **Notes:**  
  Supports weighted edges to simulate real city distances.

---

### 6. Visual Mapping System
- **Purpose:** Display an ASCII-based map of Karachi.  
- **Features:**
  - Regional breakdown (South, Central, East, West, Coastal)
  - 1–50 location numbering
  - Visual connections showing edges  
- **Data Structure:**  
  Hierarchical grouping of regions and adjacency mapping.

---

### 7. Ride History & Rebooking
- **Purpose:** Maintain a smart ride history system.  
- **Features:**
  - Automatically store completed rides
  - One-click repeat booking
  - Saved fares and previous route information
- **Justification:**  
  Enhances user convenience and booking efficiency.

---

### 8. Real-Time Ride Simulation
- **Purpose:** Visually simulate ride progress.  
- **Features:**
  - ASCII car animation
  - Percentage progress tracking
  - Segment-based travel breakdown
  - Step-by-step location transitions

---

### 9. Fare Calculation & Negotiation
- **Purpose:** Provide dynamic and flexible pricing.  
- **Algorithm:**  
  Base fare = vehicle rate + distance multiplier  
  Negotiation allowed within 5% tolerance
- **Formula:**
```text
fare = baseFare + (distance × perKmRate)
```

---

### 10. Persistent Data Management
- **Purpose:** Store key data across sessions using text files.  
- **Files Used:**
  - `users.txt` – user profiles
  - `rides.txt` – ride history
  - `reviews.txt` – feedback and ratings
  - `drivers.txt` – driver availability & locations

---

## 📂 Tech Stack
- C++
- Data Structures & Algorithms
- Graph Theory
- File Handling
- Object-Oriented Programming

---

## 📸 Interface Overview
- ASCII-based city map
- Visual ride simulation
- Route display and path breakdown

---

## 📘 Academic Context
This project was developed as part of the Data Structures and Algorithms (DSA) – CT-159 course in the BSCS/CSIT program at NED University of Engineering & Technology.
