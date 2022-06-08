#ifndef fadc_h
#define fadc_h

// maximum number of pulses possible to find in a given signal
#define MAX_NUM_PULSES 4	
// Programmable Trigger Window: range of samples in a sig[]
// that pulses can be detected [PTW_MIN, PTW_MAX]
#define PTW_MIN	10
#define PTW_MAX	110
// Threshold: a sample must be larger than this point to be
// considered a pulse
#define TET		100
// Number of samples considered in the pedestal.
#define NPED	4
// Number of Samples Above Threshold: minimum number required to be
// considered a pulse
#define NSAT	4
// Number of Samples Before [TC]: used to define the beginning of
// a pulse
#define NSB     2 
// Number of Samples After [TC]: used to define the end of a pulse
#define NSA		8
// 	TC: Threshold Crossed: point at which a pulse first crosses 
// 	the threshold.
// 	it's different for each pulse and is used as a reference point for 
// 	all these other variables.

struct	fadc
{
	Int_t	pedestal;
	Bool_t	pedestal_good;
	Int_t	tc[MAX_NUM_PULSES];
	Int_t	integral[MAX_NUM_PULSES];
	Int_t	time_coarse[MAX_NUM_PULSES];
	Int_t	time_fine[MAX_NUM_PULSES];
	Int_t	vpeak[MAX_NUM_PULSES];
	Int_t	vpeak_time[MAX_NUM_PULSES];
	Float_t	vmid[MAX_NUM_PULSES];
	Int_t	npulses;
};

void	pulseFADC(struct fadc &pulses, Int_t sig[]);

#endif
