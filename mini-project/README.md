## CALORIMETER DATA PROCESSING

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

And then on the from the interactive shell, to set up a tree instance, run:

```
root> .L tree.C
root> tree t
```

Here are some of the commands available to process the data:


display\_pulses() creates and displays a pdf with 9 graphs for the first 9 events
that have [BLANK] number of pulses using the newly implemented algorithm.
```
root> t.display_pulses(-1);    // all pulses
root> t.display_pulses(0);     // no pulses
root> t.display_pulses(1);     // one pulse
root> t.display_pulses(2);     // two pulses 
root> t.display_pulses(3);     // three pulses 
root> t.display_pulses(4);     // four pulses 
```

compare\_threshold() creates and displays a pdf with a graph showing the number of 
events where [BLANK] pulses were detected VS multiple threshold values using the newly 
implemented algorithm
```
root> t.compare_threshold();
```

compare\_heights() creates and displays a graph showing the relationship between the 
A1 value (pulse height) that comes from the old algorithm VS the integral of the 
pulse from new algorithm
```
root> t.compare_heights();
```

compare\_times() creates and displays a graph showing the relationship between where 
the old and new algorithms place the peak voltage when both algorithms detect only one 
pulse during a given event.
```
root> t.compare_times();
```

Here are a few other examples of commands that can be run on the t object. To see all the options, check out [tree.h](./tree.h)

```
root> t.GetEntry(12); // Fill t data members with entry number 12
root> t.Show();       // Show values of entry 12
root> t.Show(16);     // Read and show values of entry 16
```

## AUTHOR

Riley Reedy
