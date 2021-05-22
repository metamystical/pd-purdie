## pd-purdie
Related to [[shuffle ] object](https://github.com/metamystical/pd-shuffle) for Pure Data.

### Description

[purdie ] outputs a series of the integers in random order without repeating any, then reshuffles. 

Initially the series is empty and bang has no effect. A reset message to the leftmost input returns the series to its initial state. 

Add an integer to the series by sending it to the leftmost input. If the integer is alread in the series, it will be removed from the series until restored by adding it again. In either case, a reshuffle occurs.

Inputs are described in the following table:

| Label    | Input | Type    | Action                                                                                                                                 |
|----------|---------|---------|----------------------------------------------------------------------------------------------------------------------------------------|
| **bang**     | 0       | active   | output next integer in the series |
| **integer**   | 0       | passive  | add an integer to the series and reshuffle |
| **reset**    | 0       | passive  | empty the series |
| **fraction** | 1       | passive  | update **fraction** (0 <= **fraction** <= 0.5) - if nonzero, ensures that the last fraction (portion) of the series is not repeated at the beginning of the next series. Takes effect on the next reshuffle. Default is zero. |

note: the initial values of **fraction** can be specified within the object box itself [purdie fraction ]. Default is zero.

### Installation

The purdie.c file can be compiled into a Pure Data external using [pure-data/pd-lib-builder](https://github.com/pure-data/pd-lib-builder). Simply place the linked file (e.g. purdie.pd_linux) along with the help patch purdie-help.pd into the same directory as your patch, or somewhere on the Pd search path (e.g., externals/purdie folder).
