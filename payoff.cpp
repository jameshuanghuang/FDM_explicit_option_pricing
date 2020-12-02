//
//  payoff.cpp
//  design_pattern
//
//  Created by jameshuang on 2020/6/20.
//  Copyright © 2020 jameshuang. All rights reserved.
//

#include "payoff.hpp"
//PayOff is an abstract base class and hence does not need an implementation in the source file
//payoff will be abstract base class, it is only designed for interface


PayOff::PayOff () {}

PayOffCall::PayOffCall(const double& _K) { K = _K; }
// Over−ridden operator () method , which turns PayOffCall into object
double PayOffCall :: operator() (const double& S) const {
    return std::max(S-K, 0.0);
}

PayOffPut::PayOffPut(const double& _K) { K = _K; }
// Over−ridden operator () method , which turns PayOffCall into object
double PayOffPut :: operator() (const double& S) const {
    return std::max(K-S, 0.0);
}

