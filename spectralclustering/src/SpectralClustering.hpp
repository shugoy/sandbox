//
//  SpectralClustering.hpp
//  spectralclustering
//
//  Created by 山口 周悟 on 2016/04/29.
//  Copyright © 2016年 shugo. All rights reserved.
//

#ifndef SpectralClustering_hpp
#define SpectralClustering_hpp

#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Core>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

class SpectralClustering
{
private:
    SelfAdjointEigenSolver<MatrixXf> es;
    
public:
    SpectralClustering(Eigen::MatrixXf& GraphMatrix);
    
};


#endif /* SpectralClustering_hpp */
