/*	plots f(x,y) using the TF2 class which is a 2-D grapher
 *
 *	f(x,y) = sin(x) * sin(y)
 *		     ------   ------
 *				x        y
 *
 * 	x-axis: [-10,10]
 * 	y-axis: [-10,10]
 * 	z-axis: auto-generated from the output of the function
 * 	
 * 	- "surf1" lets the function look 3-D
 */

void xyz_graph_root() {
	TF2 *f = new TF2("f", "(sin(x)*sin(y))/(x*y)", -10, 10, -10, 10);
	f->Draw("surf1");
}
