#ifndef STIM_NETWORK_H
#define STIM_NETWORK_H

#include <stim/math/vector.h>
#include <stim/visualization/obj.h>
#include <list>
#include <ANN/ANN.h>
#include "fiber.h"
#include <sstream>


namespace stim{
/** This is the a class that interfaces with gl_spider in order to store the currently
 *   segmented network. The following data is stored and can be extracted:
 *   1)Network geometry and centerline.
 *   2)Network connectivity (a graph of nodes and edges), reconstructed using ANN library.
*/


template<typename T>
class network{

	///Each edge is a fiber with two nodes.
	///Each node is an in index to the endpoint of the fiber in the nodes array.
	class edge : public fiber<T>
	{
		public:
		int Node1, Node2;		//unique id's designating the starting and ending

		///default constructor
		edge() : fiber<T>()
		{
			Node1 = -1; Node2 = -1;
		}

		///sized costructor with two known nodes.
		///@param startId: int storing idx assigned to Node1.
		///@param endId: int storing idx assigned to Node2.
		///@param n: int for the number of points in the fiber.
		edge(int startId, int endId, int n)
		 :fiber<T>(n)
		{
			Node1 = startId; Node2 = endId;
		}
		
		///constructor from a std::vector of stim::vecs of positions and radii.
		///@param pos: Vector of stim vecs storing the positions of the fiber.
		///@param mag: Vector of stim vecs storing the radii of the fiber.
		edge(std::vector< stim::vec3<T> > pos, std::vector<T> radii)
			 : fiber<T>(pos, radii)
		{
			Node1 = -1; Node2 = -1;
		}
		
		///constructor from an std::vector of stim::vecs of positions and radii as well as a known starting and ending node.
		///@param pos: Vector of stim vecs storing the positions of the fiber.
		///@param mag: Vector of stim vecs storing the radii of the fiber.
		///@param id1: int storing idx assigned to Node1.
		///@param id2: int storing idx assigned to Node2.
		edge(std::vector< stim::vec3<T> > pos, std::vector<T> radii, int id1, int id2)
			 : fiber<T>(pos, radii)
		{
			Node1 = id1; Node2 = id2;
		}


/*		edge(std::vector< stim::vec<T> > pos, std::vector<T> radii,unsigned int idx)
			 : fiber<T>(pos, radii)
		{
			start = -1; end = -1; id = idx;
		}
*/
		///sets the endpoints to the two int values.
		///@param int id1: index of node1.
		///@param int id2: index of node2.
		void
		setEndpoints(int id1, int id2)
		{
			Node1 = id1; Node2 = id2;
		}

	
			

	};	
	
	///Node class that stores the physical position of the node as well as the edges it is connected to (edges that connect to it), As well as any additional data necessary.
	class node
	{
		private:
			std::vector<int> edges;		//indices of edges connected to this node.
		public:
			stim::vec3<T> p;			//position of this node in physical space.
			//no default constructor

			///@param pos: stim vec with the x, y, z position of the edge.
			///stim::vec constructure with a position but no attached edges.
			node(stim::vec3<T> pos)
			{
				p = pos;
			}

			///@param pos: stim vec with the x, y, z position of the edge.
			///@param i: int i storing the index of an attached edge.
			//stim::vec constructor with a position and an attached edge.
			node(stim::vec3<T> pos, int i)
			{
				p = pos;
				edges.push_back(i);
			}

			///@param x: x coordinate of the node..
			///@param y: y coordinate of the node..
			///@param z: z coordinate of the node..
			//float value constructor. 
			node(T x, T y, T z)
			{
				p = stim::vec3<T>(x,y,z);
			}

			///@param x: x coordinate of the node..
			///@param y: y coordinate of the node..
			///@param z: z coordinate of the node..
			///@param i: int i storing the index of an attached edge.
			//float value constructor with an attached edge.
			node(T x, T y, T z, int i)
			{
				p = stim::vec3<T>(x,y,z);
				edges.push_back(i);
			}

			///@param id: int index of the fiber to attach to this node.
			///adds the fiber to the rest of the fibers connected to this node.
			void
			addEdge(int id)
			{
			 	edges.push_back(id);
			}
	
			///returns the position of the node.
			stim::vec3<T>
			getPosition()
			{	
				return p;
			}

			std::vector<int>
                        getEdges()
                        {       
                                return edges;                                                     
                        }

		
			///@param id: int index of the fiber to detach to this node.
			///removes the edge from the list of the edges attached to this node.
			void
			removeEdge(int id)
			{
				for(int i = 0; i < edges.size(); i++)
				{
					if(edges[i] == id)
						edges.erase(edges.begin()+i);
				}
			}
			
			///returns and std::string with the position of this node.
			std::string
			str()
			{
				return p.str();
			}

			///returns and std::string with the list of edges attached to this node.
			std::string
			edges_to_str()
			{
				std::ostringstream ss;
//				ss << "[";
				for(int i = 0; i < edges.size()-1; i++)
				{
					ss << edges[i] << ";";
				}
				ss << edges[edges.size()-1];
//				ss << "]";
				return ss.str();
			}
			
	};

	public:

	std::vector<edge*> E;       //list of pointers to edges.
	std::vector<node> V;	    //list of nodes.
	
	///Returns the number of edges in the network.
	unsigned int
	sizeE()
	{
		return E.size();
	}

	///Returns the number of nodes in the network.
	unsigned int
	sizeV()
	{
		return V.size();
	}

/*	//adds an edge from two std::vectors with positions and radii.
	void
	addEdge(std::vector< stim::vec<T> > pos, std::vector<stim::vec<T> > radii, int endId)
	{
		
		edge a(pos,radii, endId);
		E.push_back(a);
	} */

	///A complicated method that adds an edge to the network.
	///Most of this functionality will be moved into fiber.
	void
	addEdge(std::vector< stim::vec3<T> > pos, std::vector<T> radii, int startId, int endId)
	{
		float v;	
	//
		if(startId == -1 && endId == -1)
		{
			//case where the edge is neither starting nor ending in a fiber.
			//e.i first fiber.
			
			//Add two nodes to the nodes vector
			V.push_back(node(pos[pos.size()-1]));
			V.push_back(node(pos[0]));

			//the edge will be connected to the nodes
			edge *a = new edge(pos,radii,(V.size()-2), (V.size()-1));
	
			//add fiber to fiber list.
			E.push_back(a);

			//The last two nodes added to V will "own" the last edge added to E.
			V[V.size()-1].addEdge(E.size()-1);
			V[V.size()-2].addEdge(E.size()-1);
			
		}
	
		else if(startId != -1 && endId == -1)
		{
			//case where the edge is starting with a fiber, but not ending in one.
			
			//split the fiber behind you into two.
			unsigned int point = E[startId]->nearest_idx(pos[0]);

			//split the hit fiber at point two parts temp[0], temp[1]
			std::vector < stim::fiber <T> > temp = E[startId]->split(point);
			if(temp.size() > 1)
			{
				//add the nearest point in the behind fiber into the hitting fiber.
				pos.insert(pos.begin(), E[startId]->nearest(pos[0]));
				v= E[startId]->radius(point);
				radii.insert(radii.begin(), v);

				//reset the fiber at the endId to be a shorter fiber(temp[0]).
				std::vector<stim::vec3<T> > ce = temp[0].centerline();
				std::vector<T> cm = temp[0].centerlinemag();

				//remake the edge, such that the starting point of this edge
				//is the same the split point, but the end edge is the old end.
				V.push_back(node(ce[ce.size()-1]));
				int tempNodeId = E[startId]->Node1;
				E[startId] = new edge(ce, cm, (V.size()-1), E[startId]->Node2);
				V[V.size()-1].addEdge(startId);
				

				//add the other part of the fiber (temp[1])
				ce = temp[1].centerline();
				cm = temp[1].centerlinemag();
				E.push_back(new edge(ce, cm,tempNodeId ,(V.size()-1)));
				V[V.size()-1].addEdge(E.size()-1);

				V[tempNodeId].removeEdge(startId);
				V[tempNodeId].addEdge(E.size()-1);
//				V[V.size()-1].removeEdge(startId);

				//make the new hitting fiber..
				V.push_back(node(pos[pos.size()-1]));
				edge *a = new edge(pos, radii, (V.size()-2), (V.size()-1));
				E.push_back(a);
				V[V.size()-1].addEdge(E.size()-1);
				V[V.size()-2].addEdge(E.size()-1);
			} else {
				stim::vec3<T> pz = E[startId]->nearest(pos[0]);
				if((V[ E[startId]->Node1].getPosition() - pz).len() <
					(V[E[startId]->Node2].getPosition() - pz).len())
				{
					unsigned int point = E[startId]->nearest_idx(pos[0]);
					pos.insert(pos.begin(), E[startId]->nearest(pos[0]));
					v = E[startId]->radius(point);
					radii.insert(radii.begin(), v);
					V.push_back(node(pos[pos.size()-1]));
					edge *a = new edge(pos, radii, E[startId]->Node1, (V.size()-1));
					E.push_back(a);
					
					V[V.size()-1].addEdge(E.size()-1);
					V[ E[startId]->Node1].addEdge(E.size()-1);

				} 
				else
				{
					unsigned int point = E[startId]->nearest_idx(pos[0]);
					pos.insert(pos.begin(), E[startId]->nearest(pos[0]));
					v = E[startId]->radius(point);
					radii.insert(radii.begin(), v);
					V.push_back(node(pos[pos.size()-1]));
					edge *a = new edge(pos, radii, E[startId]->Node2, (V.size()-1));
					E.push_back(a);
					
					V[V.size()-1].addEdge(E.size()-1);
					V[ E[startId]->Node2].addEdge(E.size()-1);
				}
			}
		}	

		//case where the edge is starting at a seedpoint but ends in a fiber.
		if(startId == -1 && endId != -1 && endId < sizeE())
		{	
			//split the hit fiber into two.
			unsigned int point = E[endId]->nearest_idx(pos[pos.size() -1]);
			
			//split the hit fiber at point into two parts temp[0], temp[1]
			std::vector < stim::fiber <T> > temp
				 = E[endId]->split(point);
			if(temp.size() > 1)
			{
				//add the nearest point in the hit fiber into the hitting fiber.
				pos.push_back(E[endId]->nearest(pos[pos.size()-1]));
	//			pos.insert(pos.end(), E[endId].nearest(pos[pos.size()-1]));
				v = E[endId]->radius(point);
				radii.push_back(v);

				//split the hit fiber at point into two parts temp[0], temp[1]
				std::vector < stim::fiber <T> > temp
					 = E[endId]->split(point);
				
				//reset the fiber at endId to be a shorter fiber (temp[0]).
				std::vector<stim::vec3<T> > ce = temp[0].centerline();
				std::vector<T> cm = temp[0].centerlinemag();

				//remake the edge, such that the ending point of this edge
				//is the same as before, but the starting edge is new.
				V.push_back(node(ce[ce.size()-1])); //split point.
				int tempNodeId = E[endId]->Node2;
				E[endId] = new edge(ce, cm, E[endId]->Node1, (V.size()-1));
				V[V.size()-1].addEdge(endId);

				//add that other part of the fiber (temp[1])
				//such that it begins with the middle node, and ends with 
				//the last node of the previous fiber.
				ce = temp[1].centerline();
				cm = temp[1].centerlinemag();
				E.push_back(new edge(ce, cm, (V.size()-1), tempNodeId));
				V[V.size()-1].addEdge(E.size()-1);
	//			V[V.size()-1].removeEdge(endId);
				
				
		
				//make the new hitting fiber..
				V.push_back(pos[0]);
				edge *a = new edge(pos,radii,(V.size()-1), (V.size()-2));
				E.push_back(a);
				V[V.size()-1].addEdge(E.size()-1);
				V[V.size()-2].addEdge(E.size()-1);

				//in the end we have added two new nodes and two new edges.
			}
			else {
				stim::vec3<T> pz = E[endId]->nearest(pos[0]);
				if((V[ E[endId]->Node1].getPosition() - pz).len() <
					(V[E[endId]->Node2].getPosition() - pz).len())
				{
					unsigned int point = E[endId]->nearest_idx(pos[0]);
					pos.insert(pos.begin(), E[endId]->nearest(pos[0]));
					v = E[endId]->radius(point);
					radii.insert(radii.begin(), v);
					V.push_back(node(pos[pos.size()-1]));
					edge *a = new edge(pos, radii, E[endId]->Node1, (V.size()-1));
					E.push_back(a);
					
					V[V.size()-1].addEdge(E.size()-1);
					V[ E[endId]->Node1].addEdge(E.size()-1);

				} 
				else
				{
					unsigned int point = E[endId]->nearest_idx(pos[0]);
					pos.insert(pos.begin(), E[endId]->nearest(pos[0]));
					v = E[endId]->radius(point);
					radii.insert(radii.begin(), v);
					V.push_back(node(pos[pos.size()-1]));
					edge *a = new edge(pos, radii, E[endId]->Node2, (V.size()-1));
					E.push_back(a);
					
					V[V.size()-1].addEdge(E.size()-1);
					V[ E[endId]->Node2].addEdge(E.size()-1);
				}
			}
		}

		if(startId != -1 && endId != -1 && endId < sizeE())
		{
			//case where the edge is starting with a fiber, and ends in one.
			
			//split the fiber behind you into two.
			unsigned int point = E[startId]->nearest_idx(pos[0]);
//			std::cout << "in merge to both" << std::endl;

			//split the hit fiber at point two parts temp[0], temp[1]
			std::vector < stim::fiber <T> > temp = E[startId]->split(point);
			if(temp.size() > 1)
			{
				//extend the previous fiber (guaranteed to be added last) by one
				//and add the
				pos = E[E.size()-1]->centerline();
				radii = E[E.size()-1]->centerlinemag();
				pos.insert(pos.begin(), E[startId]->nearest(pos[0]));
				v = E[startId]->radius(point);
				radii.insert(radii.begin(), v);
				V.erase(V.end());
				V.push_back(node(pos[0]));


				//something weird is going on here.
				E[E.size()-1] = new edge(pos, radii, (V.size()-2), (V.size()-1));
				V[V.size()-1].addEdge(E.size()-1);

				//reset the fiber at the endId to be a shorter fiber(temp[0]).
				std::vector<stim::vec3<T> > ce = temp[0].centerline();
				std::vector<T> cm = temp[0].centerlinemag();

//				std::cout << ce.size() << std::endl;
				//remake the edge, such that the starting point of this edge
				//is the same as before, but the end edge is new.
				int tempNodeId = E[startId]->Node1;
				E[startId] = new edge(ce, cm, (V.size()-1), E[startId]->Node2);
				V[V.size()-1].addEdge(startId);

				//add the other part of the fiber (temp[1])
				ce = temp[1].centerline();
				cm = temp[1].centerlinemag();
				E.push_back(new edge(ce, cm,tempNodeId, (V.size()-1)));
				V[V.size()-1].addEdge(E.size()-1);
				V[tempNodeId].removeEdge(startId);
				V[tempNodeId].addEdge(E.size()-1);
//				V[V.size()-1].removeEdge(startId);
			}
			else {
				stim::vec3<T> pz = E[endId]->nearest(pos[0]);
				if((V[ E[endId]->Node1].getPosition() - pz).len() <
					(V[E[endId]->Node2].getPosition() - pz).len())
				{
					unsigned int point = E[endId]->nearest_idx(pos[0]);
					pos.insert(pos.begin(), E[endId]->nearest(pos[0]));
					v= E[endId]->radius(point);
					radii.insert(radii.begin(), v);
					V.push_back(node(pos[pos.size()-1]));
					edge *a = new edge(pos, radii, E[endId]->Node1, (V.size()-1));
					E.push_back(a);
					
					V[V.size()-1].addEdge(E.size()-1);
					V[ E[endId]->Node1].addEdge(E.size()-1);

				} 
				else
				{
					unsigned int point = E[endId]->nearest_idx(pos[0]);
					pos.insert(pos.begin(), E[endId]->nearest(pos[0]));
					v = E[endId]->radius(point);
					radii.insert(radii.begin(), v);
					V.push_back(node(pos[pos.size()-1]));
					edge *a = new edge(pos, radii, E[endId]->Node2, (V.size()-1));
					E.push_back(a);
					
					V[V.size()-1].addEdge(E.size()-1);
					V[ E[endId]->Node2].addEdge(E.size()-1);
				}
			}
		}
		
	}

	///@param pos: std::vector of stim vecs with the positions of the point in the fiber.
	///@param radii: std::vector of floats with the radii of the fiber at positions in pos.
	///adds an edge from two std::vectors with positions and radii.
	void
	addEdge(std::vector< stim::vec3<T> > pos, std::vector<T> radii)
	{
		edge a = edge(pos,radii);
		E.push_back(a);
	}

	///adds an edge from two std::lists with positions and radii.
	/// NOT NECESSARY.
	void
	addEdge(std::list< stim::vec3<T> > pos, std::list<T> radii)
	{
		edge a = edge(pos,radii);
		E.push_back(a);
	}

	///returns the forest as a std::string. For testing only.
	std::string
	edges_to_str()
	{
		std::stringstream ss;
		for(unsigned int i = 0; i < E.size(); i++)
		{
			ss << i << ": " << E[i].str() << std::endl;
		}
		return(ss.str());
	}

	///@param i: index of the required fiber.
	///Returns an std::vector with the centerline of the ith fiber in the edgelist.
	std::vector< stim::vec3<T> >
	getEdgeCenterLine(int i)
	{
		std::vector < stim::vec3<T> > a;
		return E[i]->centerline();
	}

	///@param i: index of the required fiber.
	///Returns an std::vector with the centerline radii of the ith fiber in the edgelist.
	std::vector<T >
	getEdgeCenterLineMag(int i)
	{
		return E[i]->centerlinemag();
	}
	
	///@param i: index of the required fibers nodes..
	///Returns an std::string with the start and end points of this node..
	std::string
	nodes_to_str(int i)
	{
		std::stringstream ss;
		ss << "[from Node " << E[i].Node1 << " to " << E[i].Node2 << "]";
		return ss.str();
	}
	

	///exports the graph.
	void
	to_csv()
	{
		std::ofstream ofs;
		ofs.open("Graph.csv", std::ofstream::out | std::ofstream::app);
		for(int i = 0; i < V.size(); i++)
		{
			ofs << V[i].edges_to_str() << "\n";
		}
		ofs.close();
	}

	///exports the graph.
	void
	to_gdf()
	{

		std::ofstream ofs;
		ofs.open("Graph.gdf", std::ofstream::out | std::ofstream::app);
/*		ofs << "nodedef>name VARCHAR, x FLOAT, y FLOAT, z FLOAT\n";
		for(int i = 0; i < V.size(); i++)
		{
			stim::vec3<float> a = V[i].getPosition();
			ofs << i << "," << a[0] << "," << a[1] << "," << a[2] << "\n";
		}

		ofs << "edgedef>node1 VARCHAR, node2 VARCHAR, weight FLOAT, length INT\n";
		for(int i = 0; i < E.size(); i++)
		{
			float len;
			ofs << E[i]->Node1 << "," << E[i]->Node2 << ","
			 << E[i]->length()  <<E[i]->n_pts() << "\n";
		}  

		ofs.close();
*/
		ofs << "nodedef>name VARCHAR, weight INT, length FLOAT\n";
		for(int i = 0; i < E.size(); i++)
		{
			float len;
			ofs << i << "," <<E[i]->n_pts()
			 << ","<< E[i]->length() << "\n";
		} 
		ofs << "edgedef>node1 VARCHAR, node2 VARCHAR\n";
		for(int i = 0; i < V.size(); i++)
		{
			std::vector<int> temp = V[i].getEdges();
			int num = temp.size();
			switch(num){
				case 1:
					break;
				case 2:
					ofs << temp[0] << "," << temp[1] <<"\n";
					break;
				case 3:
					ofs << temp[0] << "," << temp[1] <<"\n";
					ofs << temp[1] << "," << temp[2] <<"\n";
					ofs << temp[0] << "," << temp[2] <<"\n";
					break;
				case 4:
					ofs << temp[0] << "," << temp[1] <<"\n";
					ofs << temp[1] << "," << temp[2] <<"\n";
					ofs << temp[2] << "," << temp[3] <<"\n";
					ofs << temp[0] << "," << temp[3] <<"\n";
					break;
				case 5:
					ofs << temp[0] << "," << temp[1] <<"\n";
					ofs << temp[1] << "," << temp[2] <<"\n";
					ofs << temp[2] << "," << temp[3] <<"\n";
					ofs << temp[3] << "," << temp[4] <<"\n";
					ofs << temp[0] << "," << temp[4] <<"\n";
					break;
				case 6:
					ofs << temp[0] << "," << temp[1] <<"\n";
					ofs << temp[1] << "," << temp[2] <<"\n";
					ofs << temp[2] << "," << temp[3] <<"\n";
					ofs << temp[3] << "," << temp[4] <<"\n";
					ofs << temp[4] << "," << temp[5] <<"\n";
					ofs << temp[5] << "," << temp[0] <<"\n";
					break;
				
				default:
					std::cout << "WTF happend in network.h" << std::endl;
			}
				
		}

		ofs.close();
	}	
};

};

#endif
