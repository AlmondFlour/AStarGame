//=======================
//=====Mostly psuedocode
//=======================

struct Node
{
	int distance;
	
	
	Node * parent;
	
	bool closed;
};


void AStar( GameObj* obj, Node start, Node end )
{
	//Nodes to be checked
	vector< Node> openList;
	
	//Nodes part of the path( may be problems with variable costs )
	vector< Node> closedList;
	
	//set initial values for start
	start.cost = 0;
	start.hcost = 0;
	
	start.parent = NULL;
	
	//Add starting node
	openList.push_back( node );
	
	//Loop until reach the end or out of nodes
	while( (!openList.empty()) || ( openList.back() != end ) )
	{
		//find the node with lowest cost
		Node * current = &std::min_element(openList.begin(), openlist.end() );
		
		//add to closed list
		
		//check each adjacent node
		//up
		if( ( current->up != NULL )  && ( current->up->walkable ) )
		{
				int cost = current->cost + 1;
				
				int hcost = cost + abs(current->x - end->x ) 
								 + abs(current->y - end->y ) );
		
			//if its in open list
			if()
			{
				//calculate costs when using this node
				
				
				//if they are less, then change stuff
				if( cost < current->cost )
				{
					current->up->parent = current;
					current->up->cost = cost;
					current->up->hcost = hcost;
				}
			}
			//if it isn't in openlist, add it
			else
			{
				current->up->parent = current;
				current->up->cost = cost;
				current->up->hcost = hcost;
			}
		}
		//repeat for all other directions
		
	}
	
	//wipe all astar data
	
	//set obj's path
}