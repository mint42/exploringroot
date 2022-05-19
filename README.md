# ROOT

[ROOT](https://root.cern) is a C++ framework developed by CERN to be used in the analysis of high energy particle physics experiments. These are a series of little programs I'm writing to familiarize myself with its features.

## RUNNING THE PROGRAMS

The root program will need to be installed in order to run these commands. [Here](https://root.cern/install/) is the install page.

### MACROS

From the command line:

```
root FILENAME.C
```

From the interactive shell:

```
root [0] .x FILENAME.C
```

## OVERVIEW

[ex1](./ex1): ROOT macros. Use the TF1 and TF2 classes to draw some sine functions

[ex2](./ex2): ROOT macro. Uses the THD1 class to create a histogram from some random data points centered around a bell curve

## REFERENCES

[Root Reference Guide](https://root.cern/doc/master/index.html)

[Root Workshop Tutorial](https://inpp.ohio.edu/~rochej/group_page/tips/RootClass2020_seligman_columbia_edu.pdf)

## AUTHOR

Riley Reedy
