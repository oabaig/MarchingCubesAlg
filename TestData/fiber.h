#ifndef STIM_FIBER_H
#define STIM_FIBER_H

#include <vector>
#include <stim/math/vec3.h>
#include <ANN/ANN.h>

namespace stim{
		
/**	This class stores information about a single fiber represented as a set of geometric points
 *	between two branch or end points. This class is used as a fundamental component of the stim::network
 *	class to describe an interconnected (often biological) network.
 */
template<typename T>
class fiber{

	unsigned int N;					//number of points in the fiber
	double **c;						//centerline (array of double pointers)

	T* r;						// array of fiber radii
//								// fibers this fiber intersects.
	ANNkd_tree* kdt;				//kd-tree stores all points in the fiber for fast searching


	/// Initialize an empty fiber
	void init(){
		kdt = NULL;
		c=NULL;
		r=NULL;
		N=0;
	}

	/// Initialize a fiber with N centerline points (all located at [0, 0, 0] with radius 0)
	void init(unsigned int n){

		N = n;												//set the number of points
		kdt = NULL;
		c = (double**) malloc(sizeof(double*) * N);			//allocate the array pointer

		for(unsigned int i = 0; i < N; i++)					//allocate space for each point
			c[i] = (double*) malloc(sizeof(double) * 3);
		
		r = (T*) malloc(sizeof(T) * N);			//allocate space for the radii
	}

	/// Copies an existing fiber to the current fiber

	/// @param cpy stores the new copy of the fiber
	void copy( const stim::fiber<T>& cpy ){

		///allocate space for the new fiber
		init(cpy.N);

		///copy the points
		for(unsigned int i = 0; i < N; i++){
			for(unsigned int d = 0; d < 3; d++)		//for each dimension
				c[i][d] = cpy.c[i][d];				//copy the coordinate

			r[i] = cpy.r[i];						//copy the radius
		}

		gen_kdtree();							//generate the kd tree for the new fiber
	}

	void gen_kdtree(){

		//create an array of data points
		int n_data = N;

		ANNpointArray pts = (ANNpointArray)c;			//cast the centerline list to an ANNpointArray

		kdt = new ANNkd_tree(pts, n_data, 3);			//build a KD tree
	}

	double dist(double* p0, double* p1){

		double sum = 0;
		for(unsigned int d = 0; d < 3; d++)
			sum += p0[d] * p1[d];

		return sqrt(sum);
	}

	/// This function retreives the index for the fiber point closest to q

	/// @param q is a reference point used to find the closest point on the fiber center line
	unsigned int ann( stim::vec3<double> q ){

		ANNidxArray idx = new ANNidx[1];			//variable used to hold the nearest point
		ANNdistArray sq_dist = new ANNdist[1];		//variable used to hold the squared distance to the nearest point
		kdt->annkSearch(q.data(), 1, idx, sq_dist);	//search the KD tree for the nearest neighbor

		return *idx;
	}




public:

	fiber(){
		init();
	}

	/// Copy constructor
	fiber(const stim::fiber<T> &obj){

		copy(obj);

	}
	
	//temp constructor for graph visualization
	fiber(int n)
	{
		init(n);
	}

	/// constructor takes a list of points and radii
	fiber(std::vector< stim::vec3< T > > pos, std::vector< T > radii){
		init(pos.size());		//initialize the fiber

		//for each point, set the centerline position and radius
		for(unsigned int i = 0; i < N; i++){

			//set the centerline position
			for(unsigned int d = 0; d < 3; d++)
				c[i][d] = (double) pos[i][d];

			//set the radius
			r[i] = radii[i];
		}

		//generate a kd tree
		gen_kdtree();
	}

	/// This constructor takes a list of points and radii
	fiber(std::list< stim::vec3< T > > pos, std::list< T > radii){

		init(pos.size());								//initialize the array size

		//create an iterator for each list
		typename std::list< stim::vec< T > >::iterator pi = pos.begin();
		typename std::list< T >::iterator ri = radii.begin();

		//create a counter for indexing into the fiber array
		unsigned int i = 0;

		//for each point, set the position and radius
		for(pi = pos.begin(), ri = radii.begin(); pi != pos.end(); pi++, ri++){

			//set the centerline position
			for(unsigned int d = 0; d < 3; d++)
				c[i][d] = (double) (*pi)[d];			
			
			r[i] =  *ri;								//set the radius
			i++;									//increment the array index
		}

		gen_kdtree();								//generate a kd tree

	}


	/// constructor takes an array of points and radii
	//		this function is used when the radii are represented as a stim::vec,
	//		since this may be easier when importing OBJs
	fiber(std::vector< stim::vec3<T> > pos, std::vector< stim::vec<T> > radii){

		init(pos.size());

		//for each point, set the position and radius
		for(unsigned int i = 0; i < N; i++){
			//at(i) = (double*)malloc(sizeof(double) * 3);
			for(unsigned int d = 0; d < 3; d++){
				c[i][d] = (double)  pos[i][d];
			}
			
			r[i] = radii[i][(unsigned int)0];
		}

		gen_kdtree();
	}

	/// Assignment operation
	fiber& operator=(const fiber &rhs){

		if(this == &rhs) return *this;			//test for and handle self-assignment

		copy(rhs);

	}

	///Destructor
	~fiber()
	{
		for(unsigned int i = 0; i < N; i++)
			free(c[i]);
		free(c);
		free(r);
		delete(kdt);
	}


	/// Calculate the length of the fiber and return it.
	T length(){

		double* p0;
		double *p1;
		double l = 0;				//initialize the length to zero

		//for each point
		//typename std::list< point<T> >::iterator i;	//create a point iterator
		for(unsigned int i = 0; i < N; i++){		//for each point in the fiber

			if(i == 0)						//if this is the first point, just store it
				p1 = c[0];
			else{									//if this is any other point
				p0 = p1;							//shift p1->p0
				p1 = c[i];							//set p1 to the new point
				l += dist(p0, p1);				//add the length of p1 - p0 to the running sum
			}
		}

		return (T)l;									//return the length
	}
	
	/// Calculates the length and average radius of the fiber

	/// @param length is filled with the fiber length
	T radius(T& length){

		double* p0;				//temporary variables to store point positions
		double* p1;
		T r0, r1;					//temporary variables to store radii at points
		double l;
		T r_mean;						//temporary variable to store the length and average radius of a fiber segment
		double length_sum = 0;			//initialize the length to zero
		T radius_sum = 0;			//initialize the radius sum to zero

		//for each point
		//typename std::list< point<T> >::iterator i;	//create a point iterator
		for(unsigned int i = 0; i < N; i++){		//for each point in the fiber

			if(i == 0){						//if this is the first point, just store it
				p1 = c[0];
				r1 = r[0];
			}
			else{									//if this is any other point
				p0 = p1;							//shift p1->p0 and r1->r0
				r0 = r1;
				p1 = c[i];							//set p1 to the new point
				r1 = r[i];

				l = dist(p0, p1);				//calculate the length of the p0-p1 segment
				r_mean = (r0 + r1) / 2;					//calculate the average radius of the segment

				radius_sum += r_mean * (T) l;				//add the radius scaled by the length to a running sum
				length_sum += l;					//add the length of p1 - p0 to the running sum
			}
		}

		length = length_sum;						//store the total length

		//if the total length is zero, store a radius of zero
		if(length == 0)
			return 0;
		else
			return (T)(radius_sum / length);					//return the average radius of the fiber
	}
	T average_radius()
	{
		T r_sum = 0.;
		for(unsigned int i = 0; i < N; i++)
		{
			r_sum = r_sum + r[i];	
		}
		return r_sum/((T) N);
	}

	/// Calculates the average radius of the fiber
	T radius(){
		T length;
		return radius(length);
	}

	/// Returns the radius at index idx.
	T radius(int idx){
		return r[idx];
	}

	/// Return the point on the fiber closest to q
	/// @param q is the query point used to locate the nearest point on the fiber centerline
	stim::vec3<T> nearest(stim::vec3<T> q){

		stim::vec3<double> temp( (double) q[0], (double) q[1], (double) q[2]);

		unsigned int idx = ann(temp);		//determine the index of the nearest neighbor

		return stim::vec3<T>((T) c[idx][0], (T) c[idx][1], (T) c[idx][2]);	//return the nearest centerline point
	}

	/// Return the point index on the fiber closest to q
	/// @param q is the query point used to locate the nearest point on the fiber centerline
	unsigned int nearest_idx(stim::vec3<T> q){

		stim::vec3<double> temp( (double) q[0], (double) q[1], (double) q[2]);

		unsigned int idx = ann(temp);		//determine the index of the nearest neighbor

		return idx;	//return the nearest centerline point index
	}

	/// Returns the fiber centerline as an array of stim::vec points
	std::vector< stim::vec3<T> > centerline(){

		//create an array of stim vectors
		std::vector< stim::vec3<T> > pts(N);

		//cast each point to a stim::vec, keeping only the position information
		for(unsigned int i = 0; i < N; i++)
			pts[i] = stim::vec3<T>((T) c[i][0], (T) c[i][1], (T) c[i][2]);

		//return the centerline array
		return pts;
	}

	/// Returns the fiber centerline magnitudes as an array of stim::vec points
	std::vector<T> centerlinemag(){

		//create an array of stim vectors
		std::vector<T> pts(N);

		//cast each point to a stim::vec, keeping only the position information
		for(unsigned int i = 0; i < N; i++){
			pts[i] = r[i];
		}

		//return the centerline array
		return pts;
	}

	/// Split the fiber at the specified index. If the index is an end point, only one fiber is returned
	std::vector< stim::fiber<T> > split(unsigned int idx){

		std::vector< stim::fiber<T> > fl;		//create an array to store up to two fibers

		//if the index is an end point, only the existing fiber is returned
		if(idx == 0 || idx == N-1){
			fl.resize(1);							//set the size of the fiber to 1
			fl[0] = *this;							//copy the current fiber
		}

		//if the index is not an end point
		else{

			unsigned int N1 = idx + 1;					//calculate the size of both fibers
			unsigned int N2 = N - idx;

			fl.resize(2);								//set the array size to 2

			fl[0].init(N1);								//set the size of each fiber
			fl[1].init(N2);

			//copy both halves of the fiber
			unsigned int i, d;

			//first half
			for(i = 0; i < N1; i++){					//for each centerline point
				for(d = 0; d < 3; d++)
					fl[0].c[i][d] = c[i][d];			//copy each coordinate
				fl[0].r[i] = r[i];						//copy the corresponding radius
			}

			//second half
			for(i = 0; i < N2; i++){
				for(d = 0; d < 3; d++)
					fl[1].c[i][d] = c[idx + i][d];
				fl[1].r[i] = r[idx + i];
			}
		}

		return fl;										//return the array

	}

	/// Calculates the set of fibers resulting from a connection between the current fiber and a fiber f

	/// @param f is the fiber that will be connected to the current fiber
	std::vector< stim::fiber<T> > connect( stim::fiber<T> &f, double dist){

		double min_dist;
		unsigned int idx0, idx1;

		//go through each point in the query fiber, looking for the indices for the closest points
		for(unsigned int i = 0; i < f.n_pts(); i++){
			//Run through all points and find the index with the closest point, then partition the fiber and return two fibers.

		}



	}

	/// Outputs the fiber as a string
	std::string str(){
		std::stringstream ss;

		//create an iterator for the point list
		//typename std::list< point<T> >::iterator i;
		for(unsigned int i = 0; i < N; i++){
			ss<<"  [  ";
			for(unsigned int d = 0; d < 3; d++){
				ss<<c[i][d]<<"  ";
			}
			ss<<"]  r = "<<r[i]<<std::endl;
		}

		return ss.str();
	}

	/// Returns the number of centerline points in the fiber
	unsigned int n_pts(){
		return N;
	}
};



}	//end namespace stim



#endif
