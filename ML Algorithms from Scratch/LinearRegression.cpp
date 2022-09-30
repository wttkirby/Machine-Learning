// Machine Learning Scratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int getNumLines(ifstream);	// Counts and returns the number of lines in a given file stream
vector<double> dotProd(vector<double> sexMatrix[][2], vector<double> weights, int size);
double calcAccuracy(int, int, int, int);		// Calculates and returns the accuracy from the given TP, TN, FP, and FN
double calcSensitivity(int, int);				// Calculates and returns the sensitivity from the given TP and FN
double calcSpecificity(int, int);				// Calculates and returns the specificity from the given TN and 

void logisticRegression();		// Performs logistic regression on the given train and test sets
void naiveBayes();


int main()
{
	ifstream inFS;
	string line;
	string person_in, pclass_in, survived_in, sex_in, age_in;
	const int MAX_LEN = 10000;
	vector<double> personIDTrain(MAX_LEN);
	vector<double> pclassTrain(MAX_LEN);
	vector<double> survivedTrain(MAX_LEN);
	vector<double> sexTrain(MAX_LEN);
	vector<double> ageTrain(MAX_LEN);
	vector<double> personIDTest(MAX_LEN);
	vector<double> pclassTest(MAX_LEN);
	vector<double> survivedTest(MAX_LEN);
	vector<double> sexTest(MAX_LEN);
	vector<double> ageTest(MAX_LEN);
	vector<double> numOneTrain(MAX_LEN);
	vector<double> numOneTest(MAX_LEN);
	int numEntry = 0;
	int numEntry2 = 0;

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
		//cout << "Reading line 1" << endl;
		getline(inFS, line);

		//cout << "heading: " << line << endl;

		// While there is more to read from the file
		while (inFS.good())
		{
			getline(inFS, person_in, ',');
			getline(inFS, pclass_in, ',');
			getline(inFS, survived_in, ',');
			getline(inFS, sex_in, ',');
			getline(inFS, age_in, '\n');

			if (numEntry < 1046) {
				if (numEntry < 800) {
					personIDTrain.at(numEntry) = stod(person_in);
					pclassTrain.at(numEntry) = stod(pclass_in);
					survivedTrain.at(numEntry) = stod(survived_in);
					sexTrain.at(numEntry) = stod(sex_in);
					ageTrain.at(numEntry) = stod(age_in);
					numOneTrain.at(numEntry) = 1;
				}
				else {
					personIDTest.at(numEntry2) = stod(person_in);
					pclassTest.at(numEntry2) = stod(pclass_in);
					survivedTest.at(numEntry2) = stod(survived_in);
					sexTest.at(numEntry2) = stod(sex_in);
					ageTest.at(numEntry2) = stod(age_in);
					cout << "numEntry 2: " << numEntry2 << endl;
					numEntry2++;
				}

			}

			numEntry++;
		}
		
		personIDTrain.resize(800);
		personIDTest.resize((numEntry - 800));
		pclassTest.resize((numEntry - 800));
		pclassTrain.resize(800);
		survivedTest.resize((numEntry - 800));
		survivedTrain.resize(800);
		sexTest.resize((numEntry - 800));
		sexTrain.resize(800);
		ageTest.resize((numEntry - 800));
		ageTrain.resize(800);
		
		vector <double> matrixTrainSex[800][2];


		cout << "Number of Rows: " << numEntry << endl;
		inFS.close();

	}
}

// Counts and returns the number of lines in a given file stream
int getNumLines(ifstream file)
{
	int numOfLines = 0;		// The number of lines
	string line;			// The current line of the file read (won't be used)
	// While we can read lines from the file, count it as a line
	while(getline(file, line)) {
		numOfLines++;	// Add one to the number of lines
	}

	// Return the number of lines found
	return numOfLines;
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
double calcAccuracy(int, int, int, int);		// Calculates and returns the accuracy from the given TP, TN, FP, and FN
double calcSensitivity(int, int);				// Calculates and returns the sensitivity from the given TP and FN
double calcSpecificity(int, int);				// Calculates and returns the specificity from the given TN and 

void logisticRegression();		// Performs logistic regression on the given train and test sets
void naiveBayes();