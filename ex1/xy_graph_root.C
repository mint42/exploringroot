/*	plots f(x) using the TF1 class which is a 1-D grapher
 *
 *	f(x) = sin(x) / x
 *
 * 	x-axis: [0,10]
 * 	y-axis: auto-generated from the output of the function
 * 	
 */

void xy_graph_root() {
	TF1 *f = new TF1("f", "sin(x)/x", 0, 10);
	f->Draw();
}
