#include "Core_Components.h"
#include "Graph.h"

int game_is_running = TRUE;

const int XCENTER = WINDOW_WIDTH / 2;
const int YCENTER = WINDOW_HEIGHT / 2;

int main()
{
	
	Graph graph("Graph 1", 6);
	graph.AddEdge(Node(1, { XCENTER + 200, YCENTER }), Node(2, { XCENTER + 100, YCENTER }));
	graph.AddEdge(Node(2, { XCENTER + 100, YCENTER }), Node(3, { XCENTER - 50, YCENTER }));
	graph.AddEdge(Node(2, { XCENTER + 100, YCENTER }), Node(4, { XCENTER + 200, YCENTER + 100 }));
	graph.AddEdge(Node(4, { XCENTER + 200, YCENTER + 100 }), Node(5, { XCENTER, YCENTER + 200 }));
	graph.AddEdge(Node(5, { XCENTER, YCENTER + 200 }), Node(6, { XCENTER + 200, YCENTER + 300 }));
	

	printf("\nBFS: ");
	graph.BFS(1);
	printf("\nDFS: ");
	graph.DFS(1);

	Graph cyclic("Graph 2 - cyclic", 5);
	cyclic.AddEdge(Node(1, { XCENTER + 100, YCENTER }), Node(2, { XCENTER, YCENTER }));
	cyclic.AddEdge(Node(2, { XCENTER, YCENTER }), Node(3, { XCENTER - 100, YCENTER + 100 }));
	cyclic.AddEdge(Node(3, { XCENTER - 100, YCENTER + 100 }), Node(4, { XCENTER, YCENTER + 200 }));
	cyclic.AddEdge(Node(4, { XCENTER, YCENTER + 200 }), Node(5, { XCENTER + 100, YCENTER + 200 }));
	cyclic.AddEdge(Node(4, { XCENTER, YCENTER + 200 }), Node(1, { XCENTER + 100, YCENTER }));
	printf(cyclic.HasCycle(1) ? "\nHas a cycle\n" : "\nDoes not have a cycle\n");
	
	graph.Color();
	graph.Render(false);

	cyclic.Color();
	cyclic.Render(false);
	std::cin.get();

	return 0;
}