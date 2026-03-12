# Ice Shooter 🧊🚀

A high-performance, retro arcade shooter built in **C++** using the Windows Console API. 

## 📝 The Concept
You pilot a high-speed hovercraft across a vast, frozen lake. The game uses custom physics to simulate **momentum and sliding**, requiring the player to counter-steer to avoid colliding with drifting ice boulders.



## 🛠️ Technical Features
* **Drift Physics:** Implemented a linear drag coefficient to simulate ice friction ($1.5f$ deceleration per frame).
* **Wireframe Rendering:** Custom engine logic to rotate and scale 2D coordinate arrays for real-time graphics without a GPU.
* **Procedural Fragmentation:** Ice boulders use a "splitting" algorithm—large entities break into smaller, faster sub-entities upon impact.
* **Smart State Management:** Includes a Start Screen, Pause Menu, and "Game Over" states with high-score tracking.

## 🚀 How to Run (Windows)
To compile the game, you will need a C++ compiler like **MinGW (g++)**.

1. **Clone the repo:**
   ```bash
   git clone [https://github.com/YOUR_USERNAME/Ice-Shooter.git](https://github.com/YOUR_USERNAME/Ice-Shooter.git)
   cd Ice-Shooter
