#include <iostream>
#include <set>
#include <algorithm>  
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class OnD {         
private:
    char* origin;
    char* destination;
    set<long> prices;

public:
    // Constructor
    OnD(char* o, char* d) : origin(o), destination(d) {}

    // Getter and setter
    char* getOrigin()
    {
        return origin;
    }

    char* getDestination()
    {
        return destination;
    }

    set<long>& getPrices() 
    {
        return prices;
    }
    
    void setPrices(set<long> p) 
    {
        prices = p;
    }

    // Compute minimal prices 
    long minimalPrice() 
    {
        return *min_element(prices.begin(), prices.end());
    }

    // Compute maximum prices
    long maximalPrice() 
    {
        return *max_element(prices.begin(), prices.end());
    }

    // Compute average prices
    long averagePrice()
    {
        long computePrice = 0;
        for (long price : prices) {
            computePrice += price;
        }
        return computePrice / prices.size();
    }

    // Export a line and write it in a file if specified
    vector<string> exportLine(string fileName)
    {
        // Prepare data to put in CSV
        double minPrice = minimalPrice() / pow(10, 2);
        double maxPrice = maximalPrice() / pow(10, 2);
        double avPrice = averagePrice() / pow(10, 2);

        // Add data to return
        vector<string> data;
        data.push_back(origin);
        data.push_back(destination);
        data.push_back(to_string(minPrice));
        data.push_back(to_string(maxPrice));
        data.push_back(to_string(avPrice));

        // Write in file if specified
        if (fileName != "none") {
            // Open file and add new line
            ofstream myFile(fileName, ios::app);
            // Send data to the stream
            myFile << origin << ";" << destination << ";" << minPrice << ";" << maxPrice << ";" << avPrice << "\n";
            // Close the file
            myFile.close();
        }

        return data;
    }
};

class FlightNbr {
private:
    char* flightId;
    OnD* ond;

public:
    // Constructor
    FlightNbr(char* f, OnD* o) : flightId(f), ond(o) {}
    
    // Getters
    char* getFlightId() {
        return flightId;
    }

    OnD* getOnd() {
        return ond;
    }
};

// Export in a file All flights with their corresponding Origine Destination informations
void exportFlightsList(vector<FlightNbr> flightNbrList) {
    ofstream myFile("flights.csv");
    // Send data to the stream
    myFile << "NumeroDeVol,Origine,Destination,MinimumFare,MaximumFare,MoyenneFare \n";

    for (FlightNbr flightNbr : flightNbrList) {
        myFile << flightNbr.getFlightId() << ",";
        for (string exportOnd : flightNbr.getOnd()->exportLine("none")) {
            myFile << exportOnd << ",";
        }
        myFile << "\n";
                
    }
    myFile.close();
}

int main()
{
    //Create first file and overwrite if exist
    string fileName = "OrigineDestinationComputed.csv";
    ofstream myFile(fileName);
    // Send data to the stream
    myFile << "Origine;Destination;TarifMinimum;TarifMaximum;TarifMoyen \n";

    myFile.close();

    // Load all data
    string fname = "OrigineDestinationTarif.csv";

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ';'))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else {
        cout << "Could not open the file\n";
    }

    // Sort data and initialize OnD List
    vector<OnD> ondList;

    for (int i = 1; i < content.size(); i++)
    {
        // Initialise values
        char* origin = new char[4];
        sprintf_s(origin, 4, "%s", content[i][0].c_str());

        char* destination = new char[4];
        sprintf_s(destination, 4, "%s", content[i][1].c_str());

        double priceWithFloat = stod(content[i][2]) * pow(10, 2);
        long price = (long) priceWithFloat;

        // Check if does exist
        bool exist = false;
        for (OnD& ond : ondList)
        {
            if (*ond.getOrigin() == *origin  && *ond.getDestination() == *destination)
            {
                exist = true;
                ond.getPrices().insert(price);
                break;
            }
        }

        // Create if doesn't exist
        if (!exist)
        {
            OnD ond(origin, destination);
            set<long> prices;
            prices.insert(price);
            ond.setPrices(prices);
            ondList.push_back(ond);
        }
    }

    for (OnD ond : ondList)
    {
        ond.exportLine(fileName);
    }

    // Create random flight numbers
    vector<FlightNbr> flightNbrList;

    srand(time(nullptr));

    for (int i = 0; i < 20; i++) {
        int randomIndex = rand() % ondList.size();

        int randomId = rand() % 9999;
        char* randomName = new char[7];
        sprintf_s(randomName, 7, "AF%04d", randomId);

        FlightNbr flightNbr(randomName, &ondList[randomIndex]);
        flightNbrList.push_back(flightNbr);
    }

    exportFlightsList(flightNbrList);

    return 0;
}

