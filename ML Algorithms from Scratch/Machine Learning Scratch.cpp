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
	vector<double> personID(MAX_LEN);
	vector<double> pclass(MAX_LEN);
	vector<double> survived(MAX_LEN);
	vector<double> sex(MAX_LEN);
	vector<double> age(MAX_LEN);
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
				personID.at(numEntry) = stod(person_in);
				pclass.at(numEntry) = stod(pclass_in);
				survived.at(numEntry) = stod(survived_in);
				sex.at(numEntry) = stod(sex_in);
				age.at(numEntry) = stod(age_in);
			}

			numEntry++;
		}
		
		personID.resize(numEntry);
		pclass.resize(numEntry);
		survived.resize(numEntry);
		sex.resize(numEntry);
		age.resize(numEntry);
		
		cout << numEntry << endl;
		inFS.close();

	}
}

