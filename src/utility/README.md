# Utility AI — Assignment

Refer the repo [GameGuild AI4Games Repo](https://github.com/gameguild-gg/ai4games) for the full project context. And read the [GameGuild AI4Games Course](https://gameguild.gg/p/ai4games2) for detailed explanations.

Implement the missing pieces of a minimal Utility AI system for a survival game character.

## What You Need to Implement in `utility.h`

**Response Curves:**

- `Linear::evaluate(float x)` — Return `x` unchanged
- `Quadratic::evaluate(float x)` — Return `x * x` (emphasizes high values)
- `InverseQuadratic::evaluate(float x)` — Return `1 - (1-x)²` (emphasizes low values, diminishing returns)
- `Logistic::evaluate(float x)` — Return `1 / (1 + e^(-10*(x-0.5)))` (sharp threshold at midpoint)

**Considerations:**

- `HungerConsideration::evaluate()` — Normalize `hunger` (0-100) to [0,1], apply curve, return score
- `EnergyConsideration::evaluate()` — Normalize `energy` (0-100) to [0,1], apply curve, return score
- `HealthConsideration::evaluate()` — Normalize `health` (0-100) to [0,1], apply curve, return score

**Actions:**

- `EatAction` — Should have high utility when hunger is high
- `SleepAction` — Should have high utility when energy is low
- `ExploreAction` — Should have high utility when health is good and energy is decent
- `RestAction` — Should have high utility when health is low

**Scoring System:**

- `UtilityAI::selectBestAction()` — Evaluate all actions, return the one with highest utility score

## The Character Actions

You need to configure these four actions with appropriate considerations:

```
EatAction:
  - HungerConsideration with InverseQuadratic (high hunger = high score)

SleepAction:
  - EnergyConsideration with InverseQuadratic (low energy = high score)

ExploreAction:
  - HealthConsideration with Quadratic (need good health)
  - EnergyConsideration with Quadratic (need decent energy)

RestAction:
  - HealthConsideration with InverseQuadratic (low health = high score)
```

The key insight: **Inverted** input means low values should score high. For hunger and health checks where low values should trigger actions, normalize as `1.0 - (value / max)`.

## Input/Output Format

Input: First line `hunger=<int> energy=<int> health=<int>`, then commands per line:

- `hunger=<N>` — Update hunger level (0-100, 0=starving, 100=full)
- `energy=<N>` — Update energy level (0-100, 0=exhausted, 100=energized)
- `health=<N>` — Update health level (0-100, 0=dying, 100=perfect)
- `decide` — Select and print best action: `ACTION:<name> UTILITY:<score>` with score formatted to 4 decimal places

Example:

```
hunger=80 energy=30 health=50
decide
hunger=20
decide
```

Output:

```
ACTION:Sleep UTILITY:0.5625
ACTION:Eat UTILITY:0.6400
```

The `runUtilityAI()` function is already complete and handles all parsing/printing.
