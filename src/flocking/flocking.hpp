// do not use AI.
#include "vector2.hpp"
#include <vector>
#include <iomanip>

struct Boid {
  Boid(const Vector2& pos, const Vector2& vel): position(pos), velocity(vel){};
  Boid():position({0,0}), velocity({0,0}){};
  double weight = 1.0; // all boids weights 1 for simplicity, you can discard this in your math
  Vector2 position;
  Vector2 velocity;
};

struct Cohesion {
  double radius;
  double k;

  Cohesion(double radius, double k): radius(radius), k(k){};

  /**
   * Computes the cohesion force for a given boid.
   * 
   * Cohesion is the tendency of boids to move toward the center of mass of their neighbors.
   * This function should:
   * 1. Find all neighboring boids within the cohesion radius (excluding the boid itself)
   * 2. Calculate the center of mass (average position) of these neighbors
   * 3. Compute a force vector pointing from the boid's current position toward the center of mass
   * 4. Normalize this force vector to get the direction
   * 5. Scale the normalized force by the cohesion strength parameter 'k' and return it
   * 
   * If no neighbors are found within the radius, return a zero force vector.
   * 
   * @param boids The complete list of all boids in the simulation
   * @param boidAgentIndex The index of the boid for which to compute the cohesion force
   * @return Vector2 The cohesion force to be applied to the boid
   */
  Vector2 ComputeForce(const std::vector<Boid>& boids, int boidAgentIndex) {
    // TODO: Implement cohesion force calculation
    return {0, 0};
  }
};

struct Alignment {
  double radius;
  double k;

  Alignment(double radius, double k): radius(radius), k(k){};

  /**
   * Computes the alignment force for a given boid.
   * 
   * Alignment is the tendency of boids to align their velocity with their neighbors.
   * This function should:
   * 1. Find all neighboring boids within the alignment radius (including the boid itself)
   * 2. Calculate the average velocity of these neighbors
   * 3. Compute a force vector that represents the desired velocity change
   * 4. Scale this force by the alignment strength parameter 'k'
   * 5. Return the scaled average velocity as the alignment force
   * 
   * Note: Unlike cohesion and separation, alignment typically includes the boid itself
   * in the neighbor calculation, as it contributes to the local velocity average.
   * 
   * If no neighbors are found within the radius, return a zero force vector.
   * 
   * @param boids The complete list of all boids in the simulation
   * @param boidAgentIndex The index of the boid for which to compute the alignment force
   * @return Vector2 The alignment force to be applied to the boid
   */
  Vector2 ComputeForce(const std::vector<Boid>& boids, int boidAgentIndex) {
    // TODO: Implement alignment force calculation
    return {0, 0};
  }
};

struct Separation {
  double radius;
  double k;
  // if the computed force is greater than maxForce, we clip it to maxForce
  double maxForce;

  Separation(double radius, double k, double maxForce): radius(radius), k(k), maxForce(maxForce){};

  /**
   * Computes the separation force for a given boid.
   * 
   * Separation is the tendency of boids to avoid crowding and maintain distance from neighbors.
   * This function should:
   * 1. Find all neighboring boids within the separation radius (excluding the boid itself)
   * 2. For each neighbor, calculate a repulsion force inversely proportional to distance
   * 3. The force direction should point away from each neighbor (from neighbor to current boid)
   * 4. The force magnitude should be k/distance, where k is the separation strength parameter
   * 5. Sum all individual separation forces to get the total separation force
   * 6. If the total force magnitude exceeds maxForce, clamp it to maxForce while preserving direction
   * 7. Return the final separation force vector
   * 
   * Important notes:
   * - Force is inversely proportional to distance: closer boids create stronger repulsion
   * - Avoid division by zero when distance is very small
   * - The maxForce parameter prevents excessive forces that could destabilize the simulation
   * 
   * If no neighbors are found within the radius, return a zero force vector.
   * 
   * @param boids The complete list of all boids in the simulation
   * @param boidAgentIndex The index of the boid for which to compute the separation force
   * @return Vector2 The separation force to be applied to the boid (clamped to maxForce)
   */
  Vector2 ComputeForce(const std::vector<Boid>& boids, int boidAgentIndex) {
    // TODO: Implement separation force calculation
    return {0, 0};
  }
};

struct Flocking {
private:
  Cohesion cohesion;
  Alignment alignment;
  Separation separation;

  // double buffering. to generate a new state, we only use the data from the current state. when the new state is generated, swap them and repeat next frame
  std::vector<Boid> currentState, newState;

public:
  // default constructor
  Flocking(): cohesion(0, 0), alignment(0, 0), separation(0, 0, 0){};
  Flocking(double cohesionRadius, double separationRadius, double separationMaxForce, double alignmentRadius, double cohesionK, double separationK, double alignmentK, std::vector<Boid> boids): 
  cohesion(cohesionRadius, cohesionK), alignment(alignmentRadius, alignmentK), separation(separationRadius, separationK, separationMaxForce), currentState(boids), newState(boids){};

  /**
   * Performs one simulation step for the flocking system.
   * 
   * This function advances the simulation by one time step using numerical integration.
   * The implementation should follow these steps:
   * 
   * 1. Force Computation Phase:
   *    - For each boid in the current state, compute the three flocking forces:
   *      a) Cohesion force (tendency to move toward center of mass of neighbors)
   *      b) Alignment force (tendency to align velocity with neighbors)
   *      c) Separation force (tendency to avoid crowding with neighbors)
   *    - Sum all three forces to get the total force acting on each boid
   * 
   * 2. Integration Phase:
   *    - Update each boid's velocity using: new_velocity = old_velocity + total_force * deltaTime
   *    - Update each boid's position using: new_position = old_position + new_velocity * deltaTime
   *    - Store the updated boid state in the newState buffer
   * 
   * 3. Buffer Swap Phase:
   *    - Swap the currentState and newState buffers to prepare for the next simulation step
   *    - This double-buffering ensures that force calculations use consistent state data
   * 
   * Important notes:
   * - All force calculations should use the currentState data (not newState)
   * - Position and velocity updates should be written to newState
   * - The deltaTime parameter controls the simulation time step size
   * - Smaller deltaTime values provide more accurate but computationally expensive simulation
   * 
   * @param deltaTime The time step size for numerical integration (in simulation time units)
   */
  void Step(double deltaTime) {
    // TODO: Implement simulation step
    // 1. Compute forces for each boid
    // 2. Update velocities and positions using numerical integration
    // 3. Swap state buffers
  }

  std::vector<Boid>& GetCurrentState() {
    return currentState;
  }
};

struct Simulator {
private:
  Flocking flocking;
  
  // stream can be a file or cin
  std::istream& stream;
  std::vector<std::vector<Boid>> states;
  
public:
  Simulator(std::istream& stream): stream(stream) {
    double cohesionRadius, separationRadius, separationMaxForce, alignmentRadius, cohesionK, separationK, alignmentK;
    int numberOfBoids;
    stream >> cohesionRadius >> separationRadius >> separationMaxForce >> alignmentRadius >> cohesionK >> separationK >> alignmentK >> numberOfBoids;
    std::vector<Boid> boids;
    for (int i = 0; i < numberOfBoids; i++) {
      Boid b;
      stream >> b.position.x >> b.position.y >> b.velocity.x >> b.velocity.y;
      boids.push_back(b);
    }
    flocking = Flocking(cohesionRadius, separationRadius, separationMaxForce, alignmentRadius, cohesionK, separationK, alignmentK, boids);
  }

  void Run() {
    double deltaTime;
    while (stream >> deltaTime) {
      flocking.Step(deltaTime);
      states.push_back(flocking.GetCurrentState());
    }
  }

  std::vector<std::vector<Boid>> GetStates() {
    return states;
  }

  void OutputStates(std::ostream& out) const {
    out << std::fixed << std::setprecision(3);
    for (const auto& state : states) {
      for (auto boid : state) {
        out << boid.position.x << " " << boid.position.y << " " << boid.velocity.x << " " << boid.velocity.y << std::endl;
      }
    }
  }
};