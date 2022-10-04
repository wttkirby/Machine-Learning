// Machine Learning Scratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>

using namespace std;


int getNumLines(string);						// Counts and returns the number of lines in a given file stream
void printSex(double sex[][2]);
double ageMean(vector<double>);
double ageVar(vector<double>, double);
void printPclass(double[][2]);
double calcAccuracy(double[2][2]);				// Calculates and returns the sensitivity from the given the confusion matrix
double calcSensitivity(double[2][2]);				// Calculates and returns the sensitivity from the given the confusion matrix
double calcSpecificity(double[2][2]);				// Calculates and returns the specificity from the given the confusion matrix
vector<double>  calcProb(int pclass, int sex, double age,double rawAge[][2], double rawClass[][2], double rawSex[][2], double probDied);
double ageLH( double, double, double);


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
		cout << "File number of lines: " << fileSize << endl;

		// Initialize our vectors, strings, and variables for getting our data
		string person_in, pclass_in, survived_in, sex_in, age_in; // Used to temporarily hold values from getline
		const int trainSize = 800;
		const int testSize = (fileSize - 800);
		vector<double> personIDTrain(trainSize);
		vector<double> pclassTrain(trainSize);
		vector<double> survivedTrain(trainSize);
		vector<double> sexTrain(trainSize);
		vector<double> ageTrain(trainSize);
		vector<double> personIDTest(testSize);
		vector<double> pclassTest(testSize);
		vector<double> survivedTest(testSize);
		vector<double> sexTest(testSize);
		vector<double> ageTest(testSize);
		vector<double> numOneTrain(trainSize);		// Used for multiplying our vectors later
		vector<double> weights(2);
		weights.assign(2,1);


		// Used to track the current entry in our while loop below
		int numEntry = 0;	
		int numEntry2 = 0;	

		// Read in the first line to skip it (just headers)
		getline(inFS, line);
		// cout << "heading: " << line << endl;

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
			if (numEntry < fileSize-1) {
				// If the data entry is in the first 80% of the data set, add it to train vectors
				if (numEntry < trainSize) {
					personIDTrain.insert(personIDTrain.begin() + numEntry, stod(person_in));		// Add the person's ID entry to the person ID train vector
					pclassTrain.insert(pclassTrain.begin() + numEntry, stod(pclass_in));			// Add the person's class entry to the person class train vector
					survivedTrain.insert(survivedTrain.begin() + numEntry, stod(survived_in));		// Add the survived entry to the survived train vector
					sexTrain.insert(sexTrain.begin() + numEntry, stod(sex_in));				// Add the person's sex entry to the sex train vector
					ageTrain.insert(ageTrain.begin() + numEntry, stod(age_in));				// Add the person's age entry to the age train vector
					numOneTrain.insert(numOneTrain.begin() + 1, 1);						// Add the 1 for the number one vector
				}
				else {	// Otherwise it is in the last 20% of the data set, add it to the test vectors
					personIDTest.insert(personIDTest.begin() + numEntry2, stod(person_in));		// Add the person's ID entry to the person ID test vector
					pclassTest.insert(pclassTest.begin() + numEntry2, stod(pclass_in));		// Add the person's class entry to the person class test vector
					survivedTest.insert(survivedTest.begin() + numEntry2, stod(survived_in));	// Add the survived entry to the survived test vector
					sexTest.insert(sexTest.begin() + numEntry2, stod(sex_in));				// Add the person's sex entry to the sex test vector
					ageTest.insert(ageTest.begin() + numEntry2, stod(age_in));				// Add the person's age entry to the age test vector
					numEntry2++;										// Increment the numEntry 2 (used for test vectors index)			
				}

			}
			numEntry++;		// Increment the numEntry (used for the train vectors index)
		}

		cout << "Closing file titanic_project.csv" << endl << endl;
		inFS.close();
		
		double rawProbSex[2][2]; //sex 0 is female sex 1 male
								/*
										female died   female lived
										male died	  male lived
								*/
		double rawProbPclass[3][2];
								/*
										class1 died    class1 lived
										class2 died    class2 lived
										class3 died    class3 lived
								*/

		double numDiedM, totalM;
		double numDiedF, totalF;
		double totalOne, totalTwo, totalThree;
		double oneDied, twoDied, threeDied;

		for(int i = 0; i < 800; i++){
				if(sexTrain.at(i) == 1){
					totalM++;
					if(survivedTrain.at(i) == 0)
						numDiedM++;
				}
				else if(sexTrain.at(i) == 0){
					totalF++;
					if(survivedTrain.at(i) == 0)
						numDiedF++;
				}

				if(pclassTrain.at(i) == 1){
					totalOne++;
					if(survivedTrain.at(i) == 0)
						oneDied++;
				}
				else if(pclassTrain.at(i) == 2){
					totalTwo++;
					if(survivedTrain.at(i) == 0)
						twoDied++;
				}
				else if(pclassTrain.at(i) == 3){
					totalThree++;
					if(survivedTrain.at(i) == 0)
						threeDied++;
				}
		}

		rawProbSex[0][0] = (numDiedF/totalF);
		rawProbSex[0][1] = ( 1 - (numDiedF/totalF));
		rawProbSex[1][0] = (numDiedM/totalM);
		rawProbSex[1][1] = ( 1 - (numDiedM/totalM));

		rawProbPclass[0][0] = (oneDied/totalOne);
		rawProbPclass[0][1] = 1-(oneDied/totalOne);
		rawProbPclass[1][0] = (twoDied/totalTwo);
		rawProbPclass[1][1] = 1-(twoDied/totalTwo);
		rawProbPclass[2][0] = (threeDied/totalThree);
		rawProbPclass[2][1] = 1-(threeDied/totalThree);

		printSex(rawProbSex);
		printPclass(rawProbPclass);

		double averageAge = ageMean(ageTrain);
		double varianceAge = ageVar(ageTrain, averageAge);
		double numKids, numTeens, numAdult, numMidAdult, numSenior;
		double sumKids, sumTeens, sumAdult, sumMidAdult, sumSenior;
		double probKids, probTeens, probAdult, probMidAdult, probSenior;
		double probDied = ((numDiedF + numDiedM)/(totalF + totalM));
		double sumAgeDead, sumAgeAlive;
		double numDead, numAlive;

		for( int i = 0; i < 800; i++){
			
			if(ageTrain.at(i) <= 12){
				numKids++;
				if(survivedTrain.at(i) == 0)
					sumKids++;
			}
			else if(ageTrain.at(i) <= 19){
				numTeens++;
				if(survivedTrain.at(i) == 0)
					sumTeens++;
			}
			else if(ageTrain.at(i) <= 39){
				numAdult++;
				if(survivedTrain.at(i) == 0)
					sumAdult++;
			}
			else if(ageTrain.at(i) <= 59){
				numMidAdult++;
				if(survivedTrain.at(i) == 0)
					sumMidAdult++;
			}
			else{
				numSenior++;
				if(survivedTrain.at(i) == 0)
					sumSenior++;
			}
		}

		probKids = sumKids / numKids;
		probTeens = sumTeens / numTeens;
		probAdult = sumAdult / numAdult;
		probMidAdult = sumMidAdult / numMidAdult;
		probSenior = sumSenior / numSenior;

		double rawProbAge[5][2];

		rawProbAge[0][0] = probKids;
		rawProbAge[0][1] = (1 - probKids);
		rawProbAge[1][0] = probTeens;
		rawProbAge[1][1] = (1 - probTeens);
		rawProbAge[2][0] = probAdult;
		rawProbAge[2][1] = (1 - probAdult);
		rawProbAge[3][0] = probMidAdult;
		rawProbAge[3][1] = (1 - probMidAdult);
		rawProbAge[4][0] = probSenior;
		rawProbAge[4][1] = (1 - probSenior);

		double avgAgeDied = sumAgeDead / numDead;
		double avgAgeSurv = sumAgeAlive / numAlive;

		double mean[2];
		mean[0] = avgAgeDied;
		mean[1] = avgAgeSurv;

		double sumVarD, sumVarA;
		for(int i = 0; i < 800; i++){
			if(survivedTrain.at(i) == 0){
				sumVarD += ((ageTrain.at(i) - avgAgeDied) * (ageTrain.at(i) - avgAgeDied) * (ageTrain.at(i) - avgAgeDied) * (ageTrain.at(i) - avgAgeDied));
			}
			else{
				sumVarA += ((ageTrain.at(i) - avgAgeSurv) * (ageTrain.at(i) - avgAgeSurv) * (ageTrain.at(i) - avgAgeSurv) * (ageTrain.at(i) - avgAgeSurv));
			}
		}

		double varience[2];
		varience[0] = (sumVarD/(numDead -1));
		varience[1] = (sumVarA/ (numAlive -1));

		cout << "Predictions" << endl;

		cout << "Test:" << endl << "Survived:      Died:" << endl;

		int sumCorrect = 0;
		double confusionMatrix[2][2];
		confusionMatrix[0][0] = 0;
		confusionMatrix[1][0] = 0;
		confusionMatrix[0][1] = 0;
		confusionMatrix[1][1] = 0;

		//cout << "pclassTest: " << pclassTest.at(0) << endl;
		//cout << "sexTest: " << sexTest.at(0) << endl;
		//cout << "ageTest: " << ageTest.at(0) << endl;
		//cout << "rawprobclass: " << rawProbPclass << endl;
		//cout << "rawProbSexz: " << rawProbSex << endl;
		//cout << "mean: " << mean << endl;
		//cout << "variance: " << varience << endl;
		//cout << "probDied: " << probDied << endl;


		for(int i = 0; i < 247; i++){
			vector<double> probs = calcProb(pclassTest.at(i), sexTest.at(i), ageTest.at(i), rawProbAge, rawProbPclass, rawProbSex, probDied);

			cout << probs.at(1) << "      " << probs.at(0) << endl;

			if(probs.at(0) > .5 && survivedTest.at(i) == 0){
				//cout << "TN" << endl;
				sumCorrect = sumCorrect + 1;
				confusionMatrix[1][1] += 1;
			}
			else if(probs.at(1) > .5 && survivedTest.at(i) == 1){
				//cout << "TP" << endl;
				sumCorrect = sumCorrect + 1;
				confusionMatrix[0][0] += 1;
			}
			else if(probs.at(1) > .5) {
				confusionMatrix[0][1] += 1;
				//cout << "FP" << endl;
			}
			else {
				confusionMatrix[1][0] += 1;
				//cout << "FN" <A< endl;
			}
		}

		cout << endl << "Confusion Matrix:" << endl << endl;

		for (int i = 0; i < 2; i++) {
			cout << confusionMatrix[i][0] << ", " << confusionMatrix[i][1] << endl << endl;
		}

		//cout << "Sumcorrect: " << sumCorrect << endl;
		cout << "Accuracy: " << calcAccuracy(confusionMatrix) << endl;

		cout << "Sensitivity: " << calcSensitivity(confusionMatrix) << endl;

		cout << "Specificity: " << calcSpecificity(confusionMatrix) << endl;


	}

	cout << endl << "Exiting..." << endl;
}

vector<double> calcProb(int pclass, int sex, double age,double rawAge[][2], double rawClass[][2], double rawSex[][2], double probDied){

	double prbAgeSurv, prbAgeDie;
	if(age <= 12){
			prbAgeDie = rawAge[0][0];
			prbAgeSurv = rawAge[0][1];
	}
	else if(age <= 19){
			prbAgeDie = rawAge[1][0];
			prbAgeSurv = rawAge[1][1];
	}
	else if(age <= 39){
			prbAgeDie = rawAge[2][0];
			prbAgeSurv = rawAge[2][1];
	}
	else if(age <= 59){
			prbAgeDie = rawAge[3][0];
			prbAgeSurv = rawAge[3][1];
	}
	else{
		prbAgeDie = rawAge[4][0];
		prbAgeSurv = rawAge[4][1];
	}
	double survived = (rawClass[(pclass - 1)][1]) * (rawSex[(sex)][1]) * prbAgeSurv * (1 - probDied);
	double dead = (rawClass[(pclass - 1)][0]) * (rawSex[(sex)][0]) * prbAgeDie * probDied;
	double denominator = survived + dead;

	//cout << "rawclass[][]: " << (rawClass[(pclass - 1)][1]) << endl;
	//cout << "rawsex[][]: " << (rawSex[(sex)][1]) << endl;
	//cout << "probDied: " << probDied << endl;


	//cout << "Survived in calcProb: " << survived << endl;
	//cout << "dead in calcProb: " << dead << endl;
	//cout << "denominator in calcProb: " << denominator << endl;

	vector<double> answer(2);
	answer.insert((answer.begin()), (survived / denominator));
	answer.insert((answer.begin() + 1), (dead / denominator));

	return answer;
}

double ageMean(vector<double> age){
	int sum = 0;

	for(int i = 0; i < 800; i++){
		sum += age.at(i);
	}

	return(sum/800);
}

double ageVar(vector<double> age, double average){
	int ave = ageMean(age);

	int sum = 0;

	for(int i = 0; i < 800; i++){
		sum += ((age.at(i) - ave) * (age.at(i) - ave));
	}

	return((double(sum)/799.0));
}

void printSex(double sex[][2]){
	cout << "Probabilities for survival based on sex:" << endl << endl;
	cout << "female died   female lived" << endl << "male died   male lived" << endl << endl;

	for(int i = 0; i < 2; i++)
			cout << sex[i][0] << ", " <<  sex[i][1] << endl;

	cout << endl;
}

void printPclass(double pclass[][2]){
	cout << "Probabilities for survival based on passenger class:" << endl << endl;
	cout << "class1 died    class1 lived" << endl << "class2 died    class2 lived" << endl << "class3 died    class3 lived" << endl << endl;

	for(int i = 0; i < 3; i++)
			cout << pclass[i][0] << ", " <<  pclass[i][1] << endl;

	cout << endl;
}

void printAge(double pclass[][2]){
	cout << "Probabilities for survival based on passenger class:" << endl << endl;
	cout << "Kids died    Kids lived" << endl << "Teens died    Teens lived" << endl << "Adults died    Adults lived" << endl << "Middle Age Adults died    Middle Age Adults lived" << endl << "Seniors died    Seniors lived" << endl << endl;

	for(int i = 0; i < 5; i++)
			cout << pclass[i][0] << ", " <<  pclass[i][1] << endl;

	cout << endl;
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
		

// Calculates and returns the accuracy from the given confusion matrix
double calcAccuracy(double confusionMatrix[2][2]) {
	return ( (confusionMatrix[0][0] + confusionMatrix[1][1]) / (confusionMatrix[0][0] + confusionMatrix[0][1] + confusionMatrix[1][0] + confusionMatrix[1][1]) );
}		

// Calculates and returns the sensitivity from the given confusion matrix
double calcSensitivity(double confusionMatrix[2][2]) {
	return ( (confusionMatrix[0][0]) / (confusionMatrix[0][0] + confusionMatrix[1][0]) );
}				

// Calculates and returns the specificity from the given confusion matrix
double calcSpecificity(double confusionMatrix[2][2]) {
	return ( (confusionMatrix[1][1]) / (confusionMatrix[1][1] + confusionMatrix[0][1]) );
}	