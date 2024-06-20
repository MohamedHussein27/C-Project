#include <iostream>
#include <string.h>

using namespace std;

// Function to calculate total resistance for series connections
// Takes a string describing the series connection
double series(string x) {
    double RPF = 0; // Total parallel resistance of series components
    double RS = 0;  // Total series resistance
    double RPFS = 0; // Sum of series resistances
    double R; // Individual resistance value

    // Loop through the string to find series resistances
    for (int i = 1; i < x.length(); i++) {
        if (x[i] == 'S') {
            // Inner loop to sum up resistances until 'e' is encountered
            for (i; i < x.length(); i++) {
                if (x[i] == 'e')
                    break;
                if (x[i] == ' ') {
                    // Extract resistance value from the string
                    for (int j = 1; j < x.length(); j++) {
                        if (x[i + j] == 'e')
                            break;
                        if (x[i + j] != ' ')
                            continue;
                        if (x[i + j] == ' ') {
                            R = stof(x.substr(i + 1, j - 1));
                            RS = RS + R;
                            break;
                        }
                    }
                }
            }
            RPF = RPF + 1 / RS; // Add reciprocal of series resistance for parallel calculation
            RPFS = RPFS + RS; // Sum of series resistances
            RS = 0; // Reset series resistance
        }
    }

    // Return the calculated resistance based on the initial configuration
    if (x[0] == 'P')
        return RPF; // Return parallel resistance for a series block
    else if (x[0] == 'S')
        return RPFS; // Return sum of series resistances
}

// Function to handle parallel blocks that contain series resistances
// Takes a string describing the parallel connection with inner series blocks
double parallel_series(string x) {
    double RPP = 0; // Parallel resistance
    double R; // Individual resistance value

    // Loop through the string to find resistances
    for (int i = 1; i < x.length(); i++) {
        if (x[i] == 'S') {
            // Skip over series block
            while (x[i] != 'e')
                i++;
        }
        if (x[i] == ' ') {
            // Extract and sum parallel resistances
            for (int j = 1; j < x.length(); j++) {
                if (x[i + j] == 'S' || x[i + j] == 'P')
                    break;
                if (x[i + j] != ' ')
                    continue;
                if (x[i + j] == ' ') {
                    R = stof(x.substr(i + 1, j - 1));
                    RPP = RPP + 1 / R;
                    break;
                }
            }
        }
    }
    return RPP + series(x); // Return total resistance including series components
}

// Function to calculate total resistance for parallel connections
// Takes a string describing the parallel connection
double parallel(string x) {
    double RSF = 0; // Sum of parallel resistances
    double RP = 0;  // Total parallel resistance
    double RSFP = 0; // Parallel resistance of parallel components
    double R; // Individual resistance value

    // Loop through the string to find parallel resistances
    for (int i = 1; i < x.length(); i++) {
        if (x[i] == 'P') {
            // Inner loop to sum up resistances until 'e' is encountered
            for (i; i < x.length(); i++) {
                if (x[i] == 'e')
                    break;
                if (x[i] == ' ') {
                    // Extract resistance value from the string
                    for (int j = 1; j < x.length(); j++) {
                        if (x[i + j] == 'e')
                            break;
                        if (x[i + j] != ' ')
                            continue;
                        if (x[i + j] == ' ') {
                            R = stof(x.substr(i + 1, j - 1));
                            RP = RP + 1 / R;
                            break;
                        }
                    }
                }
            }
            RSF = RSF + 1 / RP; // Add reciprocal of parallel resistance for series calculation
            RSFP = RSFP + RP; // Sum of parallel resistances
            RP = 0; // Reset parallel resistance
        }
    }

    // Return the calculated resistance based on the initial configuration
    if (x[0] == 'S')
        return RSF; // Return series resistance for a parallel block
    else if (x[0] == 'P')
        return RSFP; // Return sum of parallel resistances
}

// Function to handle series blocks that contain parallel resistances
// Takes a string describing the series connection with inner parallel blocks
double series_parallel(string x) {
    double RSS = 0; // Series resistance
    double R; // Individual resistance value

    // Loop through the string to find resistances
    for (int i = 1; i < x.length(); i++) {
        if (x[i] == 'P') {
            // Skip over parallel block
            while (x[i] != 'e')
                i++;
        }
        if (x[i] == ' ') {
            // Extract and sum series resistances
            for (int j = 1; j < x.length(); j++) {
                if (x[i + j] == 'S' || x[i + j] == 'P')
                    break;
                if (x[i + j] != ' ')
                    continue;
                if (x[i + j] == ' ') {
                    R = stof(x.substr(i + 1, j - 1));
                    RSS = RSS + R;
                    break;
                }
            }
        }
    }
    return RSS + parallel(x); // Return total resistance including parallel components
}

int main() {
    string x; // Circuit description input by the user
    double V; // Voltage input by the user

    // Prompt user for input
    cout << "Please enter the type of connection, followed by the values of the resistances, and put the letter (E) when you are finished" << endl
         << "Use the letter (P) for parallel and the letter (S) for series" << endl;
    getline(cin, x);
    cout << "Please enter the value of the voltage" << endl;
    cin >> V;

    // Validate input string
    for (int i = 0; i < x.length(); i++) {
        if (!isdigit(x[i]) && x[i] != ' ' && x[i] != '.' && x[i] != 'S' && x[i] != 'P' && x[i] != 'e' && x[i] != 'E') {
            cout << "Wrong circuit description" << endl;
            return 0;
        }
    }

    // Determine total resistance and current based on configuration
    if (x[0] == 'P') {
        // For parallel initial configuration
        double totalResistance = 1 / parallel_series(x); // Total resistance
        cout << "The value of the total resistance is: " << totalResistance << endl;
        cout << "The value of the current is: " << V / totalResistance << endl; // Current calculation
    }
    if (x[0] == 'S') {
        // For series initial configuration
        double totalResistance = series_parallel(x); // Total resistance
        cout << "The value of the total resistance is: " << totalResistance << endl;
        cout << "The value of the current is: " << V / totalResistance << endl; // Current calculation
    }
}
