#include "Graph.h"

#include <thread>

const int constNodeSize = 20;

Graph::Graph()
	:m_VertexCount(0)
{
	m_rendererPtr = std::make_shared<Renderer>("NULL");
}


Graph::Graph(const char* windowTitle, int V)
	:m_VertexCount(V)
{	
	m_rendererPtr = std::make_shared<Renderer>(windowTitle);
	adjList.resize(V);
}

void Graph::AddEdge(Node source, Node dest, bool isBidirectional)
{
	adjList[source.node_number - 1].push_back(dest);
	if (isBidirectional)
		adjList[dest.node_number - 1].push_back(source);
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

static void Draw(std::shared_ptr<Renderer> renderer, const Node& node)
{
	printf("Rendering Node: %d\n", node.node_number);
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

void RenderGraph(std::shared_ptr<Renderer> rendererPtr, const int vertsCount, const std::vector< Vector<Node> >& adjList)
{
	//Fill the background color;
	rendererPtr->FillScreen(0, 0, 0, 255);
	bool isWindow = true;
	bool* drawn = new bool[vertsCount];
	Vector2<int>* nodeCoords = new Vector2<int>[vertsCount];
	while (isWindow)
	{
		ProcessInput(isWindow);
		rendererPtr->FillScreen(0, 0, 0, 255);
		//render vertices/nodes -- TODO: could try using BFS or DFS, current algorithm is sluggish
		memset(drawn, (int)false, vertsCount);
		memset(nodeCoords, -1, sizeof(Vector<int>) * vertsCount);
		Vector2<int> srcNode, destNode;
		for (auto i = 0; i < adjList.size(); i++)
		{
			for (const auto& node : adjList[i])
			{
				destNode = node.position;
				if (nodeCoords[i] != Vector2<int>(-1, -1))
					srcNode = nodeCoords[i];
				else
					srcNode = { -1, -1 };
				if (!drawn[node.node_number - 1])
				{
					//Fill the coordinate vec2 array
					nodeCoords[node.node_number - 1] = node.position;
					//draws the node if it is not drawn
					Draw(rendererPtr, node);
					//draws the edge, if it is not drawn, and the source is known
					if (srcNode != Vector2<int>(-1, -1))
						SDL_RenderDrawLine(rendererPtr->GetRendererPtr(), srcNode.x, srcNode.y, destNode.x, destNode.y);
					//then marks the node as being drawn
					drawn[node.node_number - 1] = true;
				}
			}
		}

		//Swap the buffers finally, after all nodes and lines have been drawn
		rendererPtr->SwapBuffers();
	}
	delete[] drawn;
}

void Graph::Render()
{
	m_rendererPtr->FillScreen(0, 0, 0, 255);
	std::thread renderThread(RenderGraph, m_rendererPtr, m_VertexCount, std::ref(adjList));
	renderThread.join();
}