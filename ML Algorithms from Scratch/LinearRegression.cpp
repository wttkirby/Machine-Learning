// Machine Learning Scratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


int getNumLines(string);						// Counts and returns the number of lines in a given file stream
double calcAccuracy(int, int, int, int);		// Calculates and returns the accuracy from the given TP, TN, FP, and FN
double calcSensitivity(int, int);				// Calculates and returns the sensitivity from the given TP and FN
double calcSpecificity(int, int);				// Calculates and returns the specificity from the given TN and FN

void logisticRegression();						// Performs logistic regression on the given train and test sets
void naiveBayes();								// Performs naive bayes on the given train and test sets


int main()
{
	ifstream inFS;
	string line;

	// Opening the titanic project data file
	cout << "Opening file titanic_project.csv" << endl << endl;

	inFS.open("titanic_project.csv");

	// If we couldn't open the file
	if (!inFS.is_open())
	{
		cout << "Couldn't open the file :(" << endl;
	}	// File successfully opened
	else
	{
		// Get the file size (number of lines)
		const int fileSize = getNumLines("titanic_project.csv");
		//cout << "File number of lines: " << fileSize << endl;

		// Initialize our vectors, strings, and variables for getting our data
		string person_in, pclass_in, survived_in, sex_in, age_in; // Used to temporarily hold values from getline
		vector<double> personIDTrain(fileSize-1);
		vector<double> pclassTrain(fileSize-1);
		vector<double> survivedTrain(fileSize-1);
		vector<double> sexTrain(fileSize-1);
		vector<double> ageTrain(fileSize-1);
		vector<double> personIDTest(fileSize-1);
		vector<double> pclassTest(fileSize-1);
		vector<double> survivedTest(fileSize-1);
		vector<double> sexTest(fileSize-1);
		vector<double> ageTest(fileSize-1);
		vector<double> numOneTrain(fileSize-1);
		vector<double> numOneTest(fileSize-1);		// Used for multiplying our vectors later

		// Used to track the current entry in our while loop below
		int numEntry = 0;	
		int numEntry2 = 0;	

		// Read in the first line to skip it (just headers)
		getline(inFS, line);
		//cout << "heading: " << line << endl;

		// While there is more to read from the file
		while (inFS.good())
		{
			// Read in the input seperated by commas
			getline(inFS, person_in, ',');		// Get the person ID input (int)
			getline(inFS, pclass_in, ',');		// Get the person's class input (int, 1 to 3)
			getline(inFS, survived_in, ',');	// Get the survived input (int, binary)
			getline(inFS, sex_in, ',');			// Get the person's sex input (int, binary)
			getline(inFS, age_in, '\n');		// Get the person's age input (int)

			// If numEntry is in range of the file size
			if (numEntry < fileSize - 1 ) { 
				// If the data entry is in the first 80% of the data set, add it to train vectors
				if (numEntry < ((fileSize - 1) * 80) / 100) {
					personIDTrain.at(numEntry) = stod(person_in);		// Add the person's ID entry to the person ID train vector
					pclassTrain.at(numEntry) = stod(pclass_in);			// Add the person's class entry to the person class train vector
					survivedTrain.at(numEntry) = stod(survived_in);		// Add the survived entry to the survived train vector
					sexTrain.at(numEntry) = stod(sex_in);				// Add the person's sex entry to the sex train vector
					ageTrain.at(numEntry) = stod(age_in);				// Add the person's age entry to the age train vector
					numOneTrain.at(numEntry) = 1;						// Add the 1 for the number one vector
				}
				else {	// Otherwise it is in the last 20% of the data set, add it to the test vectors
					personIDTest.at(numEntry2) = stod(person_in);		// Add the person's ID entry to the person ID test vector
					pclassTest.at(numEntry2) = stod(pclass_in);			// Add the person's class entry to the person class test vector
					survivedTest.at(numEntry2) = stod(survived_in);		// Add the survived entry to the survived test vector
					sexTest.at(numEntry2) = stod(sex_in);				// Add the person's sex entry to the sex test vector
					ageTest.at(numEntry2) = stod(age_in);				// Add the person's age entry to the age test vector
					numEntry2++;										// Increment the numEntry 2 (used for test vectors index)			
				}

			}

			numEntry++;		// Increment the numEntry (used for the train vectors index)
		}
		

		cout << "Closing file titanic_project.csv" << endl << endl;
		inFS.close();

	}
}

// Counts and returns the number of lines in a given file stream
int getNumLines(string filename)
{
	ifstream inFile;		// The input stream to read the given file
	inFile.open(filename);	// Open the file

	// If we couldn't open the file
	if (!inFile.is_open()) {
		cout << "Couldn't open file: " << filename << endl;
		return 0;	// Return number of lines as 0

	}	// Otherwise we could
	else {
		int numOfLines = 0;		// The number of lines
		string line;			// The current line of the file read (won't be used)

		// While we can read lines from the file, count it as a line
		while(getline(inFile, line)) {
			numOfLines++;	// Add one to the number of lines
		}

		// Return the number of lines found
		return numOfLines;
	}
}

/*
	This function computes the dot product of a vector matrix with a weight vector and returns the product.
	It does this by multiplying the value at matrix[0][i] (or the equivalent of it using vectors
	(matrix[0][0].at(i))) with the weights vector at 0, and then adds it with the product of
	matrix[1][i] (or the equivalent of it using vectors (matrix[0][1].at(i))) with the weights vector at 1.
	This returns a vector or a nx1 matrix holding the ending matrix we need for calculations.
*/
vector<double> dotProd(vector<double> sexMatrix[][2], vector<double> weights, int size) {

	vector<double> answer(size);

	for (int i = 0; i < size; i++) {
		answer.at(i) = (sexMatrix[0][0].at(i) * weights.at(0) + (sexMatrix[0][1].at(i) * weights.at(1)));
	}

	return answer;
}

// Calculates and returns the accuracy from the given TP, TN, FP, and FN
double calcAccuracy(int TP, int TN, int FP, int FN) {
	return ( (TP + TN) / (TP + FP + TN + FN) );
}		

// Calculates and returns the sensitivity from the given TP and FN
double calcSensitivity(int TP, int FN) {
	return ( (TP) / (TP + FN) );
}				

// Calculates and returns the specificity from the given TN and FN
double calcSpecificity(int TN, int FN) {
	return ( (TN) / (TN + FN) );
}				

// Performs logistic regression on the given train and test sets
void logisticRegression();	

// Performs native bayes on the given train and test sets
void naiveBayes();