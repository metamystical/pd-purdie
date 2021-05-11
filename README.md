## pd-purdie
Enhancement of [[shuffle ] object](https://github.com/metamystical/pd-shuffle) for Pure Data.

### Description

[purdie ] outputs all of the integers in a specified range in random order without repeating any, then reshuffles. Each integer that is presented to the **extra** input will extend the series unless the series already contains it, in which case it will be removed from the series until restored by adding it again.

Inputs are described in the following table:

| Label    | Input | Type    | Action                                                                                                                                 |
|----------|---------|---------|----------------------------------------------------------------------------------------------------------------------------------------|
| **bang**     | 0       | active  | output next integer in the series |
| **lower**    | 1       | active  | update lower bound of range, reset and reshuffle |
| **upper**    | 2       | active | update upper bound of range, reset and reshuffle |
| **fraction** | 3       | active | update **fraction** (0 <= **fraction** <= 0.5) - if nonzero, ensures that the last fraction of the series is not repeated at the beginning of the next series, reset and reshuffle |
| **extra**     | 4       | active  | add an integer to the series, reshuffle |

note: reset causes a loss of memory (loss of all **extras** previously added to the series)

note: the initial values of **lower**, **upper** and **fraction** can be specified within the object box itself [purdie upper lower fraction ]

### Installation

The purdie.c file can be compiled into a Pure Data external using [pure-data/pd-lib-builder](https://github.com/pure-data/pd-lib-builder). Simply place the linked file (e.g. purdie.pd_linux) along with the help patch purdie-help.pd into the same directory as your patch, or somewhere on the Pd search path (e.g., externals/purdie folder).
