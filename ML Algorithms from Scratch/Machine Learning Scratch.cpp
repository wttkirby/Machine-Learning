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
	vector<double> numOne
	int numEntry = 0;

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
				}
				else {
					personIDTest.at(numEntry) = stod(person_in);
					pclassTest.at(numEntry) = stod(pclass_in);
					survivedTest.at(numEntry) = stod(survived_in);
					sexTest.at(numEntry) = stod(sex_in);
					ageTest.at(numEntry) = stod(age_in);
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
		
		cout << numEntry << endl;
		inFS.close();

	}
}

