#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
using namespace std;

//TODO     _______                  __   _                        _    _
//TODO    |_   __ \                |  ] (_)                      / |_ (_)
//TODO    | |__) |   ,--.    .--.| |  __   .--.   ,--.   .---.`| |-'__  _   __  .---.
//TODO    |  __ /   `'_\ : / /'`\' | [  |/ .'`\ \`'_\ : / /'`\]| | [  |[ \ [  ]/ /__\\
//TODO  _| |  \ \_ // | |,| \__/  |  | || \__. |// | |,| \__. | |, | | \ \/ / | \__.,
//TODO |____| |___|\'-;__/ '.__.;__][___]'.__.' \'-;__/'.___.'\__/[___] \__/   '.__.'

//TODO    ______   __                 __
//TODO   |_   _ \ [  |               [  |  _
//TODO   | |_) | | |  .--.   .---.  | | / ]
//TODO   |  __'. | |/ .'`\ \/ /'`\] | '' <
//TODO _| |__) || || \__. || \__.  | |`\ \
//TODO|_______/[___]'.__.' '.___.'[__|  \_]

//TODO    ______         __                   __          _
//TODO .' ___  |       [  |                 [  |        / |_
//TODO / .'   \_| ,--.   | |  .---.  __   _   | |  ,--. `| |-' .--.   _ .--.
//TODO | |       `'_\ :  | | / /'`\][  | | |  | | `'_\ : | | / .'`\ \[ `/'`\]
//TODO \ `.___.'\// | |, | | | \__.  | \_/ |, | | // | |,| |,| \__. | | |
//TODO `.____ .'\'-;__/[___]'.___.' '.__.'_/[___]\'-;__/\__/ '.__.' [___]



/* This program is designed to take a user-input block of radioactive/stable material, and a given amount of time
 * and the computer determines the composition of the material at that point in time. specifically, the percetnages of
 * specific isotopes in the material are given, and the length of time, and the program will return the percetnage
 * composition of the material.
 */


struct Nuclide {
    int pNumber, nNumber;
    string name;
    double halfLife;
    string halfLifeUnits;
    string decayMode;

};
struct UserIsotope {
    string name;
    double percentage;


};


vector<Nuclide> readNuclideTable() {
    vector<Nuclide> tableOfNuclides;

    ifstream IsotopeFile("C:\\Users\\heath\\CLionProjects\\FancyHalfLifeCalculator\\nndc_nudat_data.csv");

    if (!IsotopeFile.is_open()) {
        cerr << "Error opening the file: " << strerror(errno) << endl;
        return tableOfNuclides;
    }

    string line;
    while (getline(IsotopeFile, line)) {
        Nuclide isotope;
        string line;
        while (getline(IsotopeFile, line)) {
            Nuclide isotope;
            istringstream ss(line);
            string value;

            // Extract and convert pNumber
            getline(ss, value, ',');
            isotope.pNumber = stoi(value);

            // Extract and convert nNumber
            getline(ss, value, ',');
            isotope.nNumber = stoi(value);

            // Extract name
            getline(ss, isotope.name, ',');

            // Extract and convert halfLife
            getline(ss, value, ',');
            isotope.halfLife = stod(value);

            // Extract halfLifeUnits
            getline(ss, isotope.halfLifeUnits, ',');

            // Extract decayMode
            getline(ss, isotope.decayMode, ',');

            tableOfNuclides.push_back(isotope);
        }

        IsotopeFile.close();

        return tableOfNuclides;
    }
}



int searchTableWithName(string name, vector<Nuclide> TableOfNuclides) {       //general function to take an isotope name
    int counter = 0;                                                                // and returns it's row.
    while (true) {
        if (TableOfNuclides[counter].name == name) {               //iterates through entire vector, if name equals vector name
            return counter;                                        // return that row
        } else {
            counter = counter + 1;
        }

    }
}



int searchTableWithPAndNNumber(int pNumber, int nNumber, vector<Nuclide> TableOfNuclides) {
    int counter = 0;
    while(true) {
        if (TableOfNuclides[counter].pNumber == pNumber && TableOfNuclides[counter].nNumber == nNumber) {
            return counter;
        } else {
            counter = counter + 1;
        }
    }


}


vector<UserIsotope> getPercentagesOfIsotopes() {
    vector<UserIsotope> PercentageVector;
    cout << "How many different isotopes are in the material?";
    int numIsotopes;
    cin >> (numIsotopes);
    size_t i;
    for (i = 1; i <= numIsotopes; i++) {
        cout << "Please input an isotope and the percent composition (Isotope should be in the format 235U)\n";
        string name;
        cin >> (name);
        double percentage;
        cin >> (percentage);
        PercentageVector.push_back({name, percentage});
    }
    return PercentageVector;
}



string DetermineDaughterProduct(int spot, vector<Nuclide> TableOfNuclides){
    int originalPNumber = TableOfNuclides[spot].pNumber;              //Take p and n values from original nuclide
    int originalNNumber = TableOfNuclides[spot].nNumber;
    int DaughterProductPNumber;
    int DaughterProductNNumber;

    if (TableOfNuclides[spot].decayMode == "B-") {                     //Converts associate decay to
                                                                       // daughter product in terms of p and n
        DaughterProductPNumber = originalPNumber + 1;
        DaughterProductNNumber = originalNNumber - 1;

    }
    else if (TableOfNuclides[spot].decayMode == "EC") {
        DaughterProductPNumber = originalPNumber - 1;
        DaughterProductNNumber = originalNNumber + 1;

    }
    else if (TableOfNuclides[spot].decayMode == "A") {
        DaughterProductPNumber = originalPNumber -2 ;
        DaughterProductNNumber = originalNNumber -2 ;

    }
    else {

    }

    //Function returns the position of the duaghter product in the table of nuclides.
int rowSpot = searchTableWithPAndNNumber(DaughterProductPNumber,DaughterProductNNumber,TableOfNuclides);
    return TableOfNuclides[rowSpot].name;

}



vector<string> decayChains(string iso,vector<Nuclide> TableOfNuclides){

    vector<string> decayChainList;
    decayChainList.push_back(iso);                //initial state in the decayChain is the original value
        while(true) {
            int rowSpot = searchTableWithName(iso, TableOfNuclides);       //determines daughter product
                string DaughterProduct = DetermineDaughterProduct(rowSpot, TableOfNuclides);
                decayChainList.push_back(DaughterProduct);   //adds daughter product to vector
                iso = DaughterProduct;                              // prepares for next loop by setting iso to daughter.
                int RunningOutOfCleverVariableNames = searchTableWithName(iso, TableOfNuclides);
                if ( TableOfNuclides[RunningOutOfCleverVariableNames].decayMode == "0") {      //if daughter product's half-life = 0
                    //The decayList is complete.
                    return decayChainList;
                }

            }

        }






vector<UserIsotope> computeComposition(vector<UserIsotope> iso, double t, vector<Nuclide> TableOfNuclides) {   //TODO Complete ComputeComposition

    vector<UserIsotope> PercentCompositionAfter;
    size_t i;
    for (i=1; i <= iso.size(); i++) {

        vector<string> decayChain = decayChains(iso[i].name, TableOfNuclides);

        size_t j;
        for(j=1; j <= decayChain.size(); j++){
            int rowSpot = searchTableWithName(decayChain[j], TableOfNuclides);
            double halfLife = TableOfNuclides[rowSpot].halfLife;

            // After case one and case two are defined each fomrula takes the form of:

            //integral from 0 to time ((d/dt of Pervious formula/e^-lambda(tconstant-t))-d/dt of Previous formula)e^lambdacurrent(tconst-t) dt


            //if (j == 1) {
              // PercentCompositionAfter.push_back({decayChain[j],iso[i].percentage * e^(-halfLife * t)});
            //}
            //else if (j == 2) {
            //    PercentCompositionAfter.push_back({decayChain[j],integral(0,t)(d/dt(N0)e^(-halflife(t(const)-t)))});
            //}

        }



    }
}


void printComp(const vector<UserIsotope>& iso, double t){
    int size = iso.size();
    cout << "Percent Composition of the material after " << t << " days.\n";
    size_t i;
    for(i=0; i < size; i++){
        cout <<"ISOTOPE " << i+1 <<":\n";
        cout << iso[i].name << ": " << iso[i].percentage << "%\n";
    }
}


int main() {

    vector<Nuclide> TableOfNuclides = readNuclideTable();      //Opens CSV file to create vector containing all Info
                                                                //Needed regarding computation

    vector<UserIsotope> Percentages = getPercentagesOfIsotopes();   //Asks user for a list of Isotopes and the percent
                                                                    // composition and puts it into a vector

    double time;

    cout <<"How much time will pass (in days)? ";                   //Asks for time

    cin >> (time);

    vector<UserIsotope> PercentComp = computeComposition(Percentages, time, TableOfNuclides);//Computes Compostion

    printComp(PercentComp, time);    //Prints Composition

    return 0;
}
