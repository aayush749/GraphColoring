#include "Graph.h"

#include <thread>

const int constNodeSize = 20;

Graph::Graph()
	:m_VertexCount(0)
{
	m_Name = "NULL";
	//m_rendererPtr = std::make_shared<Renderer>("NULL");
}


Graph::Graph(const char* windowTitle, int V)
	:m_VertexCount(V)
{	
	m_Name = windowTitle;
	//m_rendererPtr = std::make_shared<Renderer>(windowTitle);
	adjList.resize(V);
}

void Graph::AddEdge(Node source, Node dest, bool isBidirectional)
{
	Edge edge;
	adjList[source.node_number - 1].push_back(dest);
	edge.SetEdgeSourceNode(source);
	if (isBidirectional)
	{
		adjList[dest.node_number - 1].push_back(source);
		edge.SetEdgeDestinationNode(dest);
	}
	m_Edges.push_back(edge);
}

void Graph::BFS(Node source)
{
	std::queue<Node> q;
	std::vector<bool> visited;
	visited.resize(m_VertexCount);

	q.push(source);
	visited[source.node_number - 1] = true;

	while (!q.empty())
	{
		printf("%d, ", q.front().node_number);
		Node node = q.front();
		q.pop();
		for (Node nbr : adjList[node.node_number - 1])
		{
			if (!visited[nbr.node_number - 1])
			{
				q.push(nbr);
				visited[nbr.node_number - 1] = true;
			}
		}
	}
}

void Graph::DFS(Node source)
{

	Vector<bool> visited;
	visited.resize(m_VertexCount);
	DFS_helper(source, visited);
}

bool Graph::HasCycle(Node source)
{
	Vector<bool> visited;
	visited.resize(m_VertexCount);
	return HasCycle_Helper(source, -1, visited);
}

bool Graph::HasCycle_Helper(Node source, int parent, std::vector<bool>& visited)
{
	visited[source.node_number - 1] = true;
	for (Node nbr : adjList[source.node_number - 1])
		if (!visited[nbr.node_number - 1])
		{
			bool result = HasCycle_Helper(nbr, source.node_number, visited);
			return result;
		}
		else if (visited[nbr.node_number - 1] && !(nbr.node_number == parent))
			return true;

	return false;
}

void Graph::DFS_helper(Node source, std::vector<bool>& visited)
{
	printf("%d, ", source.node_number);
	visited[source.node_number - 1] = true;

	for (Node nbrs : adjList[source.node_number - 1])
		if (!visited[nbrs.node_number - 1])
			DFS_helper(nbrs, visited);
}

static void Draw(std::shared_ptr<Renderer> renderer, const Node& node, const char* title)
{
	printf("Rendering Graph: %s, Node: %d\n", title, node.node_number);
	SDL_Rect rect_node = {
		node.position.x,
		node.position.y,
		constNodeSize,
		constNodeSize
	};

	SDL_Renderer* rendererPtr = renderer->GetRendererPtr();

	//Fill Color
	SDL_SetRenderDrawColor(rendererPtr, node.color.r, node.color.g, node.color.b, node.color.a);

	//Draw a rectangle for the node
	SDL_RenderDrawRect(rendererPtr, &rect_node);
	SDL_RenderFillRect(rendererPtr, &rect_node);

	/*renderer->FillScreen(0, 0, 0, 1);*/
}

static void ProcessInput(bool& isWindow)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	//TODO: May add confirmation to quit as a win32 api dialog box
	switch (event.type)
	{

	case SDL_QUIT:
		isWindow = false;
		printf("Closing window!\n");
		break;
	case SDL_KEYDOWN:
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			isWindow = false;
			break;
		}
		break;
	}
	}
}

void Graph::Render(bool shouldPersist)
{
	m_rendererPtr = std::make_shared<Renderer>(m_Name);

	bool isWindow = true;
	bool* drawn = new bool[m_VertexCount];
	while (isWindow)
	{
		SDL_Delay(FRAME_TARGET_TIME);
		ProcessInput(isWindow);
		//Fill the screen
		m_rendererPtr->FillScreen(255, 255, 255, 255);
		
		memset(drawn, (int)false, m_VertexCount);
		for (auto i = 0; i < adjList.size(); i++)
		{
			for (const auto& edge : m_Edges)
			{
				const auto& srcNode = edge.GetSourceNode();
				if (!drawn[srcNode.node_number - 1])
				{
					//draws the node if it is not drawn
					Draw(m_rendererPtr, srcNode, m_Name);
					//then marks the node as being drawn
					drawn[srcNode.node_number - 1] = true;
				}
				const auto& destNode = edge.GetDestinationNode();
				if (!drawn[destNode.node_number - 1])
				{
					//draws the node if it is not drawn
					Draw(m_rendererPtr, destNode, m_Name);
					//then marks the node as being drawn
					drawn[destNode.node_number - 1] = true;
				}
			}
		}


		//Draw all the edges in single pass
		for (const Edge& edge : m_Edges)
		{
			SDL_RenderDrawLine(m_rendererPtr->GetRendererPtr(),
				edge.GetSourceNode().position.x, edge.GetSourceNode().position.y,
				edge.GetDestinationNode().position.x, edge.GetDestinationNode().position.y);
		}

		//Swap the buffers finally, after all nodes and lines have been drawn
		m_rendererPtr->SwapBuffers();
	}
	delete[] drawn;
	
	if(!shouldPersist)
		m_rendererPtr->~Renderer();
}

void Graph::Color()
{
	//Could also use a color palette like this to do the coloring
	{
		SDL_Color palette[] = {
		{255, 0, 0, 255},
		{0, 255, 0, 255},
		{0, 0, 255, 255},
		{150, 200, 200, 255},
		{200, 150, 255, 255},
		{200, 150, 100, 255}
		};
	}
	// keep track of the color assigned to each vertex
	std::unordered_map<int, int> result;

	// assign a color to vertex one by one
	for (int u = 0; u < m_VertexCount; u++)
	{
		// set to store the color of adjacent vertices of `u`
		std::set<int> assigned;

		// check colors of adjacent vertices of `u` and store them in a set
		for (Node& i : adjList[u])
		{
			if (result[i.node_number - 1]) {
				assigned.insert(result[i.node_number - 1]);
			}
		}

		// check for the first free color
		int color = 1;
		for (auto& c : assigned)
		{
			if (color != c) {
				break;
			}
			color++;
		}

		// assign vertex 'u' the first available color
		result[u] = color;
	}

	//color assigned to vertex V is result[V], set the colors accordingly
	SDL_Color baseColor = { 150, 150, 150, 255 };
	bool* isColored = new bool[m_VertexCount];
	memset(isColored, (int)false, sizeof(bool) * m_VertexCount);
	int i = 1;
	for (Edge& e : m_Edges)
	{
		SDL_Color color = { baseColor.r + 10 * i , baseColor.g + 20 * i, baseColor.b + 10 * i, baseColor.a };

		Node& src = e.GetSourceNode();
		Node& dest = e.GetDestinationNode();
		if (!isColored[src.node_number - 1])
		{
			color.r += 150 * result[src.node_number - 1];
			color.g += 200 * result[src.node_number - 1];
			color.b += 50 * result[src.node_number - 1];
			src.SetColor(color);

			//then mark the node as colored
			isColored[src.node_number - 1] = true;
		}
		if (!isColored[dest.node_number - 1])
		{
			color.r += 150 * result[dest.node_number - 1];
			color.g += 200 * result[dest.node_number - 1];
			color.b += 50 * result[dest.node_number - 1];
			dest.SetColor(color);

			//then mark the node as colored
			isColored[dest.node_number - 1] = true;
		}
	}
	delete[] isColored;
}