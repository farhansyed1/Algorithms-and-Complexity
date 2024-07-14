
#include<iostream>
#include<string>
#include<string.h>

using namespace std;

/*
* Authors: Farhan Syed & Abhinav Sasikumar
* Year: 2023
*/
int main(int argc, char const *argv[])
{
    //IO
    std::ios::sync_with_stdio(false);
    cin.tie(0);

    // Read vertices, edges and max numnber of colors of the graph
    int v,e,m;
    cin >> v >> e >> m;

    // A minimum of 3 roles, 2 scenes, and 3 people are needed for the reduced problem. 
    int roles = v + 2;         // v represents orignal roles. +2 represents 2 more roles for people 1 and 2
    int scenes = 2*v + e;     // 2*v represents the scenes between person 1/2 and other roles. e represents scenes for each edge in graph   
    int people = m + 2;        // m represents origninal people. +2 represents 2 more people (1 and 2)

    // The first three rows are roles, scenes and people
    cout << roles << "\n" << scenes << "\n" << people << "\n";
    
    // ROLES
    
    // Roles (roles-1) and (roles) are given to person 1 and 2, respectively. 
    // Distribute the remaining roles (1 to roles-2) to the other people (person 3 to last person).
    string remainingRoles = to_string(m);
    for(int i=3;i<=people;i++){
        remainingRoles.append(" " + to_string(i));
    }
    // Print remaining roles (v times)
    for(int i=0;i<v;i++){
        cout << remainingRoles << "\n"; 
    }

    // Give last two roles to person 1 and 2 
    cout << 1 << " " << 1 << "\n" << 1 << " " << 2 << "\n";

    // SCENES

    // All roles are given a scene with person 1 (role: roles-1) and person 2 (role: roles). 
    // This is to prevent monologues and to ensure that all roles are used in a scene. 
    for(int i = 1; i<=roles-2;i++){
        cout << 2 << " "  << roles << " "  << i << "\n";        // Scene with person 2
        cout << 2 << " "  << (roles-1) << " "  << i << "\n";    // Scene with person 1
    }

    // Read the graph edges and convert to scenes 
    for (int i = 0; i < e; i++) {
        int a, b;
        cin >> a >> b;
        cout << 2 << " "  << a << " "  << b << "\n";
    } 
    
    return 0;
}
