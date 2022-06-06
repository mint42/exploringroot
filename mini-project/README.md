## Calorimeter Data Processing

The goal of this project is to use some sample data taken from a collision where photons were being produced, and analyze those signals by loosely recreating an algorithm used in [Jeffereson Lab](https://www.jlab.org/)'s detectors. [Here](https://coda.jlab.org/drupal/system/files/pdfs/HardwareManual/fADC250/FADC250_Processing_FPGA_Firmware_ver_0x0C0D_Description_Instructions.pdf) is the documentation on that algorithm.

This new algorithm will then be used to compare against the old algorithm for processing this data.

This project was given to me in an unfinished form, and my job is to clean it, finish it, and bug check it.

This program is written in C++ using the ROOT framework.

## RUNNING THE PROGRAM

The root program will need to be installed in order to run these commands. [Here](https://root.cern/install/) is the install page.

From the command line, first run:

```
root tree2.root
```

And then on the from the interactive shell, run:

```
root> .L tree.C
root> tree t
```

to set up the class.

Each of the following commands runs the main of the program and generates graphs of signals for the first 9 instances of 0, 1, or 2 valid pulses detected. Giving the Loop() method different parameters can limit the number of graphs generated.

```
root> t.Loop();       // Prints all multigraphs
root> t.Loop(0);      // Prints no pulse multigraph
root> t.Loop(1);      // Prints one pulse multigraph
root> t.Loop(2);      // Prints two pulse multigraph
```

Here are a few other examples of commands that can be run on the t object. To see all the options, check out [tree.h](./tree.h)

```
root> t.GetEntry(12); // Fill t data members with entry number 12
root> t.Show();       // Show values of entry 12
root> t.Show(16);     // Read and show values of entry 16
```

## AUTHOR

Riley Reedy
