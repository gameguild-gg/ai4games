#ifndef BT_H
#define BT_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace BT
{

    // ============================================================================
    // HINT: Behavior Trees basics
    // - Status values
    // - Node interface with tick()
    // - Composite nodes: Selector, Sequence
    // - Leaf nodes: Conditions (no Running) and Actions (can Running)
    // ============================================================================

    enum class Status
    {
        Success,
        Failure,
        Running
    };

    class Node
    {
    public:
        virtual ~Node() = default;
        // We will implement tick to drive node behavior and write outputs when needed
        virtual Status tick(std::ostream &out) = 0; // pure virtual
    };

    using NodePtr = std::shared_ptr<Node>;

    // COMPOSITES -----------------------------------------------------------------
    // HINT: Selector = "Try children until one works (OR logic)"
    // Semantics:
    // - Try each child in order
    // - If a child returns Success or Running, STOP and return that status
    // - Only if ALL children return Failure, return Failure
    // Think: "First plan that doesn't fail wins"
    class Selector : public Node
    {
    public:
        void add(NodePtr child) { m_children.push_back(std::move(child)); }

        Status tick(std::ostream &out) override
        {
            // TODO: Implement Selector semantics
            // Loop through children, tick each one
            // If any child returns Success or Running, return immediately
            // If all fail, return Failure
            throw std::runtime_error("Selector::tick not implemented");
        }

    private:
        std::vector<NodePtr> m_children;
    };

    class Sequence : public Node
    {
    public:
        void add(NodePtr child) { m_children.push_back(std::move(child)); }

        Status tick(std::ostream &out) override
        {
            // TODO: Implement Sequence semantics
            // Loop through children, tick each one
            // If any child returns Failure or Running, return immediately
            // If all succeed, return Success
            throw std::runtime_error("Sequence::tick not implemented");
        }

    private:
        std::vector<NodePtr> m_children;
    };

    // CONTEXT --------------------------------------------------------------------
    // The thief state used by leaves
    struct ThiefContext
    {
        int itemsNearby = 0;     // How many stealable items are nearby
        int guardDistance = 100; // Distance to nearest guard (0-100, <30 is "nearby")
        int stolenItems = 0;     // How many items successfully stolen
    };

    // LEAVES ---------------------------------------------------------------------
    class IsItemNearby : public Node
    {
    public:
        explicit IsItemNearby(ThiefContext &ctx) : m_ctx(ctx) {}
        Status tick(std::ostream & /*out*/) override
        {
            // TODO: return Success if m_ctx.itemsNearby > 0, else Failure
            throw std::runtime_error("IsItemNearby::tick not implemented");
        }

    private:
        ThiefContext &m_ctx;
    };

    class NoGuardNearby : public Node
    {
    public:
        explicit NoGuardNearby(ThiefContext &ctx) : m_ctx(ctx) {}
        Status tick(std::ostream & /*out*/) override
        {
            // TODO: return Success if m_ctx.guardDistance >= 30, else Failure
            throw std::runtime_error("NoGuardNearby::tick not implemented");
        }

    private:
        ThiefContext &m_ctx;
    };

    class StealItem : public Node
    {
    public:
        explicit StealItem(ThiefContext &ctx) : m_ctx(ctx) {}
        Status tick(std::ostream &out) override
        {
            // TODO:
            // - decrement itemsNearby by 1
            // - increment stolenItems by 1
            // - print "Stole item! Total: <stolenItems>"
            // - return Success
            throw std::runtime_error("StealItem::tick not implemented");
        }

    private:
        ThiefContext &m_ctx;
    };

    class Wander : public Node
    {
    public:
        Status tick(std::ostream &out) override
        {
            // TODO: Print "Wandering" and return Running
            throw std::runtime_error("Wander::tick not implemented");
        }
    };

    class Hide : public Node
    {
    public:
        Status tick(std::ostream &out) override
        {
            // TODO: Print "Hiding" and return Running
            throw std::runtime_error("Hide::tick not implemented");
        }
    };

    // FACTORY --------------------------------------------------------------------
    // TODO: Build the fixed tree described in README
    // Selector(Root)
    //  ├─ Sequence(Steal)
    //  │   ├─ IsItemNearby
    //  │   ├─ NoGuardNearby
    //  │   └─ StealItem
    //  ├─ Sequence(Flee)
    //  │   ├─ IsGuardNearby
    //  │   └─ Hide
    //  └─ Wander
    // Return the root node
    inline NodePtr createThiefBT(ThiefContext &ctx)
    {
        // Build the tree as specified in README
        auto root = std::make_shared<Selector>();

        // TODO: Build the tree structure
        // Hint: You'll need a Sequence for stealing (check conditions, then steal)
        //       Another Sequence for fleeing (check if guard nearby, then hide)
        //       And a fallback Wander action
        // Example pattern:
        // auto stealSeq = std::make_shared<Sequence>();
        // stealSeq->add(std::make_shared<IsItemNearby>(ctx));
        // ...

        return root;
    }

    // RUNNER ---------------------------------------------------------------------
    // Parse input and execute ticks. See README for expected I/O.
    inline std::string runBT(const std::string &input)
    {
        std::istringstream in(input);
        std::ostringstream out;

        ThiefContext ctx{};

        // Parse first line: itemsNearby=<int> guardDistance=<int>
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
                if (key == "itemsNearby")
                {
                    ctx.itemsNearby = std::stoi(val);
                }
                else if (key == "guardDistance")
                {
                    ctx.guardDistance = std::stoi(val);
                }
            }
        }

        auto root = createThiefBT(ctx);

        // Process remaining lines commands
        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty())
                continue;
            if (line.rfind("items=", 0) == 0)
            {
                try
                {
                    ctx.itemsNearby = std::stoi(line.substr(6));
                }
                catch (...)
                {
                    // ignore bad values
                }
            }
            else if (line.rfind("guard=", 0) == 0)
            {
                try
                {
                    ctx.guardDistance = std::stoi(line.substr(6));
                }
                catch (...)
                {
                    // ignore bad values
                }
            }
            else if (line == "tick")
            {
                Status s = root->tick(out);
                switch (s)
                {
                case Status::Success:
                    out << "ROOT:Success\n";
                    break;
                case Status::Failure:
                    out << "ROOT:Failure\n";
                    break;
                case Status::Running:
                    out << "ROOT:Running\n";
                    break;
                }
            }
            else
            {
                // ignore unknown commands
            }
        }

        return out.str();
    }

} // namespace BT

#endif // BT_H
