// Machine Learning Scratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

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

	cout << "Opening file titanic_project.csv" << endl << endl;

	inFS.open("titanic_project.csv");

	if (!inFS.is_open())
	{
		cout << "Couldn't open the file :(" << endl;
	}
	else
	{
		//cout << "Reading line 1" << endl;
		getline(inFS, line);

		//cout << "heading: " << line << endl;

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
					//Made numEntry2 so that it starts inputting data at 0 instead of 800

					personIDTest.at(numEntry2) = stod(person_in);
					pclassTest.at(numEntry2) = stod(pclass_in);
					survivedTest.at(numEntry2) = stod(survived_in);
					sexTest.at(numEntry2) = stod(sex_in);
					ageTest.at(numEntry2) = stod(age_in);
					numOneTest.at(numEntry2) = 1;
					numEntry2++;
				}

			}

			numEntry++;
		}

		personIDTrain.resize(800);
		personIDTest.resize(numEntry2);
		pclassTest.resize(numEntry2);
		pclassTrain.resize(800);
		survivedTest.resize(numEntry2);
		survivedTrain.resize(800);
		sexTest.resize(numEntry2);
		sexTrain.resize(800);
		ageTest.resize(numEntry2);
		ageTrain.resize(800);
		numOneTrain.resize(800);
		numOneTest.resize(numEntry2);

		vector <double> matrixTrainSex[1][2];
		matrixTrainSex[0][0] = numOneTrain;
		matrixTrainSex[0][1] = sexTrain;

		vector <double> matrixTestSex[1][2];
		matrixTrainSex[0][0] = numOneTest;
		matrixTrainSex[0][1] = sexTest;


		cout << numEntry << endl;
		inFS.close();

	}
}

