# Spoiled Linked List

This doc covers the strategy used to ensure easy and complete coverage of all conditions

We flip from fresh ranges to spoiled ranges for two reasons:
- starting with a head node containing min and max values simplifies initializing the linked list
- going to the outside of each range ensures **`lp` and `rp` can never be equal** and simplifies linked list operations


We relate `lp` and `rp` each to a node,
which means there are only three initial node conditions:
- same node
- neighbors
- separated

Finally to be thorough, we **operate from left to right**

## Same Node

| | rp < RL | rp == RL | rp < RR | rp == RR |
| --: | --- | --- | --- | --- |
| __lp < LL__ | lp, rp (L, R) | lp (rp/L, R) | lp (L, rp, R) | lp (L, rp/R) |
| __lp == LL__ | | ~~(lp/rp/L, R)~~ | (lp/L, rp, R) | (lp/L, rp/R) |
| __lp < LR__ | | | (L, lp, rp, R) | (L, lp, rp/R) |
| __lp == LR__ | | | | ~~(L, lp/rp/R)~~ |

### Stage 1: account for every possible condition
```
if lp < LL and rp < RL:
    do nothing
if lp < LL and rp == RL:
    do nothing
if lp < LL and rp < RR:
    RL = rp
if lp < LL and rp == RR:
    RL = rp

if lp == LL and rp < RR:
    split node
    LR = lp
    RL = rp
if lp == LL and rp == RR:
    split node
    LR = lp
    RL = rp

if lp < LR and rp < RR:
    split node
    LR = lp
    RL = rp
if lp < LR and rp == RR:
    split node
    LR = lp
    RL = rp
```

### Stage 2: collapse conditions with identical instructions
```
if lp < LL and rp <= RL:
    do nothing
if lp < LL and rp <= RR:
    RL = rp

if lp < LR and rp <= RR:
    split node
    LR = lp
    RL = rp
```

### Stage 3: simplify common instructions and clarify function returns
```
if rp <= RL:
    return
if lp < LL and rp <= RR:
    RL = rp
    return

split node
LR = lp
RL = rp
```

## Neighbor Nodes

| | rp < RL | rp == RL | rp < RR | rp == RR |
| --: | --- | --- | --- | --- |
| __lp < LL__ | lp (L, R) rp (L, R) | lp (L, R) (rp/L, R) | lp (L, R) (L, rp, R) | lp (L, R) (L, rp/R) |
| __lp == LL__ | (lp/L, R) rp (L, R) | (lp/L, R) (rp/L, R) | (lp/L, R) (L, rp, R) | (lp/L, R) (L, rp/R) |
| __lp < LR__ | (L, lp, R) rp (L, R) | (L, lp, R) (rp/L, R) | (L, lp, R) (L, rp, R) | (L, lp, R) (L, rp/R) |
| __lp == LR__ | (L, lp/R) rp (L, R) | (L, lp/R) (rp/L, R) | (L, lp/R) (L, rp, R) | (L, lp/R) (L, rp/R) |

### Stage 1: account for every possible condition
```
if lp < LL and rp < RL:
    stitch left prev to right
if lp < LL and rp == RL:
    stitch left prev to right
if lp < LL and rp < RR:
    stitch left prev to right
    RL = rp
if lp < LL and rp == RR:
    stich left prev to right
    RL = rp

if lp == LL and rp < RL:
    LR = lp
if lp == LL and rp == RL:
    LR = lp
if lp == LL and rp < RR:
    LR = lp
    RL = rp
if lp == LL and rp == RR:
    LR = lp
    RL = rp

if lp < LR and rp < RL:
    LR = lp
if lp < LR and rp == RL:
    LR = lp
if lp < LR and rp < RR:
    LR = lp
    RL = rp
if lp < LR and rp == RR:
    LR = lp
    RL = rp

if lp == LR and rp < RL:
    do nothing
if lp == LR and rp == RL:
    do nothing
if lp == LR and rp < RR:
    RL = rp
if lp == LR and rp == RR:
    RL = rp
```

### Stage 2: collapse conditions with identical instructions
```
if lp < LL and rp <= RL:
    stitch left prev to right
if lp < LL and rp <= RR:
    stitch left prev to right
    RL = rp

if lp < LR and rp <= RL:
    LR = lp
if lp < LR and rp <= RR:
    LR = lp
    RL = rp

if lp == LR and rp <= RL:
    do nothing
if lp == LR and rp <= RR:
    RL = rp
```

### Stage 3: simplify common instructions and clarify function returns
```
if lp < LL:
    stitch left prev to right
else:
    LR = lp

if rp <= RL:
    return

RL = rp
```

## Separated Nodes

| | rp < RL | rp == RL | rp < RR | rp == RR |
| --: | --- | --- | --- | --- |
| __lp < LL__ | lp (L, R) (L, R) rp (L, R) | lp (L, R) (L, R) (rp/L, R) | lp (L, R) (L, R) (L, rp, R) | lp (L, R) (L, R) (L, rp/R) |
| __lp == LL__ | (lp/L, R) (L, R) rp (L, R) | (lp/L, R) (L, R) (rp/L, R) | (lp/L, R) (L, R) (L, rp, R) | (lp/L, R) (L, R) (L, rp/R) |
| __lp < LR__ | (L, lp, R) (L, R) rp (L, R) | (L, lp, R) (L, R) (rp/L, R) | (L, lp, R) (L, R) (L, rp, R) | (L, lp, R) (L, R) (L, rp/R) |
| __lp == LR__ | (L, lp/R) (L, R) rp (L, R) | (L, lp/R) (L, R) (rp/L, R) | (L, lp/R) (L, R) (L, rp, R) | (L, lp/R) (L, R) (L, rp/R) |

### Stage 1: account for every possible condition

```
if lp < LL and rp < RL:
    stitch left prev to right
if lp < LL and rp == RL:
    stitch left prev to right
if lp < LL and rp < RR:
    stitch left prev to right
    RL = rp
if lp < LL and rp == RR:
    stich left prev to right
    RL = rp

if lp == LL and rp < RL:
    stitch left to right
    LR = lp
if lp == LL and rp == RL:
    stitch left to right
    LR = lp
if lp == LL and rp < RR:
    stitch left to right
    LR = lp
    RL = rp
if lp == LL and rp == RR:
    stitch left to right
    LR = lp
    RL = rp

if lp < LR and rp < RL:
    stitch left to right
    LR = lp
if lp < LR and rp == RL:
    stitch left to right
    LR = lp
if lp < LR and rp < RR:
    stitch left to right
    LR = lp
    RL = rp
if lp < LR and rp == RR:
    stitch left to right
    LR = lp
    RL = rp

if lp == LR and rp < RL:
    stitch left to right
if lp == LR and rp == RL:
    stitch left to right
if lp == LR and rp < RR:
    stitch left to right
    RL = rp
if lp == LR and rp == RR:
    stitch left to right
    RL = rp
```

### Stage 2: collapse conditions with identical instructions
```
if lp < LL and rp <= RL:
    stitch left prev to right
if lp < LL and rp <= RR:
    stitch left prev to right
    RL = rp

if lp < LR and rp <= RL:
    stitch left to right
    LR = lp
if lp < LR and rp <= RR:
    stitch left to right
    LR = lp
    RL = rp

if lp == LR and rp <= RL:
    stitch left to right
if lp == LR and rp <= RR:
    stitch left to right
    RL = rp
```

### Stage 3: simplify common instructions and clarify function returns
```
if lp < LL:
    stitch left prev to right
else:
    stitch left to right
    LR = lp

if rp <= RL:
    return

RL = rp
```