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

Make sure to run the commands from inside the appropriate project foler so that there are no path resolving errors.

## OVERVIEW

[ex1](./ex1): ROOT macros. Use the TF1 and TF2 classes to draw some sine functions

[ex2](./ex2): ROOT macro. Uses the TH1D class to create a histogram from some random data points centered around a bell curve

[ex3](./ex3): ROOT macro. Uses TTree class and TTreeReader class to run a calculation on a example root file for a fake collision experiment. It finds the average magnitude of momentum from the set of all events. Credit for the [experiment.root](/ex3/experiment.root) file is in references.

## REFERENCES

[Root Reference Guide](https://root.cern/doc/master/index.html): Here you can find a list of all the classes and namespaces in the ROOT framework along with definitions, use cases, examples, and tutorials.

[Root Official Guide on Trees](https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html)

[Root Workshop Tutorial](https://www.nevis.columbia.edu/~seligman/root-class/)

[Bill Seligman's Site](https://www.nevis.columbia.edu/~seligman/): A general thank you to the creator of the Root Tutorial I used in this project. He has provided his entire tutorial and all the example files (prev REF) for free to the public.

## AUTHOR

Riley Reedy
