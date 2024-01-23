#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<bits/stdc++.h>

using namespace sf;
using namespace std;


const int N = 10;

vector<pair<int, int>> adj[N*N]; // adjacent list
int dist[N*N]; // shortest distance to each node
int parent[N*N]; // parent node in shortest path

int get_node(int i, int j)
{
    return i*N + j;
}

void add_edge(int u, int v, int w)
{
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}

void Graph_Board() // to build the board
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int u = get_node(i, j);
            if (j < N-1)
            {
                int v = get_node(i, j+1);
                add_edge(u, v, 1);
            }
            if (i < N-1)
            {
                int v = get_node(i+1, j);
                add_edge(u, v, 1);
            }
        }
    }
}

bool is_invalid(int u) // for Check the inputs
{
    if(u < 0 || u >= N*N)
        return true;
    else
        return false;
}

bool check_adjacency(int u, int v) // to check if the two entered nodes are adjacent or not
{
    bool is_found_uv = false;
    bool is_found_vu = false;
    for (auto it = adj[u].begin(); it != adj[u].end(); it++)
    {
        if (it->first == v)
        {
            is_found_uv = true;
            break;
        }
    }
    for (auto it = adj[v].begin(); it != adj[v].end(); it++)
    {
        if (it->first == u)
        {
            is_found_vu = true;
            break;
        }
    }
    if (is_found_uv && is_found_vu)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void add_wall(int u, int v)
{
    if(check_adjacency(u, v))
    {
        adj[u].erase(remove_if(adj[u].begin(), adj[u].end(),
                               [&](const auto & p)
        {
            return p.first == v;
        }), adj[u].end());
        adj[v].erase(remove_if(adj[v].begin(), adj[v].end(),
                               [&](const auto & p)
        {
            return p.first == u;
        }), adj[v].end());
    }
    else
    {
        cout<< "There is no relation between node "<<u<< " and node "<< v<< " !!!"<<endl;
    }
}


void Shortest_Path(int s)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    fill(dist, dist+N*N, INT_MAX);
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        for (auto& p : adj[u])
        {
            int v = p.first, w = p.second;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

void Print_Board(int start, int End)
{
    cout << "Board: \n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int u = get_node(i, j);
            if (dist[u] == INT_MAX)
            {
                // node is unreachable
                cout << "X ";
            }
            else if (u == start)
            {
                // start node
                cout << "S ";
            }
            else if (u == End)
            {
                // end node
                cout << "E ";
            }
            else
            {
                // regular node
                cout << ". ";
            }
        }
        cout << "\n";
    }
}
int index=0;
bool reached_end=false;
vector<pair<int,int>> Vwall, Hwall;
vector<Text> counter;


int main()
{
    RenderWindow window(VideoMode(1110,1110),"Maze");

    window.setFramerateLimit(3);
    //build board
    vector<RectangleShape> squares;
    for(int i=0 ; i<10 ; i++)
    {
        for(int j = 0 ; j<10 ; j++)
        {
            RectangleShape rectangle(Vector2f(100.f, 100.f));
            rectangle.setPosition(j*(100+10)+10,i*(100+10)+10);
            rectangle.setFillColor(Color::Blue);
            squares.push_back(rectangle);
        }
    }

    RectangleShape frame1;
    frame1.setSize(Vector2f(1100,10));
    frame1.setFillColor(Color::White);
    frame1.setPosition(0,0);

    RectangleShape frame2;
    frame2.setSize(Vector2f(1100,10));
    frame2.setFillColor(Color::White);
    frame2.setPosition(0,window.getSize().y-10);

    RectangleShape frame3;
    frame3.setSize(Vector2f(10,1110));
    frame3.setFillColor(Color::White);
    frame3.setPosition(window.getSize().x-10,0);

    RectangleShape frame4;
    frame4.setSize(Vector2f(10,1100));
    frame4.setFillColor(Color::White);
    frame4.setPosition(0,0);

    Texture robotTex ;
    if(!robotTex.loadFromFile("images/robot3.png"))
    {
        return -1;
    }
    Sprite robot(robotTex);

    SoundBuffer sound_pass, sound_victory ;
    if (!sound_pass.loadFromFile("music/jump.wav"))
    {
        return -1;
    }
    if(!sound_victory.loadFromFile("music/victory.wav"))
    {
        return -1;
    }
    Sound s_pass, s_victory;
    s_pass.setBuffer(sound_pass);
    s_victory.setBuffer(sound_victory);
//initialize graph board

    Graph_Board();
    int start, End;
    cout << "Enter the start node: ";
    cin >> start;
    while(is_invalid(start))
    {
        cout << "out of range node!!!(0-99)."<<endl;
        cout << "Enter the start node again: ";
        cin >> start;
    }

    cout << "Enter the end node: ";
    cin >> End;
    while(is_invalid(End))
    {
        cout << "out of range node!!!(0-99)."<<endl;
        cout << "Enter the end node again: ";
        cin >> End;
    }
// Ask user if they want to add walls

    char Check;
    while (true)
    {
        cout << "Do you want to add a wall? (Y/N): ";
        cin >> Check;
        if (tolower(Check) == 'y')
        {
            int u, v;
loop :
            cout << "Enter the two nodes to add the wall between: ";
            cin >> u >> v;

            while(is_invalid(u) || is_invalid(v))
            {
                cout<< "Invalid Nodes!!!"<<endl;
                cout << "Enter the two nodes again to add the wall between: ";
                cin >> u >> v;
            }

            if(abs(u-v)==1)
            {
                if(u>v)
                {
                    swap(u,v);
                }
                Hwall.push_back({u,v});
            }
            else if(abs(u-v)==10)
            {
                if(u>v)
                {
                    swap(u,v);
                }
                Vwall.push_back({u,v});
            }

            add_wall(u, v);
        }
        else if (tolower(Check) == 'n')
        {
            break;
        }
        else
        {
            cout << "Invalid input." << endl;
        }
    }
    // to get the shortest path

    Shortest_Path(start);

    //unreachable nodes

    bool unreachable = false;
    for (int i = 0; i < N*N; i++)
    {
        if (dist[i] == INT_MAX)
        {
            cout << "Node " << i << " is unreachable.\n";
            unreachable = true;

        }
    }

    // Edit the walls
    if(unreachable)
    {
        Print_Board(start,End);
        Graph_Board();
        cout<< "All the walls has been removed . \n"
            <<"Edit the walls again !!!\n";

        Vwall.clear();
        Hwall.clear();

        goto loop;
    }

    // Print the shortest path
    vector<int> path;
    if (dist[End] == INT_MAX)
    {
        cout << "No path found.\n";
    }
    else
    {

        cout << "Shortest distance: " << dist[End] << "\n";

        for (int u = End; u != start; u = parent[u])
        {
            path.push_back(u);
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
        cout << "Shortest path: ";
        for (int u : path)
        {
            cout << u << " ";
        }
        cout << "\n";
    }
    Print_Board(start, End);

    //
    robot.setPosition(squares[start].getPosition().x,squares[start].getPosition().y);
    Font f;
    if(!f.loadFromFile("fonts/COOPBL.ttf"))
    {
        return -1;
    }

    Text s("START",f,25), e("END",f,25) /*, m("KOLO\nTMAM",f,35)*/;
    s.setFillColor(Color::Black);
    s.setPosition(squares[start].getPosition().x+7,squares[start].getPosition().y+40);

    e.setFillColor(Color::Black);
    e.setPosition(squares[End].getPosition().x+15,squares[End].getPosition().y+45);

    /* m.setFillColor(Color::White);
     m.setPosition(squares[End].getPosition().x+77,squares[End].getPosition().y);
     m.setStyle(Text::Bold);*/


    for(int i=1 ; i<path.size()-1; i++)
    {
        Text num(to_string(i),f,25);
        num.setFillColor(Color::Black);
        num.setPosition(squares[path[i]].getPosition().x+45,squares[path[i]].getPosition().y+40);
        counter.push_back(num);
    }







    while(window.isOpen())
    {
        Event event;

        while(window.pollEvent(event))
        {
            if(event.type==Event::Closed)
                window.close();
            if(event.type==Event::KeyPressed)
            {
                if(event.key.code==Keyboard::Escape)
                {
                    window.close();
                    cout<<"TMAM"<<endl;
                }

            }
        }

        //logic
        if(abs(path[index]-path[index+1])==1 && index < path.size()-1)
        {
            if(index==0)
            {
                squares[path[index]].setFillColor(Color::Magenta);
            }
            else
                squares[path[index]].setFillColor(Color::Cyan);
            if(path[index]-path[index+1]<0)
            {
                robot.move(110,0);
                s_pass.play();
                index++;
            }
            else
            {
                robot.move(-110,0);
                s_pass.play();
                index++;
            }

        }
        else if(abs(path[index]-path[index+1])==10 && index < path.size()-1)
        {
            squares[path[index]].setFillColor(Color::Cyan);
            if(path[index]-path[index+1]<0)
            {
                robot.move(0,110);
                s_pass.play();
                index++;
            }
            else
            {
                robot.move(0,-110);
                s_pass.play();
                index++;
            }
        }
        else if(index==path.size()-1)
        {

            if(abs(path[index]-path[index-1])==10)
            {
                squares[path[index]].setFillColor(Color::Green);
                index++;
            }
            else if(abs(path[index]-path[index-1])==1)
            {
                squares[End].setFillColor(Color::Green);
                index++;
            }
            s_victory.play();
            reached_end=true;
        }










        window.clear();

        //draw
        for(auto& rec : squares)
        {
            window.draw(rec);

        }
        window.draw(robot);
        window.draw(s);
        window.draw(e);
        /*if(reached_end)
        {
            window.draw(m);
        }*/

        for(Text i : counter)
        {
            window.draw(i);
        }

        for(int i=0 ; i<Vwall.size(); i++ )
        {
            RectangleShape V_wall;
            V_wall.setSize(Vector2f(100,10));
            V_wall.setPosition(squares[Vwall[i].first].getPosition().x,squares[Vwall[i].second].getPosition().y-10);
            V_wall.setFillColor(Color::Red);
            window.draw(V_wall);
        }

        for(int i=0 ; i<Hwall.size(); i++ )
        {
            RectangleShape H_wall;
            H_wall.setSize(Vector2f(10,100));
            H_wall.setPosition(squares[Hwall[i].second].getPosition().x-10,squares[Hwall[i].second].getPosition().y);
            H_wall.setFillColor(Color::Red);
            window.draw(H_wall);
        }

        window.draw(frame1);
        window.draw(frame2);
        window.draw(frame3);
        window.draw(frame4);
        //display
        window.display();
    }



    return 0;
}
