#include "datatypes.h"
#include <iostream>
using namespace std;

/**
 * Returns the ASCII sum of all characters in a string
 * To be used as the indexing key of a string into the hash table,
 * e.g. k = sumStringChars(str)
*/
unsigned int sumStringChars(const string& str) 
{
    unsigned int sum = 0;
    for (unsigned int i=0; i<str.length(); ++i)
        sum += static_cast<unsigned int>(str[i]);
    return sum;
}


// TASK 1.1: VaccinationStatus default constructor
VaccinationStatus::VaccinationStatus(){
    // TODO
    for(int i = 0; i < VACCINE_TABLE_SIZE; i++)
    {
        vaccineHashTable[i] = "";
    }
    numVaccines = 0;
}

// TASK 1.2: VaccinationStatus::addVaccine(const string&)
// - If the table is not at least half empty, output the first error message and return.
// - Else, calculate k = sumStringChars(v) and hash(k) = k % VACCINE_TABLE_SIZE
//   Follow the quadratic probing formula: h(i) = [hash(k) + i^2] % VACCINE_TABLE_SIZE
//   to add the vaccine to the table.
//   If the vaccine already exists, output the second error message and return.
void VaccinationStatus::addVaccine(const string& v)
{
    if (numVaccines >= 0.5 * VACCINE_TABLE_SIZE) {
        cout << "This system does not support having taken more than " << numVaccines << " vaccines." << endl;
        return;
    }
    else{
        if (hasVaccine(v)) {
        cout << "This animal has already taken " << v << "." << endl;
        return;
        }
    // TODO: Quadratic probing
    unsigned int k = sumStringChars(v);
    unsigned int hash = k % VACCINE_TABLE_SIZE;
    int ind, cc = 0;
    do{
        ind = (hash + (cc * cc)) % VACCINE_TABLE_SIZE;
        cc += 1;
    }while(vaccineHashTable[ind] != "");

    // TODO: Quadratic probing
    vaccineHashTable[ind] = v;
    numVaccines++;
    return;
    }
}

// TASK 1.3: VaccinationStatus::hasVaccine(const string&) const
// Returns true if the vaccine exists in the table, false otherwise
bool VaccinationStatus::hasVaccine(const string& v) const
{
    // TODO
    for(unsigned int i = 0; i < VACCINE_TABLE_SIZE; i++){
        if (vaccineHashTable[i] == v){
            return true;
        }
    }   
    return false;
}


/**
 * Returns a value representing the current hash table
 * Used for comparing 2 animals with different vaccination status
*/
unsigned int VaccinationStatus::getTotalHashValue() const 
{
    unsigned int hash = 0;
    for (unsigned int i=0; i<VACCINE_TABLE_SIZE; ++i)
        hash += sumStringChars(vaccineHashTable[i]);
    return hash * numVaccines;
}
