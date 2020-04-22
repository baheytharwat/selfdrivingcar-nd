#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  
  

}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  static double integ_cte = 0;
  static double prev_cte = cte;
  double diff_cte;
  
  integ_cte += cte;
  diff_cte = cte - prev_cte;
  
  p_error = Kp * cte;
  
  i_error = Ki * integ_cte;
  
  d_error = Kd * diff_cte;
  
  prev_cte = cte;
  
  
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  double t = (p_error + i_error + d_error) * -1;
  return t;  // TODO: Add your total error calc here!
}