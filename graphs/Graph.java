import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

public class Graph {
	private LinkedList<String> vertexes;
	private LinkedHashMap<String, LinkedList<Edge>> connections;
	
	public void dfsRec(String vertex, LinkedList<String> visited) {
		if (!visited.contains(vertex)) {
			visited.add(vertex);
			LinkedList<Edge> edges = connections.get(vertex);
			for (Edge e : edges) {
				dfsRec(e.getTo(), visited);
			}
		}
	}
	
	public LinkedList<String> dfsIterative(String startVertex) {
		LinkedList<String> visited = new LinkedList<String>();
		Stack<String> xVisit = new Stack<String>();
		
		xVisit.push(startVertex);
		while (!xVisit.isEmpty()) {
			String vertex = xVisit.pop();
			if (!visited.contains(vertex)) {
				visited.add(vertex);
				LinkedList<Edge> edges = connections.get(vertex);
				for (Edge e : edges) {
					xVisit.push(e.getTo());
				}
			}
		}
		return visited;
	}
	
	public LinkedList<String> bfs(String startVertex) {
		LinkedList<String> visited = new LinkedList<String>();
		Queue<String> xVisit = new LinkedList<String>();
		
		xVisit.offer(startVertex);
		while (!xVisit.isEmpty()) {
			String vertex = xVisit.poll();
			if (!visited.contains(vertex)) {
				visited.add(vertex);
				LinkedList<Edge> edges = connections.get(vertex);
				for (Edge e : edges) {
					xVisit.offer(e.getTo());
				}
			}
		}
		return visited;
	}
	
	public int countSubGraphs() {
		int count = 0;
		
		LinkedList<String> visited = new LinkedList<String>();
		for (String vertex : vertexes) {
			if (!visited.contains(vertex)) {
				count++;
				dfsRec(vertex, visited);
			}
		}
		
		return count;
	}
	
	public static void main(String args[]) {
		Graph g = new Graph();
		
		// populate graph
		
		LinkedList<String> visited = new LinkedList<String>();
		g.dfsRec("0", visited);
	}
}
