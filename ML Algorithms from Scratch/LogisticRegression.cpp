// Machine Learning Scratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>
#include <chrono>

using namespace std;


int getNumLines(string);									// Counts and returns the number of lines in a given file stream
vector<double> dotProd(int[][2], vector<double>, int);		// Computes the dot product of a vector matrix with a weight vector and returns the product.
vector<double> dotProdTwo(int[2][800], vector<double>);		// Computes the dot product of a vector matrix with a weight vector and returns the product.
double calcAccuracy(double[2][2]);							// Calculates and returns the accuracy from the given confusion matrix
double calcSensitivity(double[2][2]);						// Calculates and returns the sensitivity from the given confusion matrix
double calcSpecificity(double[2][2]);						// Calculates and returns the specificity from the given confusion matrix
vector<int> doPredicts(vector<double>, int);
void printEverything(double[2][2], double, double, double, auto);


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
					personIDTrain.at(numEntry) = stod(person_in);		// Add the person's ID entry to the person ID train vector
					pclassTrain.at(numEntry) = stod(pclass_in);			// Add the person's class entry to the person class train vector
					survivedTrain.at(numEntry) = stod(survived_in);		// Add the survived entry to the survived train vector
					sexTrain.at(numEntry) = stod(sex_in);				// Add the person's sex entry to the sex train vector
					ageTrain.at(numEntry) = stod(age_in);				// Add the person's age entry to the age train vector
					numOneTrain.at(numEntry) = 1;						// Add the 1 for the number one vector
				}
				else {	// Otherwise it is in the last 20% of the data set, add it to the test vectors
					personIDTest.at(numEntry2) = stod(person_in);		// Add the person's ID entry to the person ID test vector
					pclassTest.at(numEntry2) = stod(pclass_in);		// Add the person's class entry to the person class test vector
					survivedTest.at(numEntry2) = stod(survived_in);	// Add the survived entry to the survived test vector
					sexTest.at(numEntry2) = stod(sex_in);				// Add the person's sex entry to the sex test vector
					ageTest.at(numEntry2) = stod(age_in);				// Add the person's age entry to the age test vector
					numEntry2++;										// Increment the numEntry 2 (used for test vectors index)			
				}

			}
			numEntry++;		// Increment the numEntry (used for the train vectors index)
		}

		cout << "Closing file titanic_project.csv" << endl << endl;
		inFS.close();
		
		// Creating the training set matrix (array) for sex
		int trainSexMatrix[trainSize][2];
		// Creating the testing set matrix (array) for sex
		int testSexMatrix[testSize][2];

		// Setting the values for the sex training and testing set
		for (int i = 0; i < fileSize - 1; i++) {
			// If i is in the training set
			if (i < trainSize) {
			//cout << "i: "  << i << endl;
			// Set trainSex[0][i]
			trainSexMatrix[i][0] = 1;
			// Set trainSex[1][i]
			trainSexMatrix[i][1] = sexTrain.at(i);
			//cout << "trainSex[i][0]: " << trainSex[i][0] << endl;
			//cout << "trainSex[i][1]: " << trainSex[i][1] << endl;
			}	// If i is in the testing set
			else if (i < testSize + trainSize) {
			//cout << "i - trainSize: "  << i-trainSize << endl;
			// Set testSex[0][i]
			testSexMatrix[i-trainSize][0]  = 1;
			// Set testSex[1][i]
			testSexMatrix[i-trainSize][1] = sexTest.at(i-trainSize);
			//cout << "testSex[i-trainSize][0]: " << testSexMatrix[i-trainSize][0] << endl;
			//cout << "testSex[i-trainSize][1]: " << testSexMatrix[i-trainSize][1] << endl;
			}			
		} 


	// Logistic Regression
	const double learningRate = 0.001;		// The learning rate which will scale our tranDotProduct vector
	vector<double> probability(trainSize);	// The probability vector
	vector<double> dotProduct(trainSize);	// Used to hold the current dotProduct vector calculated by dotProd
	vector<double> tranDotProduct(trainSize);	// Used to hold the dotproduct of the transposed train matrix and error vector
	vector<double> error(trainSize);			// The errors vector
	int tranTrainSexMatrix[2][trainSize];	// The tranposed matrix of the trainSexMatrix

	// Get the tranposed matrix of the trainSexMatrix
	for (int i = 0; i < trainSize; i++) {
		for (int j = 0; j < 2; j++) {
			tranTrainSexMatrix[j][i] = trainSexMatrix[i][j];
		}
	}

	auto startClock = chrono::high_resolution_clock::now();
	// Iterate 100000 times
	for (int i = 0; i < 100000; i++) {
		// Get the dotProduct of the trainSetMatrix and weight vector
		dotProduct = dotProd(trainSexMatrix, weights, trainSize);


		// Create probability vector and set it's values
		for (int j = 0; j < trainSize-1; j++) {
			probability.at(j) = (1 / ( 1 + exp(-1*dotProduct.at(j))));
			error.at(j) = survivedTrain.at(j) - probability.at(j);
			//cout << "error: " << error.at(j) << endl;
		}
		
		// Get the dotProduct of the tranposed matrix and the error matrix
		tranDotProduct = dotProdTwo(tranTrainSexMatrix, error);

		// Scale the tranDotProduct vector by our learningRate
		for(int j = 0; j < 2; j++) {
			tranDotProduct.at(j) = tranDotProduct.at(j) * learningRate;
		}

		// Adjusting weight + learningRate + tranDotProduct
		weights.at(0) =  weights.at(0) + tranDotProduct.at(0);
		weights.at(1) =  weights.at(1) + tranDotProduct.at(1);

	}

	auto stopClock = chrono::high_resolution_clock::now();

	vector<double> logOdds(testSize);
		vector<double> probVect(testSize);
		vector<int> sexTestPred(testSize);

		logOdds = dotProd(testSexMatrix, weights, testSize);



		for( int i = 0; i < testSize; i++){
			//cout << "i: " << i << endl;
			//cout << "logOdds.at(i): " << logOdds.at(i) << endl;
			//cout << "exp(logOdds.at(i)): " << exp(logOdds.at(i)) << endl;
			//cout << "( 1 + exp(logOdds.at(i))): " << ( 1 + exp(logOdds.at(i))) << endl;
			probVect.at(i) =  (exp(logOdds.at(i)) / ( 1 + exp(logOdds.at(i))));
		}

		sexTestPred = doPredicts(probVect, testSize);

		double confusionMatrix[2][2];
		confusionMatrix[0][0] = 0;
		confusionMatrix[1][0] = 0;
		confusionMatrix[0][1] = 0;
		confusionMatrix[1][1] = 0;

		for(int i = 0; i < testSize; i++){

			//cout << endl << "sextestpred: " << sexTestPred.at(i) << endl;
			//cout << "survived: " << survivedTest.at(i) << endl;

			if(sexTestPred.at(i) == survivedTest.at(i) && sexTestPred.at(i) == 1){;
				confusionMatrix[0][0] += 1;
			}
			else if(sexTestPred.at(i) == survivedTest.at(i) && sexTestPred.at(i) == 0){
				confusionMatrix[1][1] += 1;

			}
			else if(sexTestPred.at(i) == 0){
				confusionMatrix[1][0] += 1;
			}
			else{
				confusionMatrix[0][1] += 1;
			}
		}

		double accuracyTest = 0;
		double sensitivityTest = 0;
		double specificityTest = 0;
		auto duration = chrono::duration_cast<chrono::microseconds>(stopClock - startClock);

		accuracyTest = calcAccuracy(confusionMatrix);
		sensitivityTest = calcSensitivity(confusionMatrix);
		specificityTest = calcSpecificity(confusionMatrix);

		printEverything(confusionMatrix, accuracyTest, sensitivityTest, specificityTest, duration);

	}

	cout << endl << "Exiting..." << endl;
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
	.
*/
vector<double> dotProd(int matrix[][2], vector<double> weights, int size) {

	vector<double> answer(size);

	// For each index of the answer vector
	for (int i = 0; i < size; i++) {
		// Multiplies the value at matrix[0][i] with the weights vector at 0
		answer.at(i) = ( double(matrix[i][0]) * weights.at(0) + double(matrix[i][1]) * weights.at(1) );
	}

	// Returns a vector or a nx1 matrix holding the ending matrix we need for calculations
	return answer;
}

vector<double> dotProdTwo(int matrix[2][800], vector<double> error) {
	
	vector<double> answer(2);
	double sum = 0;
	double sum2 = 0;
	// For each index of the answer vector
	for( int j = 0; j < 800; j++){

		sum += matrix[0][j] * error.at(j);
		sum2 += matrix[1][j] * error.at(j);
	}
		answer.at(0) = sum;
		answer.at(1) = sum2;
	// Returns a vector or a nx1 matrix holding the ending matrix we need for calculations
	return answer;
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

// Calculates the predictions based on probabilities vector
vector<int> doPredicts(vector<double> probSex , int size){
	vector<int> predictions(size);
	
	for(int i = 0; i < size; i++){
		if(probSex.at(i) > .5){
			predictions.at(i) =  1;
		}
		else{
			predictions.at(i) = 0;
		}

	}

	return predictions;
}

// Prints the given metrics to the user
void printEverything(double matrix[2][2], double acc, double sens, double spec, auto time){
	cout << "Confusion Matrix:" << endl << endl;

	for(int i = 0; i < 2; i++){
			cout << matrix[i][0] << ", " << matrix[i][1] << endl << endl;
	}
	
	cout << "Accuracy: " << acc << endl;
	cout << "Sensitivity: " << sens << endl;
	cout << "Specificity: " << spec << endl;
	cout << "Duration: " << time.count() << " microseconds" << endl;
}