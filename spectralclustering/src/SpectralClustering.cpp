//
//  SpectralClustering.cpp
//  spectralclustering
//
//  Created by 山口 周悟 on 2016/04/29.
//  Copyright © 2016年 shugo. All rights reserved.
//

#include "SpectralClustering.hpp"


SpectralClustering::SpectralClustering(Eigen::MatrixXf& GraphMatrix)
{
    // solve eigen //
    //SelfAdjointEigenSolver<MatrixXf> es(GraphMatrix);
    es = SelfAdjointEigenSolver<MatrixXf>(GraphMatrix);
    
    // display Matrix, eigen values, eigen vectors //
    cout << "---" << endl;
    cout << GraphMatrix << endl;
    cout << "---" << endl;
    cout << es.eigenvalues() << endl;
    cout << "---" << endl;
    cout << es.eigenvectors() << endl;
}