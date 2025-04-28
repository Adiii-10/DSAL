#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
using namespace std;

struct Employee {
    int empID;
    string name;
    string designation;
    double salary;
    bool isDeleted;
};

const string dataFilename = "employee.txt"; // File name
unordered_map<int, Employee> employeeMap;   // Employee database (in memory)

void loadEmployees();
void saveEmployees();
void addEmployee();
void deleteEmployee();
void displayEmployee();

int main() {
    loadEmployees();
    int choice;
    do {
        cout << "\n--- Employee Information System ---" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Delete Employee" << endl;
        cout << "3. Display Employee" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                deleteEmployee();
                break;
            case 3:
                displayEmployee();
                break;
            case 4:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while(choice != 4);

    return 0;
}

void loadEmployees() {
    employeeMap.clear();
    ifstream fin(dataFilename);
    if (!fin) {
        // File does not exist yet
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        Employee emp;

        if (!getline(ss, token, ',')) continue;
        try {
            emp.empID = stoi(token);
        } catch (const invalid_argument &e) {
            cerr << "Invalid empID: " << token << endl;
            continue;
        }

        if (!getline(ss, token, ',')) continue;
        emp.name = token;

        if (!getline(ss, token, ',')) continue;
        emp.designation = token;

        if (!getline(ss, token, ',')) continue;
        try {
            emp.salary = stod(token);
        } catch (const invalid_argument &e) {
            cerr << "Invalid salary: " << token << endl;
            continue;
        }

        if (!getline(ss, token, ',')) continue;
        emp.isDeleted = (token == "1" || token == "true");

        if (!emp.isDeleted)
            employeeMap[emp.empID] = emp;
    }
    fin.close();
}

void saveEmployees() {
    ofstream fout(dataFilename, ios::trunc);
    if (!fout) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    for (const auto &pair : employeeMap) {
        const Employee &emp = pair.second;
        fout << emp.empID << ","
             << emp.name << ","
             << emp.designation << ","
             << emp.salary << ","
             << (emp.isDeleted ? "1" : "0") << "\n";
    }
    fout.close();
}

void addEmployee() {
    Employee emp;
    emp.isDeleted = false;

    cout << "Enter Employee ID: ";
    cin >> emp.empID;
    cin.ignore();

    if (employeeMap.find(emp.empID) != employeeMap.end()) {
        cout << "Employee with ID " << emp.empID << " already exists." << endl;
        return;
    }

    cout << "Enter Name: ";
    getline(cin, emp.name);

    cout << "Enter Designation: ";
    getline(cin, emp.designation);

    cout << "Enter Salary: ";
    while (!(cin >> emp.salary) || emp.salary < 0) {
        cout << "Invalid salary. Please enter again: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    cin.ignore();

    employeeMap[emp.empID] = emp;
    saveEmployees();
    cout << "Employee added successfully." << endl;
}

void deleteEmployee() {
    int id;
    cout << "Enter Employee ID to delete: ";
    cin >> id;
    cin.ignore();

    auto it = employeeMap.find(id);
    if (it == employeeMap.end()) {
        cout << "Employee with ID " << id << " does not exist." << endl;
        return;
    }

    it->second.isDeleted = true; // Just mark deleted
    saveEmployees();
    cout << "Employee with ID " << id << " deleted successfully." << endl;
}

void displayEmployee() {
    int id;
    cout << "Enter Employee ID to display: ";
    cin >> id;
    cin.ignore();

    auto it = employeeMap.find(id);
    if (it == employeeMap.end()) {
        cout << "Employee with ID " << id << " does not exist." << endl;
        return;
    }

    const Employee &emp = it->second;
    cout << "\nEmployee Details:" << endl;
    cout << "Employee ID: " << emp.empID << endl;
    cout << "Name: " << emp.name << endl;
    cout << "Designation: " << emp.designation << endl;
    cout << "Salary: " << emp.salary << endl;
}