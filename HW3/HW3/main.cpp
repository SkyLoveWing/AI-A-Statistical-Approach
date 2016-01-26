#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

struct feature_label {
    vector<double> feature_label;
    int label;
};

class Node
{
public:
    Node() {name = ""; noLink = NULL; yesLink = NULL;}
    Node *getYesLink() const {return yesLink;}
    Node *getNoLink() const {return noLink;}
    string getName() const {return name;}
    double getThreshold() const {return threshold;}
    int getIndex() const {return index;}
    vector<feature_label> getVector() const {return dataVector;}
    void setData(vector<feature_label> theVector) {dataVector = theVector;}
    void setNoLink(Node *newNoLink) {noLink = newNoLink;}
    void setYesLink(Node *newYesLink) {yesLink = newYesLink;}
    void setName(string newName) {name = newName;}
    void setThreshold(double newThreshold) {threshold = newThreshold;}
    void setIndex(int newIndex) {index = newIndex;}
private:
    string name;
    int index;
    double threshold;
    vector<feature_label> dataVector;
    Node *noLink;
    Node *yesLink;
};

vector<double> getColumn(vector<feature_label> someVector, int feat)
{
    vector<double> result;
    for(int i = 0; i < someVector.size(); i++)
    {
        result.push_back(someVector[i].feature_label[feat]);
    }
    return result;
}

double calcHx(vector<double> featVector)
{
    double result = 0.0;
    for (int i = 0; i < featVector.size(); i++)
    {
        double count = 0.0;
        for (int j = 0; j < featVector.size(); j++)
        {
            if (featVector[i] == featVector[j])
            {
                count++;
            }
        }
        double featVectSize = featVector.size();
        count = count / featVectSize;
        result += -count*log(count);
    }
    return result;
}

double calcHXV(vector<double> featVector)
{
    double result = 1000;
    for (int k = 0; k < (featVector.size() - 1); k++)
    {
        for (int i = k+1; i < featVector.size(); i++)
        {
            double threshold = (featVector[k] + featVector[i]) / 2.0;
            double noCount = 0.0;
            double yesCount = 0.0;
            vector<double> noVector;
            vector<double> yesVector;
            for (int j = 0; j < featVector.size(); j++)
            {
                if (featVector[j] <= threshold)
                {
                    yesCount++;
                    yesVector.push_back(featVector[j]);
                }
                else
                {
                    noCount++;
                    noVector.push_back(featVector[j]);
                }
            }
            double featVectorSize = featVector.size();
            double HofXV = (((yesCount/featVectorSize)*calcHx(yesVector)) + ((noCount/featVectorSize)*calcHx(noVector)));
            if (HofXV < result)
            {
                result = HofXV;
            }
        }
    }
    return result;
}

bool isPure(vector<feature_label> someVector)
{
    for (int i = 0; i < (someVector.size() - 1); i++)
    {
        if (someVector[i].label != someVector[i+1].label)
        {
            return false;
        }
    }
    return true;
}

double getThreshold(vector<feature_label> someVector, vector<double> featVector)
{
    double finalThreshold = 0.0;
    double result = 1000;
    for (int k = 0; k < (featVector.size() - 1); k++)
    {
        for (int i = k+1; i < featVector.size(); i++)
        {
            double threshold = (featVector[k] + featVector[i]) / 2.0;
            double noCount = 0.0;
            double yesCount = 0.0;
            vector<double> noVector;
            vector<double> yesVector;
            for (int j = 0; j < featVector.size(); j++)
            {
                if (featVector[j] <= threshold)
                {
                    yesCount++;
                    yesVector.push_back(featVector[j]);
                }
                else
                {
                    noCount++;
                    noVector.push_back(featVector[j]);
                }
            }
            double featVectorSize = featVector.size();
            double HofXV = (((yesCount/featVectorSize)*calcHx(yesVector)) + ((noCount/featVectorSize)*calcHx(noVector)));
            if (HofXV < result)
            {
                result = HofXV;
                finalThreshold = threshold;
            }
        }
    }
    return finalThreshold;
}

vector<feature_label> getNoVector(vector<feature_label> someVector, vector<double> featVector)
{
    double finalThreshold = getThreshold(someVector, featVector);
    vector<feature_label> resultVector;
    for (int i = 0; i < someVector.size(); i++)
    {
        if (featVector[i] > finalThreshold)
        {
            resultVector.push_back(someVector[i]);
        }
    }
    return resultVector;
}

vector<feature_label> getYesVector(vector<feature_label> someVector, vector<double> featVector)
{
    double finalThreshold = getThreshold(someVector, featVector);
    vector<feature_label> resultVector;
    for (int i = 0; i < someVector.size(); i++)
    {
        if (featVector[i] <= finalThreshold)
        {
            resultVector.push_back(someVector[i]);
        }
    }
    return resultVector;
}

void createTree(Node *someNode)
{
    if (isPure(someNode->getVector()) != true)
    {
        vector<feature_label> tempVector = someNode->getVector();
        vector<double> x1 = getColumn(someNode->getVector(),0);
        vector<double> x2 = getColumn(someNode->getVector(),1);
        vector<double> x3 = getColumn(someNode->getVector(),2);
        vector<double> x4 = getColumn(someNode->getVector(),3);

        double iG1 = calcHx(x1) - calcHXV(x1);
        double iG2 = calcHx(x2) - calcHXV(x2);
        double iG3 = calcHx(x3) - calcHXV(x3);
        double iG4 = calcHx(x4) - calcHXV(x4);

        double maxIG = iG1;
        double index = 0;
        vector<double> indexVector = x1;
        if (iG2 > maxIG)
        {
            indexVector = x2;
            index = 1;
            maxIG = iG2;
        }
        if (iG3 > maxIG)
        {
            indexVector = x3;
            index = 2;
            maxIG = iG3;
        }
        if (iG4 > maxIG)
        {
            indexVector = x4;
            index = 3;
            maxIG = iG4;
        }

        Node *newNoNode = new Node;
        newNoNode->setData(getNoVector(tempVector,indexVector));
        Node *newYesNode = new Node;
        newYesNode->setData(getYesVector(tempVector,indexVector));
        someNode->setNoLink(newNoNode);
        someNode->setYesLink(newYesNode);
        string newName = "";
        if (index == 0)
        {
            newName = "Is X1 <= ";
            someNode->setIndex(0);
        }
        else if (index == 1)
        {
            newName = "Is X2 <= ";
            someNode->setIndex(1);
        }
        else if (index == 2)
        {
            newName = "Is X3 <= ";
            someNode->setIndex(2);
        }
        else if (index == 3)
        {
            newName = "Is X4 <= ";
            someNode->setIndex(3);
        }
        someNode->setName(newName);
        someNode->setThreshold(getThreshold(tempVector, indexVector));
        createTree(newNoNode);
        createTree(newYesNode);
    }
}

int main()
{
    ifstream infile;
    infile.open("hw3train.txt");
    vector<feature_label> training_set;
    for (int i = 0; i < 100; i++)
    {
        feature_label tempStruct;
        double temp;
        for (int j = 0; j < 4; j++)
        {
            infile >> temp;
            tempStruct.feature_label.push_back(temp);
        }
        infile >> temp;
        tempStruct.label = temp;
        training_set.push_back(tempStruct);
    }
    Node *root;
    Node *newNode = new Node;
    newNode->setData(training_set);
    root = newNode;
    //Node* root2 = newNode;
    createTree(root);
    infile.close();
    infile.open("hw3test.txt");
    vector<feature_label> test_set;
    for (int i = 0; i < 50; i++)
    {
        feature_label tempStruct;
        double temp;
        for (int j = 0; j < 4; j++)
        {
            infile >> temp;
            tempStruct.feature_label.push_back(temp);
        }
        infile >> temp;
        tempStruct.label = temp;
        test_set.push_back(tempStruct);
    }

    double countCorrect = 0.0;
    for (int i = 0; i < 50; i++)
    {
        Node* tempNode = root;
        feature_label tempStruct = test_set[i];
        while (isPure(tempNode->getVector()) != true)
        {
            int tempIndex = tempNode->getIndex();
            int tempThreshold = tempNode->getThreshold();
            if (tempStruct.feature_label[tempIndex] <= tempThreshold)
                tempNode = tempNode->getYesLink();
            else
                tempNode = tempNode->getNoLink();
        }
        vector<feature_label> tempVector = tempNode->getVector();
        if (tempVector[0].label == tempStruct.label)
            countCorrect++;
    }

    cout << "The total amount correct = " << countCorrect << endl;
    /*
    cout << root->getName() << root->getThreshold() << endl;
    root = root->getNoLink()->getYesLink()->getNoLink()->getYesLink()->getNoLink()->getYesLink()->getNoLink();
    root2 = root2->getNoLink()->getYesLink()->getNoLink()->getYesLink()->getNoLink()->getYesLink()->getYesLink();
    if (isPure(root->getVector()) == true)
    {
        vector<feature_label> temp = root->getVector();
        cout << "Pure. Label is: " << temp[0].label << " Size is:" << temp.size() << endl;
    }
    else
    {
        cout << root->getName() << root->getThreshold() << " Size is: " << root->getVector().size() << endl;
    }
    if (isPure(root2->getVector()) == true)
    {
        vector<feature_label> temp = root2->getVector();
        cout << "Pure. Label is: " << temp[0].label << " Size is:" << temp.size() << endl;
    }
    else
    {
        cout << root2->getName() << root2->getThreshold() << " Size is: " << root2->getVector().size() << endl;
    }*/
    return 0;
}
