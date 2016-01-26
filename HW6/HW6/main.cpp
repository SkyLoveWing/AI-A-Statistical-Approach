#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

int main()
{
    ifstream infile;
    infile.open("hw6train.txt");
    double ** trainMatrix = new double*[500];
    for (int i = 0; i < 500; i++)
    {
        trainMatrix[i] = new double[1532];
    }
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 1532; j++)
        {
            double temp;
            infile >> temp;
            trainMatrix[i][j] = temp;
        }
    }
    infile.close();

    vector<double> weights;
    for (int i = 0; i < 500; i++)
    {
        weights.push_back(1.0/500.0);
    }

    double minError = 100000;
    int classifier;
    int index;
    vector<double> alphavector;
    vector<int> classifiervector;
    vector<int> indexvector;
    for( int k = 0; k < 20; k++)
    {
        minError = 100000;
        for (int i = 0; i < 1531; i++)
        {
            double errhiplus = 0;
            double errhiminus = 0;
            for (int j = 0; j < 500; j++)
            {
                int hiplus;
                int himinus;
                if (trainMatrix[j][i] == 1)
                {
                    hiplus = 1;
                    himinus = -1;
                }
                else
                {
                    hiplus = -1;
                    himinus = 1;
                }
                if (hiplus != trainMatrix[j][1531])
                {
                    errhiplus += weights[j];
                }
                if (himinus != trainMatrix[j][1531])
                {
                    errhiminus += weights[j];
                }
            }
            if ((errhiplus < errhiminus) && (errhiplus <= minError))
            {
                classifier = 1;
                minError = errhiplus;
                index = i;
            }
            else if ((errhiminus < errhiplus) && (errhiminus <= minError))
            {
                classifier = 2;
                minError = errhiminus;
                index = i;
            }
        }
        double alpha = .5 * (log((1 - minError)/minError));
        alphavector.push_back(alpha);
        classifiervector.push_back(classifier);
        indexvector.push_back(index);
        for (int i = 0; i < 500; i++)
        {
            double yi;
            if (trainMatrix[i][1531] == 1.0)
                yi = 1.0;
            else
                yi = -1.0;
            if  (trainMatrix[i][index] == 1)
            {
                if (classifier == 1)
                {
                    weights[i] = weights[i] * exp(-alpha * yi * 1.0);
                }
                else
                {
                    weights[i] = weights[i] * exp(-alpha * yi * -1.0);
                }
            }
            else
            {
                if (classifier == 1)
                {
                    weights[i] = weights[i] * exp(-alpha * yi * -1.0);
                }
                else
                {
                    weights[i] = weights[i] * exp(-alpha * yi * 1.0);
                }
            }
        }

        double sum = 0;
        for (int i = 0; i < 500; i++)
        {
            sum += weights[i];
        }
        for (int i = 0; i < 500; i++)
        {
            weights[i] = weights[i] / sum;
        }

        int incorrect = 0;
        for(int i = 0; i < 500; i++)
        {
            double result = 0;
            for (int j = 0; j < alphavector.size(); j++)
            {
                if (classifiervector[j] == 1)
                {
                    if (trainMatrix[i][index] == 1)
                    {
                        result += alphavector[j];
                    }
                    else
                    {
                        result -= alphavector[j];
                    }
                }
                if (classifiervector[j] == 2)
                {
                    if (trainMatrix[i][index] == 1)
                    {
                        result -= alphavector[j];
                    }
                    else
                    {
                        result += alphavector[j];
                    }
                }
            }
            double x;
            if (result > 0)
            {
                x = 1.0;
            }
            else
            {
                x = -1.0;
            }
            if (x != trainMatrix[i][1531])
            {
                incorrect++;
            }
        }
        cout << "Training Error " << k+1 << ": " << setprecision(4) << incorrect/500.0 << "     Index:" << indexvector[k]+1 << endl;
    }






    infile.open("hw6test.txt");
    double ** testMatrix = new double*[300];
    for (int i = 0; i < 300; i++)
    {
        testMatrix[i] = new double[1532];
    }
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 1532; j++)
        {
            double temp;
            infile >> temp;
            testMatrix[i][j] = temp;
        }
    }
    infile.close();

    vector<double> testweights;
    for (int i = 0; i < 300; i++)
    {
        testweights.push_back(1.0/300.0);
    }

    double testminError = 100000;
    int testclassifier;
    int testindex;
    vector<double> testalphavector;
    vector<int> testclassifiervector;
    vector<int> testindexvector;
    for( int k = 0; k < 20; k++)
    {
        testminError = 100000;
        for (int i = 0; i < 1531; i++)
        {
            double testerrhiplus = 0;
            double testerrhiminus = 0;
            for (int j = 0; j < 300; j++)
            {
                int testhiplus;
                int testhiminus;
                if (testMatrix[j][i] == 1)
                {
                    testhiplus = 1;
                    testhiminus = -1;
                }
                else
                {
                    testhiplus = -1;
                    testhiminus = 1;
                }
                if (testhiplus != testMatrix[j][1531])
                {
                    testerrhiplus += testweights[j];
                }
                if (testhiminus != testMatrix[j][1531])
                {
                    testerrhiminus += testweights[j];
                }
            }
            if ((testerrhiplus < testerrhiminus) && (testerrhiplus <= testminError))
            {
                testclassifier = 1;
                testminError = testerrhiplus;
                testindex = i;
            }
            else if ((testerrhiminus < testerrhiplus) && (testerrhiminus <= testminError))
            {
                testclassifier = 2;
                testminError = testerrhiminus;
                testindex = i;
            }
        }
        double testalpha = .5 * (log((1 - testminError)/testminError));
        testalphavector.push_back(testalpha);
        testclassifiervector.push_back(testclassifier);
        testindexvector.push_back(testindex);
        for (int i = 0; i < 300; i++)
        {
            int testyi;
            if (testMatrix[i][1531] == 1)
                testyi = 1.0;
            else
                testyi = -1.0;
            if  (testMatrix[i][testindex] == 1)
            {
                if (testclassifier == 1)
                {
                    testweights[i] = testweights[i] * exp(-testalpha * testyi * 1.0);
                }
                else if (testclassifier == 2)
                {
                    testweights[i] = testweights[i] * exp(-testalpha * testyi * -1.0);
                }
            }
            else if (testMatrix[i][testindex] == 0)
            {
                if (testclassifier == 1)
                {
                    testweights[i] = testweights[i] * exp(-testalpha * testyi * -1.0);
                }
                else if (testclassifier == 2)
                {
                    testweights[i] = testweights[i] * exp(-testalpha * testyi * 1.0);
                }
            }
        }

        double testsum = 0;
        for (int i = 0; i < 300; i++)
        {
            testsum += testweights[i];
        }

        for (int i = 0; i < 300; i++)
        {
            testweights[i] = testweights[i] / testsum;
        }

        int testincorrect = 0;
        for(int i = 0; i < 300; i++)
        {
            double testresult = 0;
            for (int j = 0; j < testalphavector.size(); j++)
            {
                if (testclassifiervector[j] == 1)
                {
                    if (testMatrix[i][testindex] == 1)
                    {
                        testresult += testalphavector[j];
                    }
                    else
                    {
                        testresult -= testalphavector[j];
                    }
                }
                if (testclassifiervector[j] == 2)
                {
                    if (testMatrix[i][testindex] == 1)
                    {
                        testresult -= testalphavector[j];
                    }
                    else
                    {
                        testresult += testalphavector[j];
                    }
                }
            }
            double testx;
            if (testresult > 0)
            {
                testx = 1.0;
            }
            else
            {
                testx = -1.0;
            }
            if (testx != testMatrix[i][1531])
            {
                testincorrect++;
            }
        }
        cout << "Test Error " << k+1 << ": " << setprecision(4) << testincorrect/300.0 << "     Test Index:" << testindexvector[k]+1 << endl;
    }

    return 0;
}
