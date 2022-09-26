
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

double sumVect(vector<double>);
double meanVect(vector<double>);
double medianVect(vector<double>);
double rangeVect(vector<double>);
double covariance(vector<double>, vector<double>);
double correlation(vector<double>, vector<double>);

int main( int argc, char** argv)
{
	ifstream inFS;
	string line;
	string rm_in, medv_in;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);
	int numEntry = 0;

	cout << "Opening file Boston.csv" << endl << endl;

	inFS.open("Boston.csv");

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
			getline(inFS, rm_in, ',');
			getline(inFS, medv_in, '\n');

			rm.at(numEntry) = stof(rm_in);
			medv.at(numEntry) = stof(medv_in);

			numEntry++;
		}

		rm.resize(numEntry);
		medv.resize(numEntry);

		inFS.close();
		
		cout << "For rm:\nSum of rm is: " << sumVect(rm) << endl;
		cout << "Mean of rm is: " << meanVect(rm) << endl;
		cout << "Median of rm is: " << medianVect(rm) << endl;
		cout << "Range of rm is: " << rangeVect(rm) << endl << endl;

		cout << "For medv:\nSum of rm is: " << sumVect(medv) << endl;
		cout << "Mean of rm is: " << meanVect(medv) << endl;
		cout << "Median of rm is: " << medianVect(medv) << endl;
		cout << "Range of rm is: " << rangeVect(medv) << endl << endl;

		cout << "Covariance: " << covariance(rm, medv) << endl;
		cout << "Correlation: " << correlation(rm, medv) << endl;
	}
}

double sumVect(vector<double> vector) 
{
	double sum = 0;

	for (int i = 0; i < vector.size(); i++)
	{
		sum += vector.at(i);
	}

	return sum;
}

double meanVect(vector<double> vector) 
{
	double sum = sumVect(vector);
	double size = vector.size();
	double mean = sum / size;

	return mean;
}

double medianVect(vector<double> vector)
{
	int size = vector.size();
	int middle = size / 2;
	double median = -1;

	sort(vector.begin(), vector.end());

	if ( size % 2 == 0) {
		median = ((vector.at(middle - 1) + vector.at(middle)) / 2);
	}
	else {
		median = vector.at(middle);
	}

	return median;
}

double rangeVect(vector<double> vector)
{
	double top, bottom = -1;
	double range = -1;

	sort(vector.begin(), vector.end());

	top = vector.at((vector.size() -1 ));
	bottom = vector.at(0);

	range = top - bottom;

	return range;
}

double covariance(vector<double> vect1, vector<double> vect2)
{
	double mean1 = meanVect(vect1);
	double mean2 = meanVect(vect2);
	double size = vect1.size();
	double sumOf = 0;
	double covar = -1;

	for (int i = 0; i < vect1.size(); i++)
	{
		sumOf += (vect1.at(i) - mean1) * (vect2.at(i) - mean2);
	}

	covar = sumOf / size;

	return covar;
}

double variance(vector<double> vector)
{
	double mean = meanVect(vector);
	double size = vector.size();
	double sumOf = 0;
	double variance = -1;

	for (int i = 0; i < vector.size(); i++)
	{
		sumOf += (vector.at(i) - mean) * (vector.at(i) - mean);
	}

	variance = sumOf / (size - 1);

	return variance;
}

double correlation(vector<double> vect1, vector<double> vect2)
{
	double covar = covariance(vect1, vect2);
	double sigma1 = sqrt(variance(vect1));
	double sigma2 = sqrt(variance(vect2));
	double correl = covar / (sigma1 * sigma2);
	
	return correl;
}
