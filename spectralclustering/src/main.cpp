//
//  clustering.cpp
//  livecoding
//
//  Created by 山口 周悟 on 2016/04/28.
//  Copyright © 2016年 shugo. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Core>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

void coutvector(std::vector<std::vector<string>> inputnames)
{
    for (int i=0; i<inputnames.size(); i++) {
        for (int j=0; j<inputnames[i].size(); j++) {
            std::cout << inputnames[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool comparestringvec(std::vector<string> vec1, std::vector<string> vec2)
{
    for (int i=0; i<vec1.size(); i++) {
        for (int j=0; j<vec2.size(); j++) {
            if (vec1[i] == vec2[j]) {
                return true;
            }
        }
    }
    return false;
}

bool checkexist(string str, std::vector<string> data)
{
    for (int i=0; i<data.size(); i++) {
        if (str == data[i]) {
            return true;
        }
    }
    return false;
}


int main ()
{
    // input data //
    std::vector<std::vector<string>> inputnames{
        {"山田さん", "山田太郎"},
        {"イチロー", "鈴木"},
        {"a", "b"},
        {"c", "d"},
        {"やまちゃん", "山田太郎"},
        {"やまー", "山田さん"},
        {"鈴木", "鈴木さん"},
        {"0", "1"},
        {"鈴木さん", "鈴木一朗"},
        {"obiwan","ben"},
        {"ben", "kenobi"},
        {"d", "b"},
        {"a", "b"},
        {"2", "1"},
        {"2", "3"}
    };
    
    // cout vector<vector<string>> //
    coutvector(inputnames);
    
    // the number of data //
    int n = (int)inputnames.size();
    
    // Graph Matrix (nxn) //
    Eigen::MatrixXf GraphMatrix = Eigen::MatrixXf::Zero(n,n);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (i==j) continue;
            if (comparestringvec(inputnames[i], inputnames[j])) {
                GraphMatrix(i,j) = -1;
                GraphMatrix(i,i) += 1;
            }
        }
    }
    
    // solve eigen //
    SelfAdjointEigenSolver<MatrixXf> es(GraphMatrix);
    
    // display Matrix, eigen values, eigen vectors //
    cout << "---" << endl;
    cout << GraphMatrix << endl;
    cout << "---" << endl;
    cout << es.eigenvalues() << endl;
    cout << "---" << endl;
    cout << es.eigenvectors() << endl;
    
    std::vector<std::vector<string>> outputnames;
    for (int i=0; i<n; i++)
    {
        // if eigenvalue is larger than 0, skip it //
        float eigenvalue = es.eigenvalues()[i];
        int iev = round(abs(eigenvalue));
        if (iev > 0) continue;
        
        std::vector<string> tempvec;
        auto ev = es.eigenvectors().col(i);
        for (int j=0; j<n; j++) {
            if (abs(ev(j)) > 0.001) { // take non (almost) zero elements
                for (int k=0; k<2; k++) {
                    string temp = inputnames[j][k];
                    if (checkexist(temp, tempvec) == false) { // remove daburi
                        tempvec.push_back(temp);
                    }
                }
            }
        }
        outputnames.push_back(tempvec);
    }
    
    // display the clustering result
    cout << "---" << endl;
    coutvector(outputnames);
    
    return 0;
    
}









