#ifndef __FDM_CPP
#define __FDM_CPP

#include <fstream>
#include "FDM.hpp"
#include <iostream>


FDMBase::FDMBase(double _x_dom, unsigned long _J,
                 double _t_dom, unsigned long _N,
                 ConvectionDiffusionPDE* _pde) 
  : x_dom(_x_dom), J(_J), t_dom(_t_dom), N(_N), pde(_pde) {}

FDMEulerExplicit::FDMEulerExplicit(double _x_dom, unsigned long _J,
                                   double _t_dom, unsigned long _N,
                                   ConvectionDiffusionPDE* _pde) 
  : FDMBase(_x_dom, _J, _t_dom, _N, _pde) {
  calculate_step_sizes();
  set_initial_conditions();
}

void FDMEulerExplicit::calculate_step_sizes() {
  dx = x_dom/static_cast<double>(J-1);
  dt = t_dom/static_cast<double>(N-1);
}

void FDMEulerExplicit::set_initial_conditions() {
  // Spatial settings
  double cur_spot = 0.0;

  old_result.resize(J, 0.0);
  new_result.resize(J, 0.0);
  x_values.resize(J, 0.0);

  for (unsigned long j=0; j<J; j++) {
    cur_spot = static_cast<double>(j)*dx;
    old_result[j] = pde->init_cond(cur_spot);
    x_values[j] = cur_spot;
  }

  // Temporal settings
  prev_t = 0.0;
  cur_t = 0.0;
}

void FDMEulerExplicit::calculate_boundary_conditions() {
  new_result[0] = pde->boundary_left(prev_t, x_values[0]);
  new_result[J-1] = pde->boundary_right(prev_t, x_values[J-1]);
}

void FDMEulerExplicit::calculate_inner_domain() {
  // Only use inner result indices (1 to J-2)
  double r = -pde->zero_coeff(prev_t, x_values[1]);
  double sig2 = r*r*2.0*(pde->diff_coeff(prev_t, x_values[1]))/(pde->conv_coeff(prev_t, x_values[1])*pde->conv_coeff(prev_t, x_values[1]));
  double mul = 1.0+r*dt;
  for (unsigned long j=1; j<J-1; j++) {
    // Differencing coefficients
    alpha = 0.5*dt*(sig2*j*j-r*j)/mul;
    beta = (1.0-sig2*j*j*dt)/mul;
    gamma = 0.5*dt*(sig2*j*j+r*j)/mul;

    // Update inner values of spatial discretisation grid (Explicit Euler)
    new_result[j] = ( (alpha * old_result[j-1]) + 
                      (beta * old_result[j]) + 
                      (gamma * old_result[j+1]) );
  }
}

void FDMEulerExplicit::step_march() { 
  std::ofstream fdm_out("fdm.csv");

  while(cur_t < t_dom) {
    cur_t = prev_t + dt;
    calculate_boundary_conditions();
    calculate_inner_domain();
    for (int j=0; j<J; j++) {
      fdm_out << x_values[j] << " " << prev_t << " " << new_result[j] << std::endl;
    }
    
    old_result = new_result;
    prev_t = cur_t;
  }

  fdm_out.close();
}

#endif















