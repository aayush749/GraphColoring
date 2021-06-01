#pragma once

#include <iostream>
#include <vector>
//#include <unordered_map>
#include <queue>

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
	std::shared_ptr<Renderer> m_rendererPtr;
	int m_VertexCount;
	std::vector< Vector<Node> > adjList;

public:
	Graph();

	Graph(const char* windowTitle, int V);

	void AddEdge(Node source, Node dest, bool isBidirectional = true);

	void BFS(Node source);

	void DFS(Node source);

	bool HasCycle(Node source);

	void Render();
private:

	bool HasCycle_Helper(Node source, int parent, std::vector<bool>& visited);

	void DFS_helper(Node source, std::vector<bool>& visited);
};
