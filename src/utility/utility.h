#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Utility
{

    // ============================================================================
    // HINT: Utility AI basics
    // - Response curves transform normalized inputs [0,1] to utility scores [0,1]
    // - Considerations combine context + curve to produce a score
    // - Actions combine multiple considerations (typically via multiplication)
    // - The AI selects the action with highest utility
    // ============================================================================

    // RESPONSE CURVES ------------------------------------------------------------
    // Base class for all response curves
    class ResponseCurve
    {
    public:
        virtual ~ResponseCurve() = default;
        // Takes normalized input [0,1], returns utility score [0,1]
        virtual float evaluate(float x) const = 0;
    };

    // Linear: y = x
    // Uniform response across all values
    class Linear : public ResponseCurve
    {
    public:
        float evaluate(float x) const override
        {
            // TODO: Return x unchanged
            throw std::runtime_error("Linear::evaluate not implemented");
        }
    };

    // Quadratic: y = x²
    // Emphasizes high values, low values matter less
    class Quadratic : public ResponseCurve
    {
    public:
        float evaluate(float x) const override
        {
            // TODO: Return x squared
            throw std::runtime_error("Quadratic::evaluate not implemented");
        }
    };

    // Inverse Quadratic: y = 1 - (1-x)²
    // Rapid initial response, then diminishing returns
    class InverseQuadratic : public ResponseCurve
    {
    public:
        float evaluate(float x) const override
        {
            // TODO: Return 1 - (1-x)²
            throw std::runtime_error("InverseQuadratic::evaluate not implemented");
        }
    };

    // Logistic (Sigmoid): y = 1 / (1 + e^(-k*(x-0.5)))
    // Sharp threshold behavior around midpoint
    class Logistic : public ResponseCurve
    {
    public:
        explicit Logistic(float steepness = 10.0f) : m_k(steepness) {}

        float evaluate(float x) const override
        {
            // TODO: Return 1 / (1 + e^(-k*(x-0.5)))
            // Use std::exp() for the exponential
            throw std::runtime_error("Logistic::evaluate not implemented");
        }

    private:
        float m_k; // Steepness parameter
    };

    // CONTEXT --------------------------------------------------------------------
    // The character state used by considerations
    struct CharacterContext
    {
        int hunger = 0; // 0=starving, 100=full
        int energy = 0; // 0=exhausted, 100=energized
        int health = 0; // 0=dying, 100=perfect
    };

    // CONSIDERATIONS -------------------------------------------------------------
    // Base class for all considerations
    class Consideration
    {
    public:
        explicit Consideration(std::shared_ptr<ResponseCurve> curve) : m_curve(std::move(curve)) {}
        virtual ~Consideration() = default;

        // Evaluate this consideration given current context
        // Returns a score in [0,1]
        virtual float evaluate(const CharacterContext &ctx) const = 0;

    protected:
        std::shared_ptr<ResponseCurve> m_curve;
    };

    // Hunger: High hunger should score high (inverted)
    class HungerConsideration : public Consideration
    {
    public:
        explicit HungerConsideration(std::shared_ptr<ResponseCurve> curve) : Consideration(std::move(curve)) {}

        float evaluate(const CharacterContext &ctx) const override
        {
            // TODO:
            // 1. Normalize hunger (0-100) to [0,1]
            // 2. INVERT it: 1.0 - normalized (so low hunger = high score)
            // 3. Apply the response curve
            // 4. Return the result
            throw std::runtime_error("HungerConsideration::evaluate not implemented");
        }
    };

    // Energy: Low energy should score high (inverted)
    class EnergyConsideration : public Consideration
    {
    public:
        explicit EnergyConsideration(std::shared_ptr<ResponseCurve> curve) : Consideration(std::move(curve)) {}

        float evaluate(const CharacterContext &ctx) const override
        {
            // TODO:
            // 1. Normalize energy (0-100) to [0,1]
            // 2. INVERT it: 1.0 - normalized (so low energy = high score)
            // 3. Apply the response curve
            // 4. Return the result
            throw std::runtime_error("EnergyConsideration::evaluate not implemented");
        }
    };

    // Health: This one is NOT inverted by default - high health = high score
    class HealthConsideration : public Consideration
    {
    public:
        explicit HealthConsideration(std::shared_ptr<ResponseCurve> curve, bool inverted = false)
            : Consideration(std::move(curve)), m_inverted(inverted)
        {
        }

        float evaluate(const CharacterContext &ctx) const override
        {
            // TODO:
            // 1. Normalize health (0-100) to [0,1]
            // 2. If m_inverted is true, invert it: 1.0 - normalized
            // 3. Apply the response curve
            // 4. Return the result
            throw std::runtime_error("HealthConsideration::evaluate not implemented");
        }

    private:
        bool m_inverted;
    };

    // ACTIONS --------------------------------------------------------------------
    // Base class for all actions
    class Action
    {
    public:
        explicit Action(std::string name) : m_name(std::move(name)) {}
        virtual ~Action() = default;

        void addConsideration(std::shared_ptr<Consideration> consideration)
        {
            m_considerations.push_back(std::move(consideration));
        }

        // Calculate utility by combining all considerations
        float calculateUtility(const CharacterContext &ctx) const
        {
            // TODO: Implement IAUS multiplication approach
            // Start with score = 1.0
            // Multiply by each consideration's score
            // If any consideration returns 0, the whole action scores 0 (veto behavior)
            // Return the final score
            throw std::runtime_error("Action::calculateUtility not implemented");
        }

        const std::string &getName() const { return m_name; }

    protected:
        std::string m_name;
        std::vector<std::shared_ptr<Consideration>> m_considerations;
    };

    // Specific actions
    class EatAction : public Action
    {
    public:
        EatAction() : Action("Eat") {}
    };

    class SleepAction : public Action
    {
    public:
        SleepAction() : Action("Sleep") {}
    };

    class ExploreAction : public Action
    {
    public:
        ExploreAction() : Action("Explore") {}
    };

    class RestAction : public Action
    {
    public:
        RestAction() : Action("Rest") {}
    };

    // UTILITY AI SYSTEM ----------------------------------------------------------
    class UtilityAI
    {
    public:
        void addAction(std::shared_ptr<Action> action)
        {
            m_actions.push_back(std::move(action));
        }

        // Select the action with highest utility
        std::shared_ptr<Action> selectBestAction(const CharacterContext &ctx) const
        {
            // TODO:
            // 1. Loop through all actions
            // 2. Calculate utility for each
            // 3. Track the action with highest utility
            // 4. Return that action (or nullptr if no actions)
            throw std::runtime_error("UtilityAI::selectBestAction not implemented");
        }

    private:
        std::vector<std::shared_ptr<Action>> m_actions;
    };

    // FACTORY --------------------------------------------------------------------
    // Build the complete utility AI system with all actions and considerations
    inline std::unique_ptr<UtilityAI> createSurvivalAI()
    {
        auto ai = std::make_unique<UtilityAI>();

        // TODO: Create and configure all four actions
        //
        // EatAction:
        //   - Add HungerConsideration with InverseQuadratic curve
        //
        // SleepAction:
        //   - Add EnergyConsideration with InverseQuadratic curve
        //
        // ExploreAction:
        //   - Add HealthConsideration (NOT inverted) with Quadratic curve
        //   - Add EnergyConsideration (NOT inverted) with Quadratic curve
        //
        // RestAction:
        //   - Add HealthConsideration (inverted=true) with InverseQuadratic curve
        //
        // Example pattern:
        // auto eatAction = std::make_shared<EatAction>();
        // eatAction->addConsideration(
        //     std::make_shared<HungerConsideration>(std::make_shared<InverseQuadratic>())
        // );
        // ai->addAction(eatAction);

        return ai;
    }

    // RUNNER ---------------------------------------------------------------------
    // Parse input and execute decisions. See README for expected I/O.
    inline std::string runUtilityAI(const std::string &input)
    {
        std::istringstream in(input);
        std::ostringstream out;

        CharacterContext ctx{};

        // Parse first line: hunger=<int> energy=<int> health=<int>
        std::string firstLine;
        if (!std::getline(in, firstLine))
        {
            return out.str();
        }
        {
            std::istringstream init(firstLine);
            std::string token;
            while (init >> token)
            {
                auto pos = token.find('=');
                if (pos == std::string::npos)
                    continue;
                auto key = token.substr(0, pos);
                auto val = token.substr(pos + 1);
                if (key == "hunger")
                {
                    ctx.hunger = std::stoi(val);
                }
                else if (key == "energy")
                {
                    ctx.energy = std::stoi(val);
                }
                else if (key == "health")
                {
                    ctx.health = std::stoi(val);
                }
            }
        }

        auto ai = createSurvivalAI();

        // Process remaining line commands
        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty())
                continue;
            if (line.rfind("hunger=", 0) == 0)
            {
                try
                {
                    ctx.hunger = std::stoi(line.substr(7));
                }
                catch (...)
                {
                    // ignore bad values
                }
            }
            else if (line.rfind("energy=", 0) == 0)
            {
                try
                {
                    ctx.energy = std::stoi(line.substr(7));
                }
                catch (...)
                {
                    // ignore bad values
                }
            }
            else if (line.rfind("health=", 0) == 0)
            {
                try
                {
                    ctx.health = std::stoi(line.substr(7));
                }
                catch (...)
                {
                    // ignore bad values
                }
            }
            else if (line == "decide")
            {
                auto bestAction = ai->selectBestAction(ctx);
                if (bestAction)
                {
                    float utility = bestAction->calculateUtility(ctx);
                    out << "ACTION:" << bestAction->getName() << " UTILITY:" << std::fixed
                        << std::setprecision(4) << utility << "\n";
                }
            }
            else
            {
                // ignore unknown commands
            }
        }

        return out.str();
    }

} // namespace Utility

#endif // UTILITY_H
