## pd-purdie
Related to [[shuffle ] object](https://github.com/metamystical/pd-shuffle) for Pure Data.

### Description

[purdie ] outputs a series of the integers in random order without repeating any, then reshuffles. 

Initially the series is empty and bang has no effect. A **reset** message to the leftmost input returns the series to its initial state. 

Add an integer to the series by sending it to the left input. If the integer is already in the series, it is removed from the series until restored by adding it again. In either case, a reshuffle occurs.

Inputs are described in the following table. All selectors can be sent as messages to the left input with arguments as specified:

| Selector     | Input | Message argument | Effect                            |
|--------------|-------|------------------|-----------------------------------|
| **bang**     | left  | none             | output next integer in the series |
| **number**   | left  | float (truncated to integer)  | add an integer to the series and reshuffle |
| **reset**    | left  | none             | message to empty the series |
| **fraction** | right | float            | update **fraction** (0 <= **fraction** <= 0.5) - if nonzero, ensures that the last fraction (portion) of the series is not repeated at the beginning of the next series. Takes effect on the next reshuffle. Default is zero. |

Note: the initial value of **fraction** can be specified within the object box itself [purdie fraction ]. Default is zero.

Note:  Use in combination with [[seq ]](https://github.com/metamystical/pd-seq) to initialize to a range of integers similar to [shuffle ]. Connect the output of [seq ] to the **number** input.

### Installation

The purdie.c file can be compiled into a Pure Data external using [pure-data/pd-lib-builder](https://github.com/pure-data/pd-lib-builder). Simply place the linked file (e.g. purdie.pd_linux) along with the help patch purdie-help.pd into the same directory as your patch, or somewhere on the Pd search path (e.g., externals/purdie folder).
