# Behavior Trees — Assignment

Refer the repo [GameGuild AI4Games Repo](https://github.com/gameguild-gg/ai4games) for the full project context. And read the [GameGuild AI4Games Course](https://gameguild.gg/p/ai4games2) for detailed explanations.

Implement the missing pieces of a minimal Behavior Tree system for a thief AI.

## What You Need to Implement in `bt.h`

**Composite Nodes:**

- `Selector::tick()` — Try children in order until one returns Success or Running; otherwise return Failure
- `Sequence::tick()` — Run children in order; if any returns Failure or Running, stop and return that status; otherwise return Success

**Leaf Nodes (Conditions):**

- `IsItemNearby::tick()` — Return Success if `m_ctx.itemsNearby > 0`, else Failure
- `NoGuardNearby::tick()` — Return Success if `m_ctx.guardDistance >= 30`, else Failure
- `IsGuardNearby::tick()` — Return Success if `m_ctx.guardDistance < 30`, else Failure

**Leaf Nodes (Actions):**

- `StealItem::tick()` — Decrement `itemsNearby`, increment `stolenItems`, print `"Stole item! Total: <stolenItems>"`, return Success
- `Hide::tick()` — Print `"Hiding"`, return Running
- `Wander::tick()` — Print `"Wandering"`, return Running

**Factory:**

- `createThiefBT(ThiefContext&)` — Build and return the root node of the tree below:

```
Selector (root)
├── Sequence (Steal)
│   ├── IsItemNearby
│   ├── NoGuardNearby
│   └── StealItem
├── Sequence (Flee)
│   ├── IsGuardNearby
│   └── Hide
└── Wander
```

## Input/Output Format

Input: First line `itemsNearby=<int> guardDistance=<int>`, then commands per line:

- `items=<N>` — Update items nearby
- `guard=<N>` — Update guard distance (0-100)
- `tick` — Execute one tree tick and print leaf outputs + `ROOT:<Success|Failure|Running>`

The `runBT()` function is already complete and handles all parsing/printing.
