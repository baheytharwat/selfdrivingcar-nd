/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "helper_functions.h"

using std::string;
using std::vector;
using std::normal_distribution;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

double multiv_prob(double sig_x,double sig_y,double x_obs,double y_obs,double mu_x,double mu_y)
{
 double weight = 0 ;
 double gauss_norm = 1 / (2 * M_PI * sig_x * sig_y);
 double exponent;
 exponent = (pow(x_obs - mu_x, 2) / (2 * pow(sig_x, 2)))
           + (pow(y_obs - mu_y, 2) / (2 * pow(sig_y, 2)));

 weight = gauss_norm * exp(-exponent); 
 return weight ;
}


void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */
  num_particles = 20;  // TODO: Set the number of particles
  std::default_random_engine gen;

  normal_distribution<double> dist_x(x, std[0]);
  normal_distribution<double> dist_y(y, std[1]);
  normal_distribution<double> dist_theta(theta, std[2]);
  
  for (int i = 0; i < num_particles; i++) {
      Particle p;
      p.id = i;
      p.x = dist_x(gen);
      p.y = dist_y(gen);
      p.theta = dist_theta(gen);
      p.weight = 1.0;

      particles.push_back(p);
      weights.push_back(p.weight);
  }
  
	is_initialized = true;

}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */
  
  std::default_random_engine gen;

  double prediction_x, prediction_y, prediction_theta;
  
  for (int i=0; i<num_particles; i++) {
    
    double p_x = particles[i].x;
    double p_y = particles[i].y;
    double p_theta = particles[i].theta;
    
   if (fabs(yaw_rate) < 0.0001) {
    prediction_x = p_x + velocity * cos(p_theta) * delta_t;
    prediction_y = p_y + velocity * sin(p_theta) * delta_t;
    prediction_theta = p_theta;
   }
    else{
    prediction_x = p_x + (velocity/yaw_rate) * ( sin(p_theta + yaw_rate*delta_t) - sin(p_theta) );
    prediction_y = p_y + (velocity/yaw_rate) * ( cos(p_theta) - cos(p_theta + yaw_rate*delta_t) );
    prediction_theta = p_theta + yaw_rate*delta_t;
    }
    
  normal_distribution<double> dist_x(prediction_x, std_pos[0]);
  normal_distribution<double> dist_y(prediction_y, std_pos[1]);
  normal_distribution<double> dist_theta(prediction_theta, std_pos[2]);
    
    particles[i].x = dist_x(gen);
    particles[i].y = dist_y(gen);
    particles[i].theta = dist_theta(gen);
   
    
    }

}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  
   for(unsigned int i = 0; i <observations.size(); i++) 
  {
    double min_distance = std::numeric_limits<double>::infinity();
    double curr_distance = 0;
    int closest_landmark_id = 0;
    
    for (unsigned int j = 0; j < predicted.size(); j++) 
    {
      curr_distance = dist(observations[i].x,observations[i].y, predicted[j].x, predicted[j].y);
      if (curr_distance < min_distance) 
        {
        min_distance = curr_distance;
        closest_landmark_id = j;
        } 
    }
    observations[i].id = closest_landmark_id;
  }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */
  
  
   for(int i=0; i<num_particles; i++)
{
    
  double p_x = particles[i].x;
  double p_y = particles[i].y;
  double p_theta = particles[i].theta;
    
  vector<LandmarkObs> trans_obs_vector;
     
    for (unsigned int i = 0; i <observations.size(); i++)  
    {
      LandmarkObs trans_obs;
      trans_obs.id = observations[i].id;
      trans_obs.x = p_x + (cos(p_theta) * observations[i].x) - (sin(p_theta) *                                     observations[i].y);
      trans_obs.y = p_y + (sin(p_theta) * observations[i].x) + (cos(p_theta) *                                             observations[i].y);
      trans_obs_vector.push_back(trans_obs);
    }
  
  vector<LandmarkObs> selected_landmarks;
  for (unsigned int j = 0; j <map_landmarks.landmark_list.size(); j++) 
   {
      Map::single_landmark_s landmark_j = map_landmarks.landmark_list[j];
      if (dist(p_x, p_y, landmark_j.x_f, landmark_j.y_f) <= sensor_range) 
      {
        selected_landmarks.push_back(LandmarkObs {landmark_j.id_i, landmark_j.x_f, landmark_j.y_f});
      }
    }
  
  dataAssociation(selected_landmarks, trans_obs_vector);
    
  particles[i].weight = 1;
for(unsigned int n = 0; n < trans_obs_vector.size(); n++)
 { 
     double sig_x, sig_y;
     sig_x = std_landmark[0];
     sig_y = std_landmark[1];
     
     double x_obs_tran, y_obs_tran;
     x_obs_tran = trans_obs_vector[n].x;
     y_obs_tran = trans_obs_vector[n].y;
      
     int associateId = trans_obs_vector[n].id;
      
     double mu_x, mu_y;
      
      
     for (unsigned int l = 0; l < map_landmarks.landmark_list.size(); l++) 
       {
         if(map_landmarks.landmark_list[l].id_i == trans_obs_vector[n].id) 
         {
          mu_x = selected_landmarks[associateId].x;
          mu_y = selected_landmarks[associateId].y;
          particles[i].weight *=  multiv_prob(sig_x, sig_y, x_obs_tran, y_obs_tran, mu_x,mu_y);
         }
       }
    }
   
  }
 

}

void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */
  
  
  vector<double> weights;
  double maxWeight = 0;
 
  
  for(int i = 0; i < num_particles; i++) {
    
        weights.push_back(particles[i].weight);
        if ( particles[i].weight > maxWeight ) {
        	maxWeight = particles[i].weight;
       }
  }

  uniform_real_distribution<double> distDouble(0.0, maxWeight);
  uniform_int_distribution<int> distInt(0, num_particles - 1);

  std::default_random_engine gen;
  int j = distInt(gen);
  double beta = 0.0;
  vector<Particle> new_particles;
  
  
  for(int i = 0; i < num_particles; i++) {
    
    beta += distDouble(gen) * 2.0;
    while( beta > weights[j]) {
      beta -= weights[j];
      j = (j + 1) % num_particles;
    }
    new_particles.push_back(particles[j]);
  }

  particles = new_particles;

}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}