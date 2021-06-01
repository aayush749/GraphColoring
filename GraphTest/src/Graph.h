#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>

#include "Core_Components.h"
#include <memory>

//class Node;

class Node 
{
public:
	int node_number;
	Vector2<int> position;
	SDL_Color color;

	Node(int number)
		:node_number(number)
	{
		//Every node has a color white at first, by default
		color = { 255, 255, 255, 255 };
		position = { -1, -1 };
	}

	Node(int number, const Vector2<int>& pos)
	{
		node_number = number;
		position.x = pos.x;
		position.y = WINDOW_HEIGHT - pos.y;
		color = { 255, 0, 0, 255 };
	}

	void SetColor(SDL_Color& col)
	{
		color = col;
	}
};

struct Edge
{
private:
	Node m_src, m_dest;
public:
	Edge()
		:m_src(Node(-1, {-1, -1})), m_dest(Node(-1, {-1, -1}))
	{}
	void SetEdgeSourceNode(const Node& src)
	{
		m_src = src;
	}
	void SetEdgeDestinationNode(const Node& dest)
	{
		m_dest = dest;
	}
	Edge(const Node& src, const Node& dest)
		:m_src(src), m_dest(dest)
	{}
	const Node& GetSourceNode() const
	{
		return m_src;
	}
	const Node& GetDestinationNode() const
	{
		return m_dest;
	}

	Node& GetSourceNode()
	{
		return m_src;
	}

	Node& GetDestinationNode()
	{
		return m_dest;
	}
};

template<typename T>
class Vector : public std::vector<T>
{
public:

	const T& operator[](size_t index) const
	{
		return std::vector<T>::operator[](index - 1);
	}

	const T& operator[](Node& n) const
	{
		return operator[](n.node_number);
	}

	T& operator[](size_t index)
	{
		return std::vector<T>::operator[](index - 1);
	}

	T& operator[](Node& n)
	{
		return operator[](n.node_number);
	}


	const bool operator[](const Node& n) const
	{
		return std::vector<bool>::operator[](n.node_number - 1);
	}

	 bool operator[](const Node& n)
	{
		return std::vector<bool>::operator[](n.node_number - 1);
	}
};

class Graph
{
private:
	const char* m_Name;
	std::shared_ptr<Renderer> m_rendererPtr;
	int m_VertexCount;
	std::vector< Vector<Node> > adjList;
	std::vector<Edge> m_Edges;
public:
	Graph();

	Graph(const char* windowTitle, int V);

	void AddEdge(Node source, Node dest, bool isBidirectional = true);

	void BFS(Node source);

	void DFS(Node source);

	bool HasCycle(Node source);

	void Render(bool shouldPersist = true);

	void Color();
private:

	bool HasCycle_Helper(Node source, int parent, std::vector<bool>& visited);

	void DFS_helper(Node source, std::vector<bool>& visited);
};