#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

struct feature_label{
    vector<double> feature_vector;
    int label;
};

struct dist_label{
    double distance;
    int label;
};

double compare(vector<double> v1, vector<double> v2)
{
    double temp = 0;
    for (int i = 0; i < 784; i++)
    {
        temp += pow((v1[i] - v2[i]),2);
    }
    temp = sqrt(temp);
    return temp;
}

double classify(int index, vector<feature_label> test, vector<feature_label> train, int k)
{
    vector<dist_label> dist;
    for (int j = 0; j < 1000; j++)
    {
        dist_label tempDist;
        /*
        if (j == index)
        {
            tempDist.distance = 100000;
            tempDist.label = train[index].label;
        }
        else
        {*/
            double temp = compare(test[index].feature_vector, train[j].feature_vector);
            tempDist.distance = temp;
            tempDist.label = train[j].label;
            dist.push_back(tempDist);
        //}
    }
    vector<int> kNN (10, 0);
    for (int i = 0; i < k; i++)
    {
        double tempMax = 100000;
        int tempMaxLabel;
        int tempMaxIndex;
        for (int k = 0; k < 1000; k++)
        {
            if (dist[k].distance < tempMax)
            {
                tempMax = dist[k].distance;
                tempMaxLabel = dist[k].label;
                tempMaxIndex = k;
            }
        }
        dist[tempMaxIndex].distance = 100000;
        kNN[tempMaxLabel]++;
    }
    int label = 0;
    for (int i = 0; i < kNN.size(); i++)
    {
        int maxLabel = 0;
        if (kNN[i] > maxLabel)
        {
            maxLabel = kNN[i];
            label = i;
        }
    }
    return label;
}

int main()
{
    ifstream infile;
    infile.open("hw2train.txt");
    vector<feature_label> training_set;
    double temp;
    for (int j = 0; j < 1000; j++)
    {
        feature_label tempStruct;
        for (int i = 0; i <784; i++)
        {
            infile >> temp;
            tempStruct.feature_vector.push_back(temp);
        }
        infile >> temp;
        tempStruct.label = temp;
        training_set.push_back(tempStruct);
    }

    infile.close();

    infile.open("hw2validate.txt");
    vector<feature_label> validate_set;
    double temp2;
    for (int j = 0; j < 1000; j++)
    {
        feature_label tempStruct;
        for (int i = 0; i <784; i++)
        {
            infile >> temp2;
            tempStruct.feature_vector.push_back(temp2);
        }
        infile >> temp2;
        tempStruct.label = temp2;
        validate_set.push_back(tempStruct);
    }

    infile.open("hw2test.txt");
    vector<feature_label> test_set;
    double temp3;
    for (int j = 0; j < 1000; j++)
    {
        feature_label tempStruct;
        for (int i = 0; i <784; i++)
        {
            infile >> temp3;
            tempStruct.feature_vector.push_back(temp3);
        }
        infile >> temp3;
        tempStruct.label = temp3;
        test_set.push_back(tempStruct);
    }

    double right = 0;
    for (int i = 0; i < 300; i++)
    {
        if (classify(i, test_set,training_set, 1) == test_set[i].label)
        {
            right++;
        }
    }

    cout << right / 1000.0 << endl;
    right = 0;
    /*
    for (int i = 0; i < 300; i++)
    {
        if (classify(i, validate_set,training_set, 3) == validate_set[i].label)
        {
            right++;
        }
    }

    cout << right / 1000.0 << endl;
    right = 0;

    for (int i = 0; i < 300; i++)
    {
        if (classify(i, validate_set,training_set, 5) == validate_set[i].label)
        {
            right++;
        }
    }

    cout << right / 1000.0 << endl;
    right = 0;

    for (int i = 0; i < 300; i++)
    {
        if (classify(i, validate_set,training_set, 11) == validate_set[i].label)
        {
            right++;
        }
    }

    cout << right / 1000.0 << endl;
    right = 0;

    for (int i = 0; i < 300; i++)
    {
        if (classify(i, validate_set,training_set, 16) == validate_set[i].label)
        {
            right++;
        }
    }

    cout << right / 1000.0 << endl;
    right = 0;

    for (int i = 0; i < 300; i++)
    {
        if (classify(i, validate_set,training_set, 21) == validate_set[i].label)
        {
            right++;
        }
    }

    cout << right / 1000.0 << endl;
    right = 0;
    */
}
